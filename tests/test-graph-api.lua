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

local graph = require'graphprover.graph'
_ENV = nil

tests.ASSERT_CHECK_API {
   graph,
   __gc = 'function',
   __index = 'table',
   __metatable = 'string',
   count = 'function',
   new = 'function',
}
