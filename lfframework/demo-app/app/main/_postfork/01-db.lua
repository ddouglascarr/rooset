local db = assert(mondelefant.connect(config.db))
function mondelefant.class_prototype:get_db_conn() return db end
_G.db = db
execute.inner()
db:close()
