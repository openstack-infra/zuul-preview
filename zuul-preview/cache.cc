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

#include "cache.h"


namespace zuul_preview {

boost::optional<const string> Cache::get(const string &key)
{
    auto location = map.find(key);
    if (location == map.end())
      return {};

    auto val = *(location->second);
    queue.erase(location->second);
    queue.push_front(val);
    return val.second;
}

void Cache::put(const string &key, const string &value)
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

}  // namespace zuul_preview
