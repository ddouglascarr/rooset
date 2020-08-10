local ctx_factories = {}
local model_factories = require 'test.model_factories'

function ctx_factories.build_base_ctx()
	ctx = {}
	ctx.members = {}
  ctx.members.determined_poitras = model_factories.build_member(
		'determined_poitras',
		'Determined Poitras')
  ctx.members.tender_hugle = model_factories.build_member('tender_hugle', 'Tender Hugle')

	ctx.policy = model_factories.build_policy(1, "default_policy", 10)
	ctx.unit = model_factories.build_unit("test_unit")
	ctx.area = model_factories.build_area(ctx.unit.id, "test_area", "test_ref")
	model_factories.build_allowed_policy(ctx.area.id, ctx.policy.id, true)

  return ctx
end

return ctx_factories
