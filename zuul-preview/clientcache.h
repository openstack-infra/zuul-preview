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

#ifndef CLIENTCACHE_H_
#define CLIENTCACHE_H_

#include <unordered_map>
#include <cpprest/http_client.h>

using namespace std;


namespace zuul_preview {

class ClientCache {
private:
    unordered_map<string, web::http::client::http_client> clients;

public:
    ClientCache() : clients{} { }
    web::http::client::http_client get(const string &key);
};


}  // namespace zuul_preview

#endif  // CLIENTCACHE_H_
