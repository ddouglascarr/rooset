-- open and set default database handle
_G.db = assert(mondelefant.connect(config.database))

function mondelefant.class_prototype:get_db_conn() return db end

execute.inner()

-- close the database
db:close()
