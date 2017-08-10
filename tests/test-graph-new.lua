--[[ Copyright (C) 2017 PUC-Rio/Laboratorio TecMF

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
along with GraphProver.  If not, see <http://www.gnu.org/licenses/>.  ]]--

local tests = require'tests'
local assert = assert
local pcall = pcall

local graph = require'graphprover.graph'
_ENV = nil

local g = assert (graph.new ())
local n, e = assert (g:count ())
assert (n == 0 and e == 0)

local g = assert (g:new (1000))
local n, e = assert (g:count ())
assert (n == 1000 and e == 0)

local g = assert (g.new (-1))
local n, e = assert (g:count ())
assert (n == 0 and e == 0)

local g = assert (g:new (1000000))
local n, e = assert (g:count ())
assert (n == 1000000 and e == 0)
