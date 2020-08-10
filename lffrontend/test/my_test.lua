local model_factories = require 'test.model_factories'
local ctx_factories = require 'test.ctx_factories'

function test_test()
  ctx = ctx_factories.build_base_ctx()
  member_1 = Member:by_id(ctx.members.tender_hugle.id)

  assert(member_1, 'can retrieve a member by id')
  assert(
		member_1.name == ctx.members.tender_hugle.name,
		'retrieves the correct member by id')
end

function test_unit_factory()
  ctx = ctx_factories.build_base_ctx()

	unit_0 = Unit:by_id(ctx.unit.id)
	assert(unit_0, 'can retrieve unit by id')
	assert(unit_0.name == 'test_unit', 'unit properties correctly set by test')
	assert(unit_0.name == ctx.unit.name, "retrieves unit properties by name")
end

function test_area_factory()
	ctx = ctx_factories.build_base_ctx()

	area_0 = Area:by_id(ctx.area.id)
	assert(area_0, 'can retrieve area by id')
	assert(area_0.name == 'test_area', 'area factory sets name')
	assert(area_0.id == ctx.area.id, 'retrieves correct area')
end

return { test_test, test_area_factory, test_unit_factory }
