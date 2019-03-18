dnl -*- bash -*-
dnl Process this file with autoconf to produce a configure script.

dnl Copyright 2009 Sun Microsystems, Inc.
dnl Copyright 2019 Red Hat, Inc.
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; version 2 of the License.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


AC_INIT(
  [zuul_preview],
  [1.0],
  [http://storyboard.openstack.org],
  [zuul-preview],
  [http://zuul-ci.org/])

AC_CONFIG_AUX_DIR([config])

AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability subdir-objects foreign tar-ustar])
AC_PREREQ(2.59)dnl		Minimum Autoconf version required.

AX_CHECK_ENABLE_DEBUG
AC_CANONICAL_HOST
AC_CANONICAL_BUILD
AC_ARG_PROGRAM
AC_USE_SYSTEM_EXTENSIONS

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_SRCDIR([zuul-preview/main.cc])

LT_PREREQ([2.4])
LT_INIT
LT_LANG([C++])

OPENDEV_CANONICAL_TARGET
REQUIRE_LIBCPPREST
REQUIRE_LIBGTEST

#########################################################################

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

echo "---"
echo "Configuration summary for $PACKAGE_NAME version $VERSION"
echo ""
echo "   * Installation prefix: $prefix"
echo "   * System type:         $host_vendor-$host_os"
echo "   * Host CPU:            $host_cpu"
echo "   * C++ Flags:           $AM_CXXFLAGS"
echo "   * CPP Flags:           $AM_CPPFLAGS"
echo "   * LIBS:                $LIBS"
echo "   * Debug enabled:       $enable_debug"
echo "   * Coverage enabled:    $enable_code_coverage"
echo ""
echo "---"
