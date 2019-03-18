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


#include "zuul-preview/zuul_preview.h"
#include "gtest/gtest.h"

namespace {

using namespace std;
using namespace zuul_preview;

TEST(SplitTest, SplitSuccess) {
    vector<string> expected;
    expected.push_back(string("a"));
    expected.push_back(string("b"));
    expected.push_back(string("c"));
    vector<string> actual = split("a.b.c", '.');
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_EQ(expected, actual);
}

}  // namespace
