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


/***
 * Directed graph data structure.
 * @classmod graph
 */
#define GRAPH "graphprover.graph"
#define GRAPH_REGISTRY_INDEX  (deconst (void *, &_graph_magic))
static const int _graph_magic = 0;

#define graph_registry_create(L)\
  luax_mregistry_create ((L), GRAPH_REGISTRY_INDEX)
#define graph_registry_get(L)\
  luax_mregistry_get ((L), GRAPH_REGISTRY_INDEX)

/* Graph object data.  */
typedef struct Graph
{
  igraph_t ig;                  /* underlying igraph */
  lua_State *L;                 /* associated lua_State */
} Graph;

/* Checks if the object at INDEX is a graph.
   If IG is non-NULL, stores the underlying igraph into *IG.  */
static inline Graph *
graph_check (lua_State *L, int index, igraph_t **ig)
{
  Graph *graph;
  graph = (Graph *) luaL_checkudata (L, index, GRAPH);
  g_assert_nonnull (graph);
  if (ig != NULL)
    *ig = &graph->ig;
  return graph;
}

/* Checks if the object stored in igraph IG is a graph.
   If L is non-null stores the associated Lua into *L.  */
static inline Graph *
graph_check_ig (igraph_t *ig, lua_State **L)
{
  Graph *graph;
  graph = ig->attr;
  g_assert_nonnull (graph);
  g_assert_nonnull (graph->L);
  if (L != NULL)
    *L = graph->L;
  return graph;
}

/* Pushes graph table (or one of its fields) onto stack.  */
static void
graph_table (lua_State *L, Graph *graph, const char *field)
{
  graph_registry_get (L);
  lua_rawgetp (L, -1, graph);
  lua_remove (L, -2);
  if (field != NULL)
    {
      lua_getfield (L, -1, field);
      lua_remove (L, -2);
    }
}


/* Initializes graph attributes.
   Called whenever a new graph is created.  */
static int
ig_attr_init (igraph_t *ig, igraph_vector_ptr_t *attr)
{
  Graph *graph;
  lua_State *L;

  ig->attr = attr;
  graph = graph_check_ig (ig, &L);
  TRACE ("ig=%p graph=%p L=%p", ig, graph, L);

  /* Creates a table for graph and adds it to module registry.  */
  graph_registry_get (L);
  lua_newtable (L);
  lua_newtable (L);
  lua_setfield (L, -2, "nodes"); /* node map */
  lua_newtable (L);
  lua_setfield (L, -2, "nodes-rev"); /* node reverse map */
  lua_rawsetp (L, -2, graph);
  lua_pop (L, 1);

  return IGRAPH_SUCCESS;
}

static void
ig_attr_destroy (igraph_t *ig)
{
  Graph *graph;
  lua_State *L;

  graph = graph_check_ig (ig, &L);
  TRACE ("ig=%p graph=%p L=%p", ig, graph, L);

  graph_registry_get (L);
  lua_pushvalue (L, -2);
  lua_pushnil (L);
  lua_rawset (L, -3);
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
ig_attr_add_vertices (igraph_t *ig,
                      long int nv,
                      igraph_vector_ptr_t *attr)
{
  Graph *graph;
  lua_State *L;
  const char *id;

  if (nv == 0)
    return IGRAPH_SUCCESS;      /* nothing to do */

  TRACE ("ig=%p nv=%ld id=%s", ig, nv, (char *) attr);
  g_assert (nv == 1);
  id = (const char *) attr;
  g_assert_nonnull (id);

  graph = (Graph *) ig->attr;
  g_assert_nonnull (graph);
  L = graph->L;
  g_assert_nonnull (L);

  /* Create table for the new node.  */
  lua_newtable (L);
  lua_pushstring (L, id);
  lua_setfield (L, -2, "_id");
  lua_pushinteger (L, nv - 1);
  lua_setfield (L, -2, "_index");
  _luax_dump_stack (L, 3);

  /* Update node map.  */
  graph_table (L, graph, "nodes");
  lua_pushvalue (L, -2);
  lua_setfield (L, -2, id);
  lua_pop (L, 1);

  /* Update node reverse map.  */
  graph_table (L, graph, "nodes-rev");
  lua_pushvalue (L, -2);
  lua_rawseti (L, -2, nv - 1);
  lua_pop (L, 1);

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

/* igraph attribute table.  */
static igraph_attribute_table_t igraph_attrs = {
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
 * Creates a new empty graph.
 * @function new
 * @return[1] the resulting graph
 * @return[2] `nil` plus error message
 */
static int
l_graph_new (lua_State *L)
{
  Graph *graph;

  luax_optudata (L, 1, GRAPH);
  graph = (Graph *) lua_newuserdata (L, sizeof (*graph));
  g_assert_nonnull (graph);
  graph->L = L;
  igraphx_empty_attrs (&graph->ig, 0, TRUE, graph);
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

/**
 * Adds node to graph.
 * @function add_node
 * @tparam string id Node id.
 * @return[1] `true` if successful
 * @return[2] `false` otherwise
 */
static int
l_graph_add_node (lua_State *L)
{
  igraph_t *ig;
  const char *id;

  graph_check (L, 1, &ig);
  id = luaL_checkstring (L, 2);
  igraphx_add_vertices (ig, 1, deconst (void *, id));
  lua_pushboolean (L, 1);

  return 1;
}

/***
 * Removes node from graph.
 */
static int
l_graph_remove_node (lua_State *L)
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
  {"add_node", l_graph_add_node},
  {"count", l_graph_count},
  {"new", l_graph_new},
  {"remove_node", l_graph_remove_node},
  {NULL, NULL},
};


int luaopen_graphprover_graph (lua_State *);

int
luaopen_graphprover_graph (lua_State *L)
{
  G_TYPE_INIT_WRAPPER ();
  igraph_i_set_attribute_table (&igraph_attrs);

  lua_newtable (L);
  graph_registry_create (L);

  luax_newmetatable (L, GRAPH);
  luaL_setfuncs (L, methods, 0);

  return 1;
}
