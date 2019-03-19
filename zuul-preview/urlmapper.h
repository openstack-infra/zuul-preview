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

#ifndef URLMAPPER_H_
#define URLMAPPER_H_

#include <string>
#include <vector>
#include "cache.h"
#include "clientcache.h"

using namespace std;


namespace zuul_preview {

class URLMapper {
private:
    Cache cache;
    ClientCache clients;

    const vector<string> split(const string &in, char delim);

public:
    URLMapper() : cache{1024}, clients{} { }
    const string map(const string &input);
};

}  // namespace zuul_preview

#endif  // URLMAPPER_H_
