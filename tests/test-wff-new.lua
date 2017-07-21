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
local ipairs = ipairs

local wff = require'graphprover.wff'
_ENV = nil

-- Syntax errors.
assert (not wff.new'')
assert (not wff.new'1')
assert (not wff.new'1 1')
assert (not wff.new'a 1')
assert (not wff.new'()a 1')
assert (not wff.new'()')
assert (not wff.new'not a ()')
assert (not wff.new')not a(')
assert (not wff.new'not-a')
assert (not wff.new'and a')
assert (not wff.new' a ')

local f = wff.new ('a')
assert (f)
assert (f:tag () == 'atom' and f:sym () == 'a')
