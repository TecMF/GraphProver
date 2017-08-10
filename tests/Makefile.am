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

AM_CPPFLAGS=
AM_CFLAGS= $(WERROR_CFLAGS) $(WARN_CFLAGS) $(LUA_CFLAGS) $(GLIB_CFLAGS)
AM_LDFLAGS= -static $(LUA_LIBS) $(GLIB_LIBS)

# Auxiliary programs and scripts.
noinst_PROGRAMS=
noinst_SCRIPTS=

# Generate env.sh, env.csh, and gdb.cfg.
include $(top_srcdir)/build-aux/Makefile.am.env

# Configure Valgrind and generate suppfile from suppfile.m4.
include $(top_srcdir)/build-aux/Makefile.am.valgrind

# Setup test framework.
AM_COLOR_TESTS= always
DISABLE_HARD_ERRORS= true
TEST_EXTENSIONS= .lua .sh
LOG_COMPILER= $(_VALGRIND)
LUA_LOG_COMPILER= $(_VALGRIND) lua
SH_LOG_COMPILER= $(SHELL)

# Setup tests environment.
graph_so= graph$(SOEXT)
LD_PRELOAD= $(graph_so)
AM_TESTS_ENVIRONMENT=\
  export G_DEBUG="all";\
  export G_SLICE="always-malloc";\
  export LD_LIBRARY_PATH="$(top_builddir)/graphprover";\
  export LD_PRELOAD="$(LD_PRELOAD)";\
  export LUA_CPATH="$(builddir)/.libs/?$(SOEXT);$(top_builddir)/?$(SOEXT);;";\
  export LUA_PATH="$(top_srcdir)/?.lua;$(top_srcdir)/?/init.lua;$(top_srcdir)/tests/?.lua;;";\
  export MALLOC_CHECK_=1;\
  $(NULL)

# Test scripts.
scripts=
# graphprover.graph
scripts+= test-graph-api.lua
scripts+= test-graph-new.lua
# graphprover.wff
scripts+= test-wff-api.lua
scripts+= test-wff-new.lua
scripts+= test-wff-parse.lua
scripts+= test-wff-__eq.lua
scripts+= test-wff-is-immediate-sub.lua
check_SCRIPTS= $(scripts)

TESTS= $(check_SCRIPTS)
EXTRA_DIST+= $(check_SCRIPTS) tests.lua
CLEANFILES+= *.log *.trs
