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
assert (not wff.parse'')
assert (not wff.parse'1')
assert (not wff.parse'1 1')
assert (not wff.parse'a 1')
assert (not wff.parse'()a 1')
assert (not wff.parse'()')
assert (not wff.parse'not a ()')
assert (not wff.parse')not a(')
assert (not wff.parse'not-a')
assert (not wff.parse'and a')

-- String, tag/is_*, symbol.
local tests = {
   -----------------------------------------------
   -- atoms
   {'a',         'atom', 'a'},
   {'b',         'atom', 'b'},
   {'_',         'atom', '_'},
   {'test_33',   'atom', 'test_33'},
   {'(a)',       'atom', 'a'},
   {'((a ))',    'atom', 'a'},
   {'(((a)))',   'atom', 'a'},
   {'(( (b) ))', 'atom', 'b'},
   {'  xxx ',    'atom', 'xxx'},
   -----------------------------------------------
   -- not
   {'not a',     'not',  'not'},
   {'¬a',        'not',  '¬'},
   {'\\lnota',   'not',  '\\lnot'},
   {'\\lnot a',  'not',  '\\lnot'},
   {'\\nega',    'not',  '\\neg'},
   {'\\neg a',   'not',  '\\neg'},
   -----------------------------------------------
   -- and
   {'a and a',   'and',  'and'},
   {'a∧a',       'and',  '∧'},
   {'a\\landa',  'and',  '\\land'},
   {'a\\land a', 'and',  '\\land'},
   {'a\\wedgea', 'and',  '\\wedge'},
   {'a\\wedge a', 'and', '\\wedge'},
   -----------------------------------------------
   -- or
   {'a or a',    'or',   'or'},
   {'a∨a',       'or',   '∨'},
   {'a\\lora',   'or',   '\\lor'},
   {'a\\lor a',  'or',   '\\lor'},
   {'a\\veea',   'or',   '\\vee'},
   {'a\\vee a',  'or',   '\\vee'},
   -----------------------------------------------
   -- imp
   {'a imp a',   'imp',  'imp'},
   {'a→a',       'imp',  '→'},
   {'a\\toa',    'imp',   '\\to'},
   {'a\\to a',   'imp',   '\\to'},
   {'a\\rightarrowa', 'imp', '\\rightarrow'},
   {'a\\rightarrow a', 'imp', '\\rightarrow'},
   -----------------------------------------------
   -- iff
   {'a iff a',   'iff',  'iff'},
   {'a↔a',       'iff',  '↔'},
   {'a\\iffa',   'iff',   '\\iff'},
   {'a\\iff a',  'iff',   '\\iff'},
   {'a\\leftrightarrowa', 'iff', '\\leftrightarrow'},
   {'a\\leftrightarrow a', 'iff', '\\leftrightarrow'},
   -----------------------------------------------
   -- misc
   {'¬¬¬a',      'not',  '¬'},
   {'¬a→b',      'imp',  '→'},
   {'¬a∨¬b',     'or',   '∨'},
   {'¬a↔b∧c',    'iff',  '↔'},
   {'¬(a→b∧c)',  'not',  '¬'},
}
for _,t in ipairs (tests) do
   local f = wff.parse (t[1])
   assert (f, ("failed to parse '%s'"):format (t[1]))
   assert (f:tag () == t[2]
              and wff[('is_%s'):format (f:tag ())] (f) == true,
           ("bad tag for '%s' (expected '%s', got '%s')")
              :format (t[1], t[2], f:tag ()))
   assert (f:sym () == t[3],
           ("bad symbol for '%s' (expected '%s', got '%s')")
              :format (t[1], t[3], f:sym ()))
end
