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


#ifndef ZUUL_PREVIEW_H_
#define ZUUL_PREVIEW_H_

// This must be defined before we include the C++Rest header or else
// googletest complains.
// See: https://github.com/Microsoft/cpprestsdk/issues/230
#define _TURN_OFF_PLATFORM_STRING

#include <list>
#include <boost/optional.hpp>
#include <cpprest/http_client.h>


namespace zuul_preview {

using namespace std;


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


// An LRU cache of hostname->URL mappings.
class Cache {
  // A queue of hostname, URL pairs.  The head of the queue is always
  // the most recently accessed entry, the tail is the least.
  list<pair<const string, const string>> queue;

  // A map of hostname -> iterator that points into the queue, for
  // quick lookup.
  unordered_map<string, list<pair<const string, const string>>::iterator> map;

  // The maximum size of the cache.
  const uint32_t size;

public:
  Cache(uint s)
    : queue {}, map {}, size{s}
  { }

  // Lookup the hostname in the cache and return the URL if present.
  // If the entry is present, it is moved to the head of the queue.
  boost::optional<const string> get(const string &key)
  {
    auto location = map.find(key);
    if (location == map.end())
      return {};

    auto val = *(location->second);
    queue.erase(location->second);
    queue.push_front(val);
    return val.second;
  }

  // Add an entry to the cache.  If the cache is full, drop the least
  // recently used entry.
  void put(const string &key, const string &value)
  {
    auto location = map.find(key);
    if (location != map.end())
      return;

    if (queue.size() == size) {
      auto last = queue.back();
      queue.pop_back();
      map.erase(last.first);
    }

    queue.push_front(make_pair(key, value));
    map[key] = queue.begin();
  }
};


class ClientCache {
  unordered_map<string, web::http::client::http_client> clients;
public:
  ClientCache() : clients{} { }

  web::http::client::http_client get(const string &key)
  {
    auto location = clients.find(key);
    if (location == clients.end()) {
      auto value = web::http::client::http_client(key);
      clients.insert(make_pair(key, value));
      return value;
    }
    return location->second;
  }
};


}  // namespace zuul_preview

#endif  // ZUUL_PREVIEW_H_
