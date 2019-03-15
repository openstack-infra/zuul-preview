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

/*
 * This program reads one line at a time on standard input, expecting
 * a specially formatted hostname from Apache's RewriteMap and uses
 * that to look up a build URL which it emits on standard output.
 */


#include <config.h>
#include "zuul_preview.h"


using namespace std;
using namespace zuul_preview;

vector<string> split(const string &in, char delim)
{
  istringstream stream(in);
  vector<string> parts;
  string part;
  while (getline(stream, part, delim)) {
    parts.push_back(part);
  }
  return parts;
}

int main(int, char**)
{
  string input;
  Cache cache{1024};
  ClientCache clients;

  // For each request apache receieves, it sends us the HTTP host name
  // on standard input.  We use that to look up the build URL and emit
  // it on standard output.  Apache will send us one request at a time
  // (protected by an internal mutex) and expect exactly one line of
  // output for each.
  // Expected input:
  // https://zuul.opendev.org site.167715b656ee4504baa940c5bd9f3821.openstack.preview.opendev.org
  while (getline(cin, input)) {

    // Split the input into api_url, hostname
    auto parts = split(input, ' ');
    if (parts.size() != 2) {
      cout << "NULL" << endl;
      continue;
    }
    auto api_url = parts[0];
    auto hostname = parts[1];

    // If we have the value in the cache, return it.
    if (auto val = cache.get(hostname)) {
      cout << val.value() << endl;
      continue;
    }

    // We use the first three parts of the hostname to look up the
    // build url.
    parts = split(hostname, '.');
    if (parts.size() < 3) {
      cout << "NULL" << endl;
      continue;
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

      cout << artifact_url << endl;

      cache.put(hostname, artifact_url);
    } catch (...) {
      // If anything goes wrong, we still need to return only a single
      // string to apache, and recover for the next request, so we
      // have a general exception handler here.
      cout << "NULL" << endl;
    }
  }

  return EXIT_SUCCESS;
}
