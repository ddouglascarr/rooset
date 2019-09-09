function locale._get_translation_table()
  local language_code = locale.get("lang")
  if language_code then
    if type(language_code) ~= "string" then
      error('locale.get("lang") does not return a string.')
    end
    local translation_table = locale._translation_tables[language_code]
    if translation_table then 
      return translation_table
    end
    local filename = encode.file_path(WEBMCP_BASE_PATH, "locale", "translations." .. language_code .. ".lua")
    local func, load_errmsg = loadcached(filename)
    if not func then
      error('Could not load translation file: ' .. load_errmsg)
    end
    translation_table = func()
    if type(translation_table) ~= "table" then
      error("Translation file did not return a table.")
    end
    locale._translation_tables[language_code] = translation_table
    return translation_table
  else
    return locale._empty_translation_table
  end
end
