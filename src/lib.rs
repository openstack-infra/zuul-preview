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
}

impl Query {
    pub fn new(line: String) -> Result<Query, &'static str> {
        let parts: Vec<&str> = line.split(' ').collect();

        if parts.len() != 2 {
            return Err("NULL");
        }

        let api_url = parts[0].to_string();
        let hostname = parts[1].to_string();

        Ok(Query { api_url, hostname })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn query_new_success() {
        let api_url = "/some/path";
        let hostname = "example.com";
        let line: String = format!("{} {}", api_url, hostname);
        let query = Query::new(line).unwrap();
        assert_eq!(query.api_url, api_url);
        assert_eq!(query.hostname, hostname);
    }

    #[test]
    fn query_new_fail() {
        match Query::new(String::from("")) {
            Ok(_) => panic!("Expected failure"),
            Err(e) => assert_eq!(e, "NULL"),
        };
    }
}
