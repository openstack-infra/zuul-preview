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


// NOTE(Shrews): Important to include gtest.h before any headers that include
// cpprest headers b/c of: https://github.com/Microsoft/cpprestsdk/issues/230
#include "gtest/gtest.h"
#include "zuul-preview/urlmapper.h"

namespace zuul_preview {

TEST(TestCache, GetReturnsOptional) {
    Cache cache{10};
    boost::optional<const string> ret = cache.get(string("missing"));
    ASSERT_FALSE(ret);
}

TEST(TestCache, PushGet) {
    Cache cache{10};
    string key = string("hostname");
    string value = string("url");
    cache.put(key, value);
    boost::optional<const string> ret = cache.get(key);
    ASSERT_TRUE(ret);
    ASSERT_EQ(value, *ret);
}

// We expect exactly 2 parts on the input line.
TEST(TestURLMapper, MapFailMissingInput) {
    URLMapper mapper;
    const string input("part1");
    const string ret = mapper.map(input);
    ASSERT_EQ("NULL", ret);
}

TEST(TestURLMapper, MapFailExtraInput) {
    URLMapper mapper;
    const string input("part1 part2 part3");
    const string ret = mapper.map(input);
    ASSERT_EQ("NULL", ret);
}

// We expect at least 3 parts to the hostname portion
TEST(TestURLMapper, MapFailHostnameParse) {
    URLMapper mapper;
    const string input("part1 invalid.part2");
    const string ret = mapper.map(input);
    ASSERT_EQ("NULL", ret);
}

}  // namespace zuul_preview
