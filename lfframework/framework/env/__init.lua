--[[--
translated_string =             -- translated string
_(
  string_to_translate,          -- string to translate
  {
    placeholder_name1 = text1,  -- replace all occurrences of "#{placeholder_name1}" with the string text1
    placeholder_name2 = text2,  -- replace all occurrences of "#{placeholder_name2}" with the string text2
    ...
  }
)

Translation function for localization. The "_" function translates a given string to the currently selected language (see locale.set{...}). If the translated string contains placeholders in the form #{name}, then those placeholders may be automatically replaced with a corresponding substring which is taken from the table passed as optional second argument.

Hint: Lua's syntax rules allow to write _"text" as a shortcut for _("text"), or _'text' instead of _('text') respectivly.

--]]--

function _G._(text, replacements)
  local text = locale._get_translation_table()[text] or text
  if replacements then
    return (
      string.gsub(
        text,
        "#{(.-)}",
        function (placeholder)
          return replacements[placeholder]
        end
      )
    )
  else
    return text
  end
end
--//--

--[[--
cloned_table =  -- newly generated table
table.new(
  table_or_nil  -- keys of a given table will be copied to the new table
)

If a table is given, then a cloned table is returned.
If nil is given, then a new empty table is returned.

--]]--
function table.new(tbl)
  local new_tbl = {}
  if tbl then
    for key, value in pairs(tbl) do
      new_tbl[key] = value
    end
  end
  return new_tbl
end
--//--

-- load libraries
-- (except "extos", which has to be loaded earlier, and "multirand", which must be loaded after forking)
_G.nihil       = require 'nihil'
_G.mondelefant = require 'mondelefant'
_G.atom        = require 'atom'
_G.json        = require 'json'
require 'mondelefant_atom_connector'
-- NOTE: "multirand" library is loaded in mcp.lua after forking

-- setup mondelefant
mondelefant.connection_prototype.error_objects = true
function mondelefant.connection_prototype:sql_tracer(command)
  if trace.is_disabled() then
    return
  end
  local start_time = extos.monotonic_hires_time()
  return function(error_info)
    trace.sql{
      command        = command,
      execution_time = extos.monotonic_hires_time() - start_time,
      error_position = error_info and error_info.position or nil
    }
  end
end

--[[--
config  -- table to store application configuration

'config' is a global table, which can be modified by a config file of an application to modify the behaviour of that application.
--]]--
_G.config = {}
--//--

