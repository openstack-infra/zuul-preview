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

#ifndef CACHE_H_
#define CACHE_H_

#include <list>
#include <unordered_map>
#include <boost/optional.hpp>

using namespace std;


namespace zuul_preview {

// An LRU cache of hostname->URL mappings.
class Cache {
private:
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
  boost::optional<const string> get(const string &key);

  // Add an entry to the cache.  If the cache is full, drop the least
  // recently used entry.
  void put(const string &key, const string &value);
};

}  // namespace zuul_preview

#endif  // CACHE_H_
