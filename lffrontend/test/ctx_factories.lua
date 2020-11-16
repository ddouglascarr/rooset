local ctx_factories = {}
local model_factories = require 'test.model_factories'

function ctx_factories.build_base_ctx()
	ctx = {}
	ctx.members = {}
  ctx.members.determined_poitras = model_factories.build_member(
		'determined_poitras',
		'Determined Poitras')
  ctx.members.tender_hugle = model_factories.build_member('tender_hugle', 'Tender Hugle')
  ctx.members.dreamy_almeida= model_factories.build_member(
		'dreamy_almeida',
		'Dreamy Almeida')
  ctx.members.thirsty_swirles = model_factories.build_member(
		'thirsty_swirles',
		'Thirsty Swirles')
  ctx.members.goofy_heisenberg = model_factories.build_member(
		'goofy_heisenberg',
		'Goofy Heisenberg')

	ctx.policy = model_factories.build_policy(1, "default_policy", 30)
	ctx.unit = model_factories.build_unit("test_unit")
	ctx.area = model_factories.build_area(ctx.unit.id, "test_area", "test_ref")
	model_factories.build_allowed_policy(ctx.area.id, ctx.policy.id, true)

	model_factories.build_privilege(ctx.unit.id, ctx.members.determined_poitras.id)
	model_factories.build_privilege(ctx.unit.id, ctx.members.tender_hugle.id)
	model_factories.build_privilege(ctx.unit.id, ctx.members.dreamy_almeida.id)
	model_factories.build_privilege(ctx.unit.id, ctx.members.thirsty_swirles.id)
	model_factories.build_privilege(ctx.unit.id, ctx.members.goofy_heisenberg.id)

	model_factories.build_membership(ctx.area.id, ctx.members.determined_poitras.id)
	model_factories.build_membership(ctx.area.id, ctx.members.tender_hugle.id)
	model_factories.build_membership(ctx.area.id, ctx.members.dreamy_almeida.id)
	model_factories.build_membership(ctx.area.id, ctx.members.thirsty_swirles.id)
	model_factories.build_membership(ctx.area.id, ctx.members.goofy_heisenberg.id)

  return ctx
end

return ctx_factories
