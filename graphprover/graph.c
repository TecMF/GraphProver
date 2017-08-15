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

#define TRACE(fmt, ...) g_debug ("%s: " fmt "\n", G_STRFUNC, __VA_ARGS__)


/* igraph attributes.  */

static int
ig_attr_init (igraph_t *graph, igraph_vector_ptr_t *attr)
{
  TRACE ("graph=%p attr=%p", graph, attr);
  attr = NULL;
  graph->attr = attr;
  return IGRAPH_SUCCESS;
}

static void
ig_attr_destroy (igraph_t *graph)
{
  TRACE ("graph=%p", graph);
}

static int
ig_attr_copy (igraph_t *to, const igraph_t *from,
              arg_unused (igraph_bool_t ga),
              arg_unused (igraph_bool_t va),
              arg_unused (igraph_bool_t ea))
{
  TRACE ("to=%p from=%p", to, from);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_add_vertices (igraph_t *graph,
                      long int nv,
                      igraph_vector_ptr_t *attr)
{
  TRACE ("graph=%p nv=%ld %p=attr", graph, nv, attr);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_permute_vertices (const igraph_t *graph,
                          igraph_t *newgraph,
                          const igraph_vector_t *idx)
{
  TRACE ("graph=%p newgraph=%p idx=%p\n", graph, newgraph, idx);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_combine_vertices (const igraph_t *graph,
                          igraph_t *newgraph,
                          arg_unused (const igraph_vector_ptr_t *merges),
                          arg_unused (const igraph_attribute_combination_t *cb))
{
  TRACE ("graph=%p newgraph=%p", graph, newgraph);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_add_edges (igraph_t *graph,
                   arg_unused (const igraph_vector_t *edges),
                   arg_unused (igraph_vector_ptr_t *attr))
{
  TRACE ("graph=%p", graph);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_permute_edges (const igraph_t *graph,
                       igraph_t *newgraph,
                       arg_unused (const igraph_vector_t *idx))
{
  TRACE ("graph=%p newgraph=%p", graph, newgraph);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_combine_edges (const igraph_t *graph,
                       igraph_t *newgraph,
                       arg_unused (const igraph_vector_ptr_t *merges),
                       arg_unused (const igraph_attribute_combination_t *cb))
{
  TRACE ("graph=%p newgraph=%p", graph, newgraph);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_info (const igraph_t *graph,
                  arg_unused (igraph_strvector_t *gnames),
                  arg_unused (igraph_vector_t *gtypes),
                  arg_unused (igraph_strvector_t *vnames),
                  arg_unused (igraph_vector_t *vtypes),
                  arg_unused (igraph_strvector_t *enames),
                  arg_unused (igraph_vector_t *etypes))
{
  TRACE ("graph=%p", graph);
  return IGRAPH_SUCCESS;
}

static igraph_bool_t
ig_attr_has_attr (const igraph_t *graph,
                  arg_unused (igraph_attribute_elemtype_t type),
                  arg_unused (const char* name))
{
  TRACE ("graph=%p", graph);
  return FALSE;
}

static int
ig_attr_get_type (const igraph_t *graph,
                  arg_unused (igraph_attribute_type_t *type),
                  arg_unused (igraph_attribute_elemtype_t elemtype),
                  arg_unused (const char *name))
{
  TRACE ("graph=%p", graph);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_numeric_graph_attr (const igraph_t *graph,
                                const char *name,
                                arg_unused (igraph_vector_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_string_graph_attr (const igraph_t *graph,
                               const char *name,
                               arg_unused (igraph_strvector_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_boolean_graph_attr (const igraph_t *graph,
                                const char *name,
                                arg_unused (igraph_vector_bool_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_numeric_vertex_attr (const igraph_t *graph,
                                 const char *name,
                                 arg_unused (igraph_vs_t vs),
                                 arg_unused (igraph_vector_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_string_vertex_attr (const igraph_t *graph,
                                const char *name,
                                arg_unused (igraph_vs_t vs),
                                arg_unused (igraph_strvector_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_boolean_vertex_attr
(const igraph_t *graph,
 const char *name,
 arg_unused (igraph_vs_t vs),
 arg_unused (igraph_vector_bool_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_numeric_edge_attr (const igraph_t *graph,
                               const char *name,
                               arg_unused (igraph_es_t es),
                               arg_unused (igraph_vector_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_string_edge_attr (const igraph_t *graph,
                              const char *name,
                              arg_unused (igraph_es_t es),
                              arg_unused (igraph_strvector_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static int
ig_attr_get_boolean_edge_attr (const igraph_t *graph,
                               const char *name,
                               arg_unused (igraph_es_t es),
                               arg_unused (igraph_vector_bool_t *value))
{
  TRACE ("graph=%p name=%s", graph, name);
  return IGRAPH_SUCCESS;
}

static G_GNUC_UNUSED igraph_attribute_table_t igraph_attrs = {
  ig_attr_init,
  ig_attr_destroy,
  ig_attr_copy,
  ig_attr_add_vertices,
  ig_attr_permute_vertices,
  ig_attr_combine_vertices,
  ig_attr_add_edges,
  ig_attr_permute_edges,
  ig_attr_combine_edges,
  ig_attr_get_info,
  ig_attr_has_attr,
  ig_attr_get_type,
  ig_attr_get_numeric_graph_attr,
  ig_attr_get_string_graph_attr,
  ig_attr_get_boolean_graph_attr,
  ig_attr_get_numeric_vertex_attr,
  ig_attr_get_string_vertex_attr,
  ig_attr_get_boolean_vertex_attr,
  ig_attr_get_numeric_edge_attr,
  ig_attr_get_string_edge_attr,
  ig_attr_get_boolean_edge_attr,
};


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

/* Checks if the object at index is a graph.
   If IG is non-NULL, stores the graph's igraph handle into *IG.  */
static inline Graph *
graph_check (lua_State *L, int index, igraph_t **ig)
{
  Graph *graph;
  graph = (Graph *) luaL_checkudata (L, index, GRAPH);
  if (ig != NULL)
    *ig = &graph->ig;
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
  luaL_setmetatable (L, GRAPH);

  return 1;
}

/***
 * Destroys graph.
 * @function __gc
 */
static int
__l_graph_gc (lua_State *L)
{
  igraph_t *ig;

  graph_check (L, 1, &ig);
  igraph_destroy (ig);

  return 0;
}

/***
 * Gets the number of nodes and number of edges in graph.
 * @function count
 * @return number of nodes and number of edges in graph
 */
static int
l_graph_count (lua_State *L)
{
  igraph_t *ig;

  graph_check (L, 1, &ig);
  lua_pushinteger (L, igraph_vcount (ig));
  lua_pushinteger (L, igraph_ecount (ig));

  return 2;
}

/***
 * Remove node from graph.
 */
static int
l_graph_delete (lua_State *L)
{
  igraph_t *ig;
  int id;
  int status;

  graph_check (L, 1, &ig);
  id = luaL_checkint (L, 2);

  status = igraph_delete_vertices (ig, igraph_vss_1 (id));
  lua_pushboolean (L, status == IGRAPH_SUCCESS);

  return 1;
}


/* Graph methods.  */
static const struct luaL_Reg methods[] = {
  {"__gc", __l_graph_gc},
  {"count", l_graph_count},
  {"delete", l_graph_delete},
  {"new", l_graph_new},
  {NULL, NULL},
};


int luaopen_graphprover_graph (lua_State *);

int
luaopen_graphprover_graph (lua_State *L)
{
  G_TYPE_INIT_WRAPPER ();

  igraph_i_set_attribute_table (&igraph_attrs);
  luax_newmetatable (L, GRAPH);
  luaL_setfuncs (L, methods, 0);

  return 1;
}
