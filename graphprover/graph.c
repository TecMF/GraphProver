/* graphprover.graph -- Graph data structure.
   Copyright (C) 2017 PUC-Rio/Laboratorio TecMF

This file is part of GraphProver.

GraphProver is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GraphProver is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GraphProver.  If not, see <http://www.gnu.org/licenses/>.  */

#include <config.h>
#include <assert.h>
#include <math.h>

#include <glib.h>
#include <lauxlib.h>
#include <lua.h>

int luaopen_graphprover_graph (lua_State *);

int
luaopen_graphprover_graph (lua_State *L)
{
  lua_pushboolean (L, TRUE);
  return 1;
}
