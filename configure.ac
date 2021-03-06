# configure.ac -- Configure template for GraphProver.
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

AC_PREREQ([2.62])
AU_GIT_VERSION_GEN([graphprover])
AC_INIT([GraphProver],
  graphprover_version_string,
 [hermann@inf.puc-rio.br],
 [graphprover],
 [http://github.com/TecMF/GraphProver])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux])
AC_USE_SYSTEM_EXTENSIONS
AC_CONFIG_SRCDIR([graphprover/graph.c])
AC_CONFIG_HEADERS([graphprover/config.h])
AU_GITLOG_TO_CHANGELOG([2017-07-20])

AM_INIT_AUTOMAKE([1.14 -Wall -Werror dist-xz gnu no-dist-gzip])
AM_SILENT_RULES([yes])
AM_PROG_AR

LT_PREREQ([2.2])
LT_INIT([win32-dll])
AU_LIBTOOL_MODULE_LDFLAGS

# GraphProver (package) version.
AC_SUBST([GRAPHPROVER_VERSION_MAJOR], graphprover_version_major)
AC_SUBST([GRAPHPROVER_VERSION_MINOR], graphprover_version_minor)
AC_SUBST([GRAPHPROVER_VERSION_MICRO], graphprover_version_micro)
AC_SUBST([GRAPHPROVER_VERSION_STRING], graphprover_version_string)

# Dependencies version.
m4_define([glib_required_version], [2.32.4])
m4_define([igraph_required_version], [0.7.0])
m4_define([lua_required_version], [5.2])
AC_SUBST([GLIB_REQUIRED_VERSION], glib_required_version)
AC_SUBST([IGRAPH_REQUIRED_VERSION], igraph_required_version)
AC_SUBST([LUA_REQUIRED_VERSION], lua_required_version)

# Tool checks.
AU_PROG_CC_VISIBILITY
AU_PROG_PKG_CONFIG
AU_PROG_UNIX_TOOLS

# System checks.
AU_SYSTEM
AU_SYSTEM_MINGW

# Configure options.
AU_ARG_ENABLE_COVERAGE
AU_ARG_ENABLE_DEBUG
AU_ARG_ENABLE_VALGRIND

nw=
nw="$nw -Wsystem-headers"     # suppress system headers warnings
nw="$nw -Wpadded"             # our structs are not packed
nw="$nw -Wcast-qual"          # triggered by GLib headers
nw="$nw -Wc++-compat"         # ditto
AU_ARG_ENABLE_WARNINGS([$nw])

# Library functions.
AC_CHECK_LIBM
AU_CHECK_MACROS_H

# Check for GLib.
AU_VERSION_BREAK([glib], glib_required_version)
AU_CHECK_PKG([GLIB], glib-2.0 >= glib_required_version,
 [AC_LANG_PROGRAM([[
#include <glib.h>
#if !GLIB_CHECK_VERSION  \
    (GLIB_REQUIRED_MAJOR,\
     GLIB_REQUIRED_MINOR,\
     GLIB_REQUIRED_MICRO)
# error "glib is too old"
#endif
 ]])],
 [AC_LANG_PROGRAM([[]], [[glib_check_version (0, 0, 0);]])])

AC_MSG_CHECKING([if g_type_init must be explicitly called])
AC_LANG_PUSH([C])
saved_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $GLIB_CFLAGS"
AC_COMPILE_IFELSE(
 [AC_LANG_PROGRAM([[
#include <glib.h>
#if !GLIB_CHECK_VERSION (2,36,0)
# error "must call g_type_init"
#endif
 ]])],
 [must_call_g_type_init=no],
 [must_call_g_type_init=yes])
CFLAGS="$saved_CFLAGS"
AS_UNSET([saved_CFLAGS])
AC_LANG_POP
AS_IF([test "$must_call_g_type_init" = yes],
 [GLIB_CFLAGS="$GLIB_CFLAGS -DMUST_CALL_G_TYPE_INIT"])
AH_VERBATIM([G_TYPE_INIT_WRAPPER],
[/* Expands to g_type_init if GLIB < 2.36.  */
#ifdef MUST_CALL_G_TYPE_INIT
# define G_TYPE_INIT_WRAPPER() g_type_init ()
#else
# define G_TYPE_INIT_WRAPPER()
#endif
])
AC_MSG_RESULT([$must_call_g_type_init])

# Check for igraph.
AU_VERSION_BREAK([igraph], igraph_required_version)
AU_CHECK_PKG([IGRAPH], igraph >= igraph_required_version,
 [AC_LANG_PROGRAM([[
#include <igraph.h>
#if IGRAPH_VERSION_MAJOR
# if (IGRAPH_VERSION_MAJOR < IGRAPH_REQUIRED_MAJOR) \
     || (IGRAPH_VERSION_MINOR < IGRAPH_REQUIRED_MINOR) \
     || (IGRAPH_VERSION_PATCH <  IGRAPH_REQUIRED_MICRO)
#  error "igraph is too old"
# endif
#endif
 ]])],
 [AC_LANG_PROGRAM([[]], [[igraph_version (0, 0, 0, 0);]])])

# Check for Lua.
AU_CHECK_LUA(lua_required_version)
LUA_LIBS="$LUA_LIBS -export-dynamic"
AS_IF([test "$au_lua_version" = 5.3],
 [AC_DEFINE([LUA_COMPAT_APIINTCASTS], [1],
   [Define to 1 to enable Lua macros for integer manipulation])])
AC_SUBST([graphproverdatadir], ["${luadatadir}/$PACKAGE"])
AC_SUBST([graphproverlibdir], ["${lualibdir}/$PACKAGE"])

# With docs?
AU_ARG_WITH([doc], [build documentation])
AS_IF([test "$with_doc" = no], [],
 [AC_CHECK_PROGS([LDOC], [ldoc])
  AS_IF([test -z "$LDOC"],
   [AS_IF([test "$with_doc" = yes],
     [AC_MSG_ERROR([--with-doc requires ldoc])],
     [with_doc=no])],
   [with_doc=yes])])
AM_CONDITIONAL([WITH_DOC], [test "$with_doc" = yes])

# Output.
AC_CONFIG_FILES([
Makefile
graphprover/Makefile
tests/Makefile
])

AC_OUTPUT
AC_MSG_NOTICE([summary of build options:

  version:       ${VERSION}
  host:          ${host}
  prefix:        ${prefix}
  compiler:      ${CC}
  cppflags:      ${CPPFLAGS}
  cflags:        ${CFLAGS}
  ldflags:       ${LDFLAGS}
  warning flags: ${WERROR_CFLAGS} ${WARN_CFLAGS}
  library types: shared=${enable_shared}, static=${enable_static}
  valgrind:      ${VALGRIND}

  lua version:   ${au_lua_version}
  build docs:    ${with_doc}
])
