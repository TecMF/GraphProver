--[[ graphprover.wff -- Well-formed formulas.
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
along with GraphProver.  If not, see <http://www.gnu.org/licenses/>.  ]]--

local assert = assert
local error = error
local ipairs = ipairs
local lpeg = require'lpeg'
local pairs = pairs
local setmetatable = setmetatable
local table = table
local type = type

local wff = {}
_ENV = nil

---
-- Propositional logic formula.
-- @classmod wff
---
do
   wff.__index = wff
   wff.__metatable = 'not your business'
end

-- Grammar.
local Space   = (lpeg.S ' \t\n')^0
local Open    = '(' * Space
local Close   = ')' * Space

local Digit   = lpeg.R'09'
local Lower   = lpeg.R'az'
local Upper   = lpeg.R'AZ'
local Alpha   = Lower + Upper
local Alnum   = Alpha + Digit
local Id      = (lpeg.P'_' + Alpha) * (Alnum + '_')^0

local Form    = lpeg.V'Form'
local FormIff = lpeg.V'FormIff'
local FormImp = lpeg.V'FormImp'
local FormOr  = lpeg.V'FormOr'
local FormAnd = lpeg.V'FormAnd'
local FormNot = lpeg.V'FormNot'
local Atom    = lpeg.V'Atom'

local Conn = {
   ['iff'] = {utf8={'↔'}, latex={'\\iff', '\\leftrightarrow'}},
   ['imp'] = {utf8={'→'}, latex={'\\to', '\\rightarrow'}},
   ['or']  = {utf8={'∨'}, latex={'\\lor', '\\vee'}},
   ['and'] = {utf8={'∧'}, latex={'\\land', '\\wedge'}},
   ['not'] = {utf8={'¬'}, latex={'\\lnot', '\\neg'}},
}
do
   for k,v in pairs (Conn) do
      local pat = lpeg.P (k)
      for _,t in ipairs {v.utf8, v.latex} do
         for _,s in ipairs (t) do
            pat = pat + lpeg.P (s)
         end
      end
      Conn[k].pat = lpeg.C (pat) * Space
   end
end
local ConnIff = Conn.iff.pat
local ConnImp = Conn.imp.pat
local ConnOr  = Conn['or'].pat
local ConnAnd = Conn['and'].pat
local ConnNot = Conn['not'].pat

-- Returns a function that wraps pattern captures into an object.
local function wrap (tag, style)
   if style == nil or style == 'prefix' then -- prefix operator
      return function (sym, ...)
         local t = {_tag=tag, _sym=sym, ...}
         return setmetatable (t, wff)
      end
   elseif style == 'infix' then -- right-associative, infix operator
      local func
      func = function (x, sym, ...)
         if sym == nil then
            return assert (x)   -- nothing to do
         end
         assert (x and sym)
         local t = {_tag=tag, _sym=sym, x}
         setmetatable (t, wff)
         table.insert (t, func (...))
         return t
      end
      return func
   else
      error'should not get here'
   end
end

local Grammar = {
   Form,
   Form    = FormIff,
   FormIff = FormImp * (ConnIff * FormImp)^0
      / wrap ('iff', 'infix'),
   FormImp  = FormOr * (ConnImp * FormOr)^0
      / wrap ('imp', 'infix'),
   FormOr  = FormAnd * (ConnOr * FormAnd)^0
      / wrap ('or', 'infix'),
   FormAnd = FormNot * (ConnAnd * FormNot)^0
      / wrap ('and', 'infix'),
   FormNot = ConnNot * FormNot / wrap ('not', 'prefix')
      + Atom + Open * Form * Close,
   Atom    = Id / wrap ('atom') * Space,
}


---
-- Creates a new atomic formula.
-- @param sym formula symbol.
-- @return[1] the resulting atomic formula.
-- @return[2] `nil` plus error message.
---
function wff:new (sym)
   local sym = sym or self
   assert (sym and type (sym) == 'string')
   if not lpeg.match (Id * -1, sym) then
      return nil, 'bad symbol'
   end
   return setmetatable ({_tag='atom', _sym=sym}, wff)
end

---
-- Parses formula string.
-- @param s formula string.
-- @return[1] the resulting formula.
-- @return[2] `nil` plus error message.
---
function wff:parse (s)
   local s = s or self
   assert (s and type (s) == 'string')
   local f = lpeg.match (Space * Grammar * -1, s)
   if f == nil then
      return nil, 'syntax error'
   end
   return f
end

---
-- Gets formula tag.
-- @return the formula tag.
---
function wff:tag ()
   return self._tag
end

---
-- Gets formula symbol.
-- @return the formula symbol.
---
function wff:sym ()
   return self._sym
end

---
-- Tests whether formula is an atom.
-- @return[1] `true` if formula is an atom.
-- @return[2] `false` otherwise.
---
function wff:is_atom ()
   return self:tag () == 'atom'
end

---
-- Tests whether formula is a negation.
-- @return[1] `true` if formula is a negation.
-- @return[2] `false` otherwise.
---
function wff:is_not ()
   return self:tag () == 'not'
end

---
-- Tests whether formula is a conjunction.
-- @return[1] `true`, if formula is a conjunction.
-- @return[2] `false`, otherwise.
---
function wff:is_and ()
   return self:tag () == 'and'
end

---
-- Tests whether formula is a disjunction.
-- @return[1] `true` if formula is a disjunction.
-- @return[2] `false` otherwise.
---
function wff:is_or ()
   return self:tag () == 'or'
end

---
-- Tests whether formula is an implication.
-- @return[1] `true` if formula is a implication.
-- @return[2] `false` otherwise.
---
function wff:is_imp ()
   return self:tag () == 'imp'
end

---
-- Tests whether formula is a bi-implication.
-- @return[1] `true` if formula is a bi-implication.
-- @return[2] `false` otherwise.
---
function wff:is_iff ()
   return self:tag () == 'iff'
end

---
-- Tests whether formula is equal to another.
-- @param other formula.
-- @return[1] `true` if successful.
-- @return[2] `false` otherwise.
---
function wff:__eq (other)
   if self._tag ~= other._tag or #self ~= #other then
      return false
   end
   if self._sym ~= other._sym then
      return false
   end
   for i=1,#self do
      if self[i] ~= other[i] then
         return false
      end
   end
   return true
end

---
-- Tests whether formula is an immediate formula of another.
-- @param other formula.
-- @return[1] `true` if successful.
-- @return[2] `false` otherwise.
---
function wff:is_immediate_sub (other)
   for i=1,#other do
      if self == other[i] then
         return true, i
      end
   end
   return false
end

---
-- Tests whether formula is a proper formula of another.
-- @param other formula.
-- @return[1] `true` if successful.
-- @return[2] `false` otherwise.
---
function wff:is_proper_sub (other)
   if #other == 0 then
      return false
   end
   if self:is_immediate_sub (other) then
      return true
   end
   for i=1,#other do
      if self:is_proper_sub (other[i]) then
         return true
      end
   end
   return false
end

---
-- Gets immediate subformulas.
-- @return the immediate subformulas.
---
function wff:unpack ()
   if not self:is_formula () then
      return nil                -- nothing to do
   end
   return table.unpack (self)
end

return wff
