/* graphprover.graph -- Directed graph data structure.
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

#include <glib.h>
#include <lauxlib.h>
#include <lua.h>
#include "luax-macros.h"

PRAGMA_DIAG_PUSH ()
PRAGMA_DIAG_IGNORE (-Wundef)
PRAGMA_DIAG_IGNORE (-Wredundant-decls)
#include <igraph.h>
#include "igraphx.h"
PRAGMA_DIAG_POP ()

/***
 * Directed graph data structure.
 * @classmod graph
 */
#define GRAPH "graphprover.graph"

/* Graph object data.  */
typedef struct Graph
{
  igraph_t ig;
} Graph;

/* Checks if the object at INDEX is a graph.
   If IG is non-NULL, stores the graph's igraph handle into *IG.  */

static inline Graph *
graph_check (lua_State *L, int index, igraph_t *ig)
{
  Graph *graph;
  graph = (Graph *) luaL_checkudata (L, index, GRAPH);
  set_if_nonnull (ig, graph->ig);
  return graph;
}


/***
 * Creates a new graph.
 * @function new
 * @int[opt=0] n initial number of nodes
 * @return[1] the resulting graph
 * @return[2] `nil` plus error message
 */
static int
l_graph_new (lua_State *L)
{
  Graph *graph;
  int n;

  luax_optudata (L, 1, GRAPH);
  n = (int) CLAMP (luaL_optinteger (L, 2, 0), 0, G_MAXINT);

  graph = (Graph *) lua_newuserdata (L, sizeof (*graph));
  g_assert_nonnull (graph);
  igraphx_empty_attrs (&graph->ig, n, TRUE, NULL);

  return 1;
}

/***
 * Destroys graph.
 * @function __gc
 */
static int
__l_graph_gc (lua_State *L)
{
  igraph_t ig;
  graph_check (L, 1, &ig);
  igraphx_destroy (&ig);
  return 0;
}


/* Graph methods.  */
static const struct luaL_Reg methods[] = {
  {"new", l_graph_new},
  {"__gc", __l_graph_gc},
  {NULL, NULL},
};


int luaopen_graphprover_graph (lua_State *);

int
luaopen_graphprover_graph (lua_State *L)
{
  G_TYPE_INIT_WRAPPER ();
  luax_newmetatable (L, GRAPH);
  luaL_setfuncs (L, methods, 0);
  return 1;
}
