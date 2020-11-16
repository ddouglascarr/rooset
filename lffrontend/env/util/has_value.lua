function util.has_value(tbl, val) 
	for k, v in pairs(tbl) do
		if v == val then
			return true
	  end
	end

	return false
end

