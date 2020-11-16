local model_factories = require 'test.model_factories'
local ctx_factories = require 'test.ctx_factories'
local t = require 'test.testing'

t.it('test test', function()
  local ctx = ctx_factories.build_base_ctx()
  local member_1 = Member:by_id(ctx.members.tender_hugle.id)

  assert(member_1, 'can retrieve a member by id')
  assert(
		member_1.name == ctx.members.tender_hugle.name,
		'retrieves the correct member by id')
end)

t.it('unit factory should create a unit', function()
  local ctx = ctx_factories.build_base_ctx()

	local unit_0 = Unit:by_id(ctx.unit.id)
	assert(unit_0, 'can retrieve unit by id')
	assert(unit_0.name == 'test_unit', 'unit properties correctly set by test')
	assert(unit_0.name == ctx.unit.name, "retrieves unit properties by name")
end)

t.it('area factory should build an area', function() 
	local ctx = ctx_factories.build_base_ctx()

	local area_0 = Area:by_id(ctx.area.id)
	assert(area_0, 'can retrieve area by id')
	t.eq(area_0.name, 'test_area', 'area factory sets name')
	t.eq(area_0.id, ctx.area.id, 'retrieves correct area')
end)
