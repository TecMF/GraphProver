/* igraphx.h -- Auxiliary macros for igraph.
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

#ifndef IGRAPHX_H
#define IGRAPHX_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <glib.h>
#include <igraph.h>

#define igraphx_ok(expr) g_assert ((expr) == IGRAPH_SUCCESS)
#define igraphx_empty_attrs(G, n, dir, attr)\
  igraphx_ok (igraph_empty_attrs ((G), (n), (dir), (attr)))
#define igraphx_destroy(G)\
  igraphx_ok (igraph_destroy ((G)))

#endif
