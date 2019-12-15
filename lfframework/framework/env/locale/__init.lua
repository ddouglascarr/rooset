locale._translation_tables = {}
locale._empty_translation_table = {}

request.for_each(function()
  locale._current_data = {}
end)
