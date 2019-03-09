/*
 *  Copyright (C) 2019 Clint Byrum
 *  Copyright (C) 2019 Red Hat, Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * This program reads one line at a time on standard input, expecting
 * a specially formatted hostname from Apache's RewriteMap and uses
 * that to look up a build URL which it emits on standard output.
 */

#[macro_use]
extern crate custom_error;
extern crate lru;

use std::io::{self, BufRead};

use lru::LruCache;
use reqwest::{self, Client, Url, UrlError};
use serde::{Deserialize};

use zuul_preview::Query;

custom_error! {PreviewError
    InvalidData{source: UrlError} = "Garbage In",
    Http{source: reqwest::Error} = "HTTP Fail",
    Io{source: io::Error} = "IO Things",
}

#[derive(Deserialize)]
struct ZuulArtifact {
    name: String,
    url: String,
}

#[derive(Deserialize)]
struct ZuulBuild {
    artifacts: Vec<ZuulArtifact>,
}

fn main() -> Result<(), PreviewError> {
    let mut cache = LruCache::new(1024);
    let client = Client::new();
    for line in io::stdin().lock().lines() {
        let line = line?;

        let query = match Query::new(line) {
            Ok(q) => q,
            Err(e) => {
                println!("{}", e);
                continue;
            },
        };

        if let Some(val) = cache.get(&query.hostname) {
            println!("{}", val);
            continue;
        }

        let parts: Vec<&str> = query.hostname.split('.').collect();
        if parts.len() < 3 {
            println!("NULL");
            continue;
        }
        let artifact = parts[0];
        let buildid = parts[1];
        let _tenant = parts[2];
        recoverable(|| {
            let base = Url::parse(&query.api_url[..])?;
            let url = base.join(&format!("/api/build/{}", buildid))?;
            let build: ZuulBuild = client.get(url).send()?.json()?;
            for build_artifact in build.artifacts {
                if build_artifact.name == artifact {
                    println!("{}", build_artifact.url);
                    cache.put(query.hostname.clone(), build_artifact.url.clone());
                } else {
                    println!("NULL");
                }
            }
            Ok(())
        });
    }
    Ok(())
}

fn recoverable<F>(mut func: F)
where
    F: FnMut() -> Result<(), PreviewError>,
{
    match func() {
        Err(_) => println!("NULL"),
        Ok(_) => (),
    }
}
