local t = require 'test.testing'

t.it('has_value should return true if value is in table', function()
	local test_tbl = {
		foo='bar'
	}
	local positive_result = util.has_value(test_tbl, 'bar')
	assert (positive_result == true)

	local negative_result = util.has_value(test_tbl, 'foo')
	assert (negative_result == false)
end)
