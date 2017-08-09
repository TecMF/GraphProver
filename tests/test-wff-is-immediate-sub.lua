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

-- Sanity checks.
assert (not (wff.parse'a'):is_immediate_sub (wff.new'a'))
assert (not (wff.parse'a'):is_immediate_sub (wff.new'b'))
assert (not (wff.parse'¬a'):is_immediate_sub (wff.new'a'))
assert ((wff.new'a'):is_immediate_sub (wff.parse'¬a'))

-- Formula, immediate subformulas.
local tests = {
   {'¬a', {'a'}},
   {'¬b', {'b'}},
   {'¬¬a', {'¬a'}},
   {'¬¬¬a', {'¬¬a'}},
   {'a∧a', {'a','a'}},
   {'¬a∧a', {'¬a', 'a'}},
   {'a∨b', {'a', 'b'}},
   {'a∨¬b', {'a', '¬b'}},
   {'a∨¬b∧c', {'a', '¬b∧c'}},
   {'(a→a)→a∨(b)', {'a→a', 'a∨b'}},
   {'(a→a)→a↔(b)', {'(a→a)→a', 'b'}},
}
for _,t in ipairs (tests) do
   local f = assert (wff.parse (t[1]))
   for _,s in ipairs (assert (t[2])) do
      local status, idx = assert (wff.parse (s)):is_immediate_sub (f)
      assert (status, ("'%s' is not an immediate sub of '%s'")
                 :format (s, t[1]))
   end
end
