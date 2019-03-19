/* -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
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

#include <sstream>
#include "urlmapper.h"

namespace zuul_preview {

const vector<string> URLMapper::split(const string &in, char delim)
{
   istringstream stream(in);
   vector<string> parts;
   string part;
   while (getline(stream, part, delim)) {
      parts.push_back(part);
   }
   return parts;
}

const string URLMapper::map(const string &input)
{
  // Split the input into api_url, hostname
  auto parts = split(input, ' ');
  if (parts.size() != 2) {
    return string("NULL");
  }

  auto api_url = parts[0];
  auto hostname = parts[1];

  // If we have the value in the cache, return it.
  if (auto val = cache.get(hostname)) {
    return val.value();
  }

  // We use the first three parts of the hostname to look up the
  // build url.
  parts = split(hostname, '.');
  if (parts.size() < 3) {
    return string("NULL");
  }
  auto artifact = parts[0];
  auto buildid = parts[1];
  auto tenant = parts[2];

  try {
    // Use the Zuul API to look up the artifact URL.

    auto client = clients.get(api_url);
    auto uri = web::uri_builder("api/tenant");
    uri.append_path(tenant);
    uri.append_path("build");
    uri.append_path(buildid);
    auto response = client.request(
      web::http::methods::GET, uri.to_string()).get();
    // body is a web::json::value
    auto body = response.extract_json().get();
    auto artifacts = body["artifacts"].as_array();

    string artifact_url = "Artifact_not_found";
    for (uint i = 0; i < artifacts.size(); i++) {
      if (artifacts[i]["name"].as_string() == artifact) {
          artifact_url = artifacts[i]["url"].as_string();
      }
    }

    cache.put(hostname, artifact_url);
    return artifact_url;

  } catch (...) {
    // If anything goes wrong, we still need to return only a single
    // string to apache, and recover for the next request, so we
    // have a general exception handler here.
    return string("NULL");
  }
}

}  // namespace zuul_preview
