--[[--
text =                                          -- a string
format.string(
  value,                                           -- any value where tostring(value) gives a reasonable result
  {
    nil_as                = nil_text,              -- text to be returned for a nil value
    truncate_mode         = "codepoints",          -- performe truncating by counting UTF-8 codepoints ("codepoints") or Unicode grapheme clusters ("graphmeclusters")
                                                   -- (currently only "codepoints" are supported and this option may be omitted)
    truncate_at           = truncate_at,           -- truncate string after the given number of UTF-8 codepoints (or Unicode grapheme clusters)
    truncate_suffix       = truncate_suffix,       -- string to append, if string was truncated (use boolean true for Unicode ellipsis)
    truncate_count_suffix = truncate_count_suffix  -- if true, then the total length (including suffix) may not exceed the given length
  }
)

Formats a value as a text by calling tostring(...), unless the value is nil, in which case the text returned is chosen by the 'nil_as' option. Using the 'truncate_*' parameters, it is possible to show only the beginning of a string.

--]]--

local function codepoint_count(str)
  return #string.gsub(str, '[\128-\255][\128-\191]?[\128-\191]?[\128-\191]?', 'x')
end

local function codepoint_truncate(str, length)
  local byte_pos = 1
  local count = 0
  while count < length do
    b1, b2, b3, b4 = string.byte(str, byte_pos, byte_pos+3)
    if not b2 then
      break
    end
    b3 = b3 or 0
    b4 = b4 or 0
    if b1 >= 128 and b2 >= 128 and b2 <= 191 then
      if b3 >= 128 and b3 <= 191 then
        if b4 >= 128 and b4 <= 191 then
          byte_pos = byte_pos + 4
          count = count + 1
        elseif count + 1 < length and b4 < 128 then
          byte_pos = byte_pos + 4
          count = count + 2
        else
          byte_pos = byte_pos + 3
          count = count + 1
        end
      elseif count + 1 < length and b3 < 128 then
        if count + 2 < length and b4 < 128 then
          byte_pos = byte_pos + 4
          count = count + 3
        else
          byte_pos = byte_pos + 3
          count = count + 2
        end
      else
        byte_pos = byte_pos + 2
        count = count + 1
      end
    elseif count + 1 < length and b2 < 128 then
      if count + 2 < length and b3 < 128 then
        if count + 3 < length and b4 < 128 then
          byte_pos = byte_pos + 4
          count = count + 4
        else
          byte_pos = byte_pos + 3
          count = count + 3
        end
      else
        byte_pos = byte_pos + 2
        count = count + 2
      end
    else
      byte_pos = byte_pos + 1
      count = count + 1
    end
  end
  return string.sub(str, 1, byte_pos-1)
end

function format.string(str, options)
  local options = options or {}
  if str == nil then
    return options.nil_as or ""
  elseif options.truncate_at then
    str = tostring(str)
    -- TODO: Unicode grapheme cluster boundary detetion is not implemented
    -- (Unicode codepoints are used instead)
    local truncate_suffix = options.truncate_suffix
    if truncate_suffix == true then
      truncate_suffix = '\226\128\166'
    elseif not truncate_suffix then
      truncate_suffix = ''
    end
    if options.truncate_count_suffix and truncate_suffix then
      local suffix_length = codepoint_count(truncate_suffix)
      if codepoint_count(str) > options.truncate_at then
        return (
          codepoint_truncate(str, options.truncate_at - suffix_length) ..
          truncate_suffix
        )
      else
        return str
      end
    else
      if codepoint_count(str) > options.truncate_at then
        return codepoint_truncate(str, options.truncate_at) .. truncate_suffix
      else
        return str
      end
    end
  else
    return tostring(str)
  end
end
