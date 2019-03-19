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

#include "urlmapper.h"

using namespace zuul_preview;


int main(int, char**)
{
  string input;
  URLMapper mapper;

  // For each request apache receieves, it sends us the HTTP host name
  // on standard input.  We use that to look up the build URL and emit
  // it on standard output.  Apache will send us one request at a time
  // (protected by an internal mutex) and expect exactly one line of
  // output for each.
  // Expected input:
  // https://zuul.opendev.org site.167715b656ee4504baa940c5bd9f3821.openstack.preview.opendev.org

  while (getline(cin, input)) {
      cout << mapper.map(input) << endl;
  }

  return EXIT_SUCCESS;
}
