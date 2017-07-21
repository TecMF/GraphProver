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

local error = error
local io = io
local pairs = pairs
local table = table
local type = type

local tests = {}
_ENV = nil

local function cat (s)
   io.stdout:write (s)
end

local function dump (x, tab)
   if type (x) ~= 'table' then
      cat (tostring (x))
   else
      local tab = tab or 1
      cat ('{\n')
      for k,v in pairs (x) do
         cat (('   '):rep (tab))
         dump (k, tab + 1)
         cat ('=')
         dump (v, tab + 1)
         cat (',\n')
      end
      cat (('   '):rep (tab - 1)..'}')
   end
end

---
-- Dumps arguments to stdout.
---
function tests.dump (...)
   local args = {...}
   local n = tests.maxi (args) or 0
   for i=1,n do
      dump (args[i])
      cat ('\n')
   end
end

---
-- Returns the maximum integer index in the given table T.
-- If no index is found, returns nil.
---
function tests.maxi (t)
   local max = nil
   for k,_ in pairs (t) do
      if type (k) == 'number' and (max == nil or k > max) then
         max = k
      end
   end
   return max
end

---
-- Checks if table MOD contains the data specified in SIG.
-- Returns true if successful, otherwise throws an error.
---
function tests.ASSERT_CHECK_API (args)
   local mod = args[1]
   table.remove (args, 1)
   local sig = args
   if mod.__name ~= nil then
      sig.__name = type (mod.__name)
   end
   for k,v in pairs (mod) do
      if sig[k] == nil then
         error (("extra %s '%s'"):format (type (v), k), 2)
      end
      if type (v) ~= sig[k] then
         error (("bad type for '%s' (%s expected, got %s)")
                   :format (k, sig[k], type (v), 2))
      end
   end
   for k,v in pairs (sig) do
      if mod[k] == nil then
         error (("missing %s '%s'"):format (sig[k], k), 2)
      end
   end
   return true
end

return tests
