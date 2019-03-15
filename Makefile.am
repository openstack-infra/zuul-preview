# Copyright (c) 2019 Red Hat, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

ACLOCAL_AMFLAGS = -I m4

SUFFIXES =

SUBDIRS= .

DIST_SUBDIRS = ${SUBDIRS}

EXTRA_DIST = \
	${top_srcdir}/m4/*m4 \
	${top_srcdir}/bindep.txt \
	${top_srcdir}/README.rst


bin_PROGRAMS = zuul-preview/zuul-preview
zuul_preview_zuul_preview_LDADD = ${LTLIBCPPREST}
zuul_preview_zuul_preview_SOURCES = \
	zuul-preview/zuul_preview.cc \
	zuul-preview/main.cc
