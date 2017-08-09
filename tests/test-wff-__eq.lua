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

local wff = require'graphprover.wff'
_ENV = nil

-- atoms
assert (wff.parse'a' == wff.parse'a')
assert (wff.parse'a' == wff.parse'(a)')
assert (wff.parse'a' == wff.parse'((a))')
assert (wff.parse'a' ~= wff.parse'b')
assert (wff.parse'a' ~= wff.parse'A')
assert (wff.parse'a' ~= wff.parse'¬a')

-- not
assert (wff.parse'not a' == wff.parse'not a')
assert (wff.parse'¬a' == wff.parse'¬a')
assert (wff.parse'¬a' == wff.parse'((not a))')
assert (wff.parse'¬((a))' ~= wff.parse'((not b))')
assert (wff.parse'¬a' ~= wff.parse'not b')
assert (wff.parse'¬a' ~= wff.parse'a and a and a')

-- and
assert (wff.parse'a and a' == wff.parse'((a)and(a))')
assert (wff.parse'a and a' == wff.parse'(a∧a)')
assert (wff.parse'a∧(a∧a)' == wff.parse'a∧a∧a')
assert (wff.parse'a∧(a∧(a∧a))' == wff.parse'a∧a∧a∧a')
assert (wff.parse'a∧(a∧(¬a∧a))' == wff.parse'a∧a∧(not a)∧a')
assert (wff.parse'a∧a' ~= wff.parse'a∧b')
assert (wff.parse'a∧a' ~= wff.parse'a∧b')
assert (wff.parse'a∧b∧a' ~= wff.parse'(a and b) and a')
assert (wff.parse'a∧b∧a' ~= wff.parse'a and a and a')
assert (wff.parse'a∧a' ~= wff.parse'a∨a')

-- or
assert (wff.parse'a or a' == wff.parse'((a)or(a))')
assert (wff.parse'a or a' == wff.parse'(a∨a)')
assert (wff.parse'a∨(a∨a)' == wff.parse'a∨a∨a')
assert (wff.parse'a∨(a∨(a∨a))' == wff.parse'a∨a∨a∨a')
assert (wff.parse'a∨(a∨(¬a∨a))' == wff.parse'a∨a∨(not a)∨a')
assert (wff.parse'a∨a' ~= wff.parse'a∨b')
assert (wff.parse'a∨a' ~= wff.parse'a∨b')
assert (wff.parse'a∨b∨a' ~= wff.parse'(a or b) or a')
assert (wff.parse'a∨b∨a' ~= wff.parse'a or a or a')
assert (wff.parse'a∨a' ~= wff.parse'a∧a')

-- imp
assert (wff.parse'a imp a' == wff.parse'((a)imp(a))')
assert (wff.parse'a imp a' == wff.parse'(a→a)')
assert (wff.parse'a→(a→a)' == wff.parse'a→a→a')
assert (wff.parse'a→(a→(a→a))' == wff.parse'a→a→a→a')
assert (wff.parse'a→(a→(¬a→a))' == wff.parse'a→a→(not a)→a')
assert (wff.parse'a→a' ~= wff.parse'a→b')
assert (wff.parse'a→a' ~= wff.parse'a→b')
assert (wff.parse'a→b→a' ~= wff.parse'(a imp b) imp a')
assert (wff.parse'a→b→a' ~= wff.parse'a imp a imp a')
assert (wff.parse'a→a' ~= wff.parse'a∧a')

-- iff
assert (wff.parse'a iff a' == wff.parse'((a)iff(a))')
assert (wff.parse'a iff a' == wff.parse'(a↔a)')
assert (wff.parse'a↔(a↔a)' == wff.parse'a↔a↔a')
assert (wff.parse'a↔(a↔(a↔a))' == wff.parse'a↔a↔a↔a')
assert (wff.parse'a↔(a↔(¬a↔a))' == wff.parse'a↔a↔(not a)↔a')
assert (wff.parse'a↔a' ~= wff.parse'a↔b')
assert (wff.parse'a↔a' ~= wff.parse'a↔b')
assert (wff.parse'a↔b↔a' ~= wff.parse'(a iff b) iff a')
assert (wff.parse'a↔b↔a' ~= wff.parse'a iff a iff a')
assert (wff.parse'a↔a' ~= wff.parse'a∧a')
