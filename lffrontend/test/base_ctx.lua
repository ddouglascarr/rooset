local ctx_factories = {}
local model_factories = require 'test.model_factories'
local t = require 'test.testing'

local BaseCtx = {}

function BaseCtx:new()
  local o = {}
  setmetatable(o, self)
  self.__index = self
  o:_init()
  return o
end

function BaseCtx:_init()
  self.member_determined_poitras = model_factories.build_member(
    'determined_poitras',
    'Determined Poitras')
  self.member_tender_hugle = model_factories.build_member('tender_hugle', 'Tender Hugle')
  self.member_dreamy_almeida= model_factories.build_member(
    'dreamy_almeida',
    'Dreamy Almeida')
  self.member_thirsty_swirles = model_factories.build_member(
    'thirsty_swirles',
    'Thirsty Swirles')
  self.member_goofy_heisenberg = model_factories.build_member(
    'goofy_heisenberg',
    'Goofy Heisenberg')

  self.policy = model_factories.build_policy(1, "default_policy", 30)
  self.unit = model_factories.build_unit("test_unit")
  self.area = model_factories.build_area(self.unit.id, "test_area", "test_ref")
  model_factories.build_allowed_policy(self.area.id, self.policy.id, true)

  model_factories.build_privilege(self.unit.id, self.member_determined_poitras.id)
  model_factories.build_privilege(self.unit.id, self.member_tender_hugle.id)
  model_factories.build_privilege(self.unit.id, self.member_dreamy_almeida.id)
  model_factories.build_privilege(self.unit.id, self.member_thirsty_swirles.id)
  model_factories.build_privilege(self.unit.id, self.member_goofy_heisenberg.id)

  model_factories.build_membership(self.area.id, self.member_determined_poitras.id)
  model_factories.build_membership(self.area.id, self.member_tender_hugle.id)
  model_factories.build_membership(self.area.id, self.member_dreamy_almeida.id)
  model_factories.build_membership(self.area.id, self.member_thirsty_swirles.id)
  model_factories.build_membership(self.area.id, self.member_goofy_heisenberg.id)
end

function BaseCtx:create_initiative_0()
  local action_status = t.do_action{
    module = 'initiative',
    action = 'create',
    member_id = self.member_determined_poitras.id,
    params = {
      area_id = self.area.id,
      tk = jwt.encode({
          BaseSHA = self.area.external_reference,
          SHA = 'new_sha',
          ModifiedSectionIDs = { '123' },
      }),
      policy_id = self.policy.id,
      name = 'my initiative',
      draft = 'foo bar',
    },
  }

  t.do_lf_update()

  local initiative_id = request._redirect.id
  self.initiative_0 = Initiative:by_id(initiative_id)
  self.issue_0 = self.initiative_0.issue
  return action_status
end

function BaseCtx:create_initiative_1_competes_with_0()
  local action_status = t.do_action{
    module = 'initiative',
    action = 'create',
    member_id = self.member_goofy_heisenberg.id,
    params = {
      area_id = self.area.id,
      issue_id = self.issue_0.id,
      tk = jwt.encode({
          BaseSHA = self.area.external_reference,
          SHA = 'new_sha_1',
          ModifiedSectionIDs = { '123' },
      }),
      policy_id = self.policy.id,
      name = 'my initiative 1',
      draft = 'foo bar',
    },
  }

  t.do_lf_update()

  local initiative_id = request._redirect.id
  self.initiative_1 = Initiative:by_id(initiative_id)
  return action_status
end

function BaseCtx:create_initiative_2_competes_with_0()
  local action_status = t.do_action{
    module = 'initiative',
    action = 'create',
    member_id = self.member_thirsty_swirles.id,
    params = {
      area_id = self.area.id,
      issue_id = self.issue_0.id,
      tk = jwt.encode({
          BaseSHA = self.area.external_reference,
          SHA = 'new_sha_2',
          ModifiedSectionIDs = { '123' },
      }),
      policy_id = self.policy.id,
      name = 'my initiative 2',
      draft = 'foo bar',
    },
  }

  t.do_lf_update()

  local initiative_id = request._redirect.id
  self.initiative_2 = Initiative:by_id(initiative_id)
  return action_status
end

function BaseCtx:create_initiative_conflicting_with_0()
  local action_status = t.do_action{
    module = 'initiative',
    action = 'create',
    member_id = self.member_tender_hugle.id,
    params = {
      area_id = self.area.id,
      tk = jwt.encode({
          BaseSHA = self.area.external_reference,
          SHA = 'different_sha',
          ModifiedSectionIDs = { '123' },
      }),
      policy_id = self.policy.id,
      name = 'my conflicting initiative',
      draft = 'foo bar',
    },
  }
  t.do_lf_update()
  if action_status == 'error' then return action_status end

  local initiative_id = request._redirect.id
  self.initiative_conflicting_with_0 = Initiative:by_id(initiative_id)
  self.issue_conflicting_with_0 = self.initiative_0.issue
  return action_status
end

function BaseCtx:support_initiative(args)
  local member_id = args.member_id
  local initiative_id = args.initiative_id
  t.do_action{
    member_id = member_id,
    module = 'initiative',
    action = 'add_support',
    id = initiative_id,
  }
  t.do_lf_update()
end

function BaseCtx:assert_initiative_admitted_to_voting(args)
  local initiative_id = args.initiative_id
  local is_admitted = args.is_admitted
  
  t.do_view{
    module = 'initiative',
    view = 'show',
    id = initiative_id,
    member_id = self.member_determined_poitras.id,
  }
  t.eq(request.data.initiative_id, initiative_id)
  t.eq(request.data.issue_state, 'voting')

  if is_admitted then
    assert(request.data.initiative_admitted == true, 'initiative should be admitted')
  else
    assert(
      request.data.initiative_admitted == nil or request.data.initiative_admitted == false,
      'initiative should not be admitted')
  end
end

function BaseCtx:time_warp_one_period()
  t.do_time_warp('61 minutes')
  t.do_lf_update()
end

return BaseCtx
