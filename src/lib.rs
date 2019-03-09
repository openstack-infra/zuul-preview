/*
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

pub struct Query {
    pub api_url: String,
    pub hostname: String,
    pub artifact: String,
    pub buildid: String,
    pub tenant: String,
}

struct HostParts {
    artifact: String,
    buildid: String,
    tenant: String,
}

impl Query {
    pub fn new(line: String) -> Result<Query, &'static str> {
        let parts: Vec<&str> = line.split(' ').collect();

        if parts.len() != 2 {
            return Err("NULL");
        }

        let api_url = parts[0].to_string();
        let hostname = parts[1].to_string();

        let h = Query::parse_hostname(&hostname)?;

        Ok(Query {
            api_url: api_url,
            hostname: hostname,
            artifact: h.artifact,
            buildid: h.buildid,
            tenant: h.tenant,
        })
    }

    fn parse_hostname(hostname: &String) -> Result<HostParts, &'static str> {
        println!("Got hostname {}", hostname);
        let parts: Vec<&str> = hostname.split('.').collect();
        if parts.len() < 3 {
            return Err("NULL");
        }
        Ok(HostParts {
            artifact: parts[0].to_string(),
            buildid: parts[1].to_string(),
            tenant: parts[2].to_string(),
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn query_new_success() {
        let api_url = "/some/api/url";
        let hostname = "site.buildid.tenant.preview.opendev.org";
        let line: String = format!("{} {}", api_url, hostname);
        let query = Query::new(line).unwrap();
        assert_eq!(query.api_url, api_url);
        assert_eq!(query.hostname, hostname);
        assert_eq!(query.artifact, "site");
        assert_eq!(query.buildid, "buildid");
        assert_eq!(query.tenant, "tenant");
    }

    #[test]
    fn query_new_fail() {
        match Query::new(String::from("")) {
            Ok(_) => panic!("Expected failure"),
            Err(e) => assert_eq!(e, "NULL"),
        };
    }

    #[test]
    fn query_new_fail_bad_hostname() {
        let api_url = "/some/api/url";
        let hostname = "site.buildid";
        let line: String = format!("{} {}", api_url, hostname);
        match Query::new(line) {
            Ok(_) => panic!("Expected failure"),
            Err(e) => assert_eq!(e, "NULL"),
        };
    }

    #[test]
    fn parse_hostname_success() {
        let hostname = "site.buildid.tenant.preview.opendev.org".to_string();
        let hostparts = Query::parse_hostname(&hostname).unwrap();
        assert_eq!(hostparts.artifact, "site");
        assert_eq!(hostparts.buildid, "buildid");
        assert_eq!(hostparts.tenant, "tenant");
    }

    #[test]
    fn parse_hostname_fail() {
        let hostname = String::from("");
        match Query::parse_hostname(&hostname) {
            Ok(_) => panic!("Expected failure"),
            Err(e) => assert_eq!(e, "NULL"),
        };
    }
}
