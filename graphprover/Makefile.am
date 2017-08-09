# Makefile.am -- Template for generating Makefile via Automake.
# Copyright (C) 2017 PUC-Rio/Laboratorio TecMF
#
# This file is part of GraphProver.
#
# GraphProver is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GraphProver is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GraphProver.  If not, see <http://www.gnu.org/licenses/>.

include $(top_srcdir)/build-aux/Makefile.am.common
include $(top_srcdir)/build-aux/Makefile.am.link

AM_CPPFLAGS=
AM_CFLAGS= $(WERROR_CFLAGS) $(WARN_CFLAGS)\
  $(GLIB_CFLAGS) $(IGRAPH_CFLAGS) $(LUA_CFLAGS)
AM_LDFLAGS= -module $(LT_MODULE_LDFLAGS)\
  $(GLIB_CFLAGS) $(IGRAPH_LIBS) $(LUA_LIBS)

# Lua/C modules.
graphproverlib_LTLIBRARIES= graph.la

# graph
graph_la_DEPENDENCIES=
graph_la_CFLAGS= $(AM_CFLAGS)
graph_la_LDFLAGS= $(AM_LDFLAGS)
graph_la_SOURCES= graph.c
