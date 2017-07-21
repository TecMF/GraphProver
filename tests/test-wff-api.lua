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
local wff = require'graphprover.wff'
_ENV = nil

tests.ASSERT_CHECK_API {
   wff,
   __eq = 'function',
   __index = 'table',
   __metatable = 'string',
   is_and = 'function',
   is_atom = 'function',
   is_iff = 'function',
   is_immediate_sub = 'function',
   is_imp = 'function',
   is_not = 'function',
   is_or = 'function',
   is_proper_sub = 'function',
   parse = 'function',
   sym = 'function',
   tag = 'function',
   unpack = 'function',
}
