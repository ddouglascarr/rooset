local model_factories = require 'test.model_factories'
local BaseCtx = require 'test.base_ctx'
local t = require 'test.testing'

-- TODO: use a table test for the actual voting. So many possibilites

t.it('show area', function()
  local ctx = BaseCtx:new()

  t.do_view{
    module = 'area',
    view = 'show',
    id = ctx.area.id,
    member_id = ctx.member_determined_poitras.id,
  }

  t.eq(request.data.area_id, ctx.area.id,
    'payload should have correct id')

  t.eq(request.data.area_name, ctx.area.name,
    'payload should have correct name')

  t.eq(request.data.area_current_external_reference, ctx.area.current_external_reference)

  html = t.HTML:new()
  html:assert_rtv('area-name', ctx.area.name)
  html:assert_rtv('area-id', ctx.area.id)
  html:assert_rtv('area-current_external_reference', 'test_ref')
end)

t.it('show issue', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()

  t.do_view{
    module = 'issue',
    view = 'show',
    id = ctx.issue_0.id,
    member_id = ctx.member_determined_poitras.id,
  }
  t.eq(request.data.issue_state, 'admission')
  t.eq(request.data.issue_id, ctx.issue_0.id)
  t.eq(request.data.area_id, ctx.area.id)
  t.eq(request.data.policy_id, ctx.policy.id)

  local html = t.HTML:new()
  html:assert_rtv('issue-id', ctx.issue_0.id)
  html:assert_rtv('issue-state', 'admission')
end)

t.it('create initiative', function ()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()

  rd = request.get_redirect_data()
  t.eq(rd.module, 'initiative', 'should redirect to initiative')
  t.eq(rd.view, 'show', 'should redirect to initiative')

  t.do_view{
    module = rd.module,
    view = rd.view,
    id = rd.id,
    member_id = ctx.member_determined_poitras.id,
  }
  t.eq(request.data.initiative_id, ctx.initiative_0.id, 'redirects to new initiative')
  t.eq(request.data.issue_state, 'admission', 'issue is in admission state')

  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'admission', 'issue is in admission state')
  html:assert_rtv('initiative-id', ctx.initiative_0.id, 'redirects to a new initiative')
  html:assert_rtv('initiative-current_draft-external_reference', 'new_sha',
    'includes external rerence from token')
  html:assert_rtk_content('initiative-id', ctx.initiative_0.display_name,
    'displays initiative name in heading')
end)

t.it('issue should be canceled if initiative quorum not reached', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()
  local initiative_id = ctx.initiative_0.id

  ctx:time_warp_one_period()
  t.do_lf_update()
  
  t.do_view{
    module = 'initiative',
    view = 'show',
    id = initiative_id,
    member_id = ctx.member_determined_poitras.id,
  }

  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'canceled_issue_not_accepted',
    'issue is in canceled state')
end)

t.it('issue should enter discussion if quorum reached through explicit support', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()
  ctx:support_initiative{
    member_id = ctx.member_tender_hugle.id,
    initiative_id = ctx.initiative_0.id,
  }

  t.do_view{
    module = 'initiative',
    view = 'show',
    id = ctx.initiative_0.id,
    member_id = ctx.member_determined_poitras.id,
  }

  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'discussion',
    'issue should be in discussion state')
end)

t.it('issue should enter discussion if quorum reached through area delegation', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()

  t.do_action{
    module = 'delegation',
    action = 'update',
    member_id=ctx.member_tender_hugle.id,
    params = {
      trustee_id = ctx.member_determined_poitras.id,
      area_id = ctx.area.id,
    }
  }
  t.do_lf_update()

  t.do_view{
    module = 'initiative',
    view = 'show',
    id = ctx.initiative_0.id,
    member_id = ctx.member_determined_poitras.id,
  }

  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'discussion',
    'issue should be in discussion state')
end)

t.it('issue should enter discussion if quorum reached through unit delegation', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()
  local initiative_id = ctx.initiative_0.id

  t.do_action{
    module = 'delegation',
    action = 'update',
    member_id=ctx.member_tender_hugle.id,
    params = {
      trustee_id = { tostring(ctx.member_determined_poitras.id) },
      unit_id = { tostring(ctx.unit.id) },
    }
  }
  t.do_lf_update()

  t.do_view{
    module = 'initiative',
    view = 'show',
    id = ctx.initiative_0.id,
    member_id = ctx.member_determined_poitras.id,
  }

  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'discussion',
    'issue should be in discussion state')
end)

t.it('issue should enter discussion if quorum reached through issue delegation', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()

  t.do_action{
    module = 'delegation',
    action = 'update',
    member_id=ctx.member_tender_hugle.id,
    params = {
      trustee_id = ctx.member_determined_poitras.id,
      issue_id = ctx.issue_0.id,
    }
  }
  t.do_lf_update()

  t.do_view{
    module = 'initiative',
    view = 'show',
    id = ctx.initiative_0.id,
    member_id = ctx.member_determined_poitras.id,
  }

  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'discussion',
    'issue should be in discussion state')
end)

t.it('initiatives cannot be created when sections conflict with admitted initiative', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()
  ctx:support_initiative{
    member_id = ctx.member_tender_hugle.id,
    initiative_id = ctx.initiative_0.id,
  }

  -- try to submit an initiative that modifies a section under discussion
  local action_status = ctx:create_initiative_conflicting_with_0()
  t.eq(action_status, 'error', 'action should fail')
end)

t.it('conflicting section admitted, other initiatives cancelled', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()

  local create_status = ctx:create_initiative_conflicting_with_0()
  t.eq(create_status, 'ok', 'initiative should be created')

  -- add support to initiative_0, to get admission quorum
  ctx:support_initiative{
    member_id = ctx.member_tender_hugle.id,
    initiative_id = ctx.initiative_0.id,
  }
  t.do_lf_update()
  t.do_lf_update()  -- needs to be run twice to react to the other issue being admitted

  t.do_view{
    module = 'initiative',
    view = 'show',
    id = ctx.initiative_0.id,
    member_id = ctx.member_determined_poitras.id,
  }
  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'discussion',
    'issue that reached quorum should be admitted')

  t.do_view{
    module = 'initiative',
    view = 'show',
    id = ctx.initiative_conflicting_with_0.id,
    member_id = ctx.member_determined_poitras.id,
  }
  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'canceled_conflicting_section_admitted',
    'initiative that did not reach quorum should be cancelled')
end)

t.it('create competing initiative', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()
  ctx:support_initiative{
    member_id = ctx.member_tender_hugle.id,
    initiative_id = ctx.initiative_0.id,
  }
  local action_status = ctx:create_initiative_1_competes_with_0()
  t.eq(action_status, 'ok', 'should be able to create competing initiative')
  
  t.do_view{
    module = 'initiative',
    view = 'show',
    id = ctx.initiative_1.id,
    member_id = ctx.member_determined_poitras.id,
  }

  local html = t.HTML:new()
  html:assert_rtv('issue-state', 'discussion')
  html:assert_rtv('initiative-id', ctx.initiative_1.id)
  html:assert_rtv('initiative-current_draft-external_reference', 'new_sha_1',
    'includes external rerence from token')
end)  

t.it('competing intiiatives admitted to voting if they reach initiative quorum', function()
  local ctx = BaseCtx:new()
  ctx:create_initiative_0()
  ctx:support_initiative{
    member_id = ctx.member_tender_hugle.id,
    initiative_id = ctx.initiative_0.id,
  }
  ctx:create_initiative_1_competes_with_0()
  ctx:create_initiative_2_competes_with_0()

  ctx:support_initiative{
    member_id = ctx.member_dreamy_almeida.id,
    initiative_id = ctx.initiative_2.id,
  }
  ctx:time_warp_one_period()  -- verification
  ctx:time_warp_one_period()  -- voting
  
  ctx:assert_initiative_admitted_to_voting{
    initiative_id=ctx.initiative_0.id,
    is_admitted = true,
  }
  ctx:assert_initiative_admitted_to_voting{
    initiative_id=ctx.initiative_1.id,
    is_admitted = false,
  }
  ctx:assert_initiative_admitted_to_voting{
    initiative_id=ctx.initiative_2.id,
    is_admitted = true,
  }
end)

t.it('members can submit a vote', function()
  local ctx = BaseCtx:new()
  ctx:advance_2_initiatives_to_voting_stage()

  t.do_view{
    module = 'vote',
    view = 'list',
    params = { issue_id = { tostring(ctx.issue_0.id) }},
    member_id = ctx.member_determined_poitras.id,
  }

  -- before voting, everything is graded 0.
  local html = t.HTML:new()
  html:assert_rtv('voting-grade:initiative:' .. ctx.initiative_0.id, 0)
  html:assert_rtv('voting-grade:initiative:' .. ctx.initiative_1.id, 0)

  t.do_action{
    module = 'vote',
    action = 'update',
    params = {
      issue_id = { tostring(ctx.issue_0.id) },
      scoring = { ctx.initiative_1.id .. ':1;' .. ctx.initiative_0.id .. ':-1;' },
    },
    member_id = ctx.member_determined_poitras.id,
  }
  
  t.do_view{
    module = 'vote',
    view = 'list',
    params = { issue_id = { tostring(ctx.issue_0.id) } },
    member_id = ctx.member_determined_poitras.id,
  }

  -- after voting, grades are offsets from abstention.
  local html = t.HTML:new()
  html:assert_rtv('voting-grade:initiative:' .. ctx.initiative_0.id, '-1')
  html:assert_rtv('voting-grade:initiative:' .. ctx.initiative_1.id, '1')
end)

t.it('winning initiative should be adopted', function()
  local ctx = BaseCtx:new()
  ctx:advance_2_initiatives_to_voting_stage()
  
  -- 2 people vote for 1, sq, 0
  t.do_action{
    module = 'vote',
    action = 'update',
    params = {
      issue_id = { tostring(ctx.issue_0.id) },
      scoring = { ctx.initiative_1.id .. ':1;' .. ctx.initiative_0.id .. ':-1;' },
    },
    member_id = ctx.member_determined_poitras.id,
  }
  t.do_action{
    module = 'vote',
    action = 'update',
    params = {
      issue_id = { tostring(ctx.issue_0.id) },
      scoring = { ctx.initiative_1.id .. ':1;' .. ctx.initiative_0.id .. ':-1;' },
    },
    member_id = ctx.member_goofy_heisenberg.id,
  }

  -- 1 person votes sq, 1, 0
  t.do_action{
    module = 'vote',
    action = 'update',
    params = {
      issue_id = { tostring(ctx.issue_0.id) },
      scoring = { ctx.initiative_1.id .. ':-1;' .. ctx.initiative_0.id .. ':-2;' },
    },
    member_id = ctx.member_tender_hugle.id,
  }

  ctx:time_warp_one_period()  -- finish voting

  t.do_view{
    module = 'issue',
    view = 'show',
    id = ctx.issue_0.id,
    member_id = ctx.member_determined_poitras.id,
  }
  local html = t.HTML:new()
  html:assert_rtv('issue-id', ctx.issue_0.id)
  html:assert_rtv('issue-state', 'finished_with_winner')
  html:assert_rtv('initiative-rank:1', ctx.initiative_1.id,
    'correct initiative is the winner')
    
  t.do_view{
    module = 'area',
    view = 'show',
    id = ctx.area.id,
    member_id = ctx.member_determined_poitras.id,
  }
  html = t.HTML:new()
  html:assert_rtv('area-current_external_reference', 'new_sha_1',
    'area current_external_reference updated to that of the winning initiative')
  
end)

t.it('status quo should be able to be maintained', function()
  local ctx = BaseCtx:new()
  ctx:advance_2_initiatives_to_voting_stage()
  
  -- 2 people vote for sq, 1, 0
  t.do_action{
    module = 'vote',
    action = 'update',
    params = {
      issue_id = { tostring(ctx.issue_0.id) },
      scoring = { ctx.initiative_1.id .. ':-1;' .. ctx.initiative_0.id .. ':-22;' },
    },
    member_id = ctx.member_determined_poitras.id,
  }
  t.do_action{
    module = 'vote',
    action = 'update',
    params = {
      issue_id = { tostring(ctx.issue_0.id) },
      scoring = { ctx.initiative_1.id .. ':-1;' .. ctx.initiative_0.id .. ':-2;' },
    },
    member_id = ctx.member_goofy_heisenberg.id,
  }

  -- 1 person votes 1, sq, 0
  t.do_action{
    module = 'vote',
    action = 'update',
    params = {
      issue_id = { tostring(ctx.issue_0.id) },
      scoring = { ctx.initiative_1.id .. ':1;' .. ctx.initiative_0.id .. ':-1;' },
    },
    member_id = ctx.member_tender_hugle.id,
  }
  ctx:time_warp_one_period()  -- finish voting

  t.do_view{
    module = 'issue',
    view = 'show',
    id = ctx.issue_0.id,
    member_id = ctx.member_determined_poitras.id,
  }
  local html = t.HTML:new()
  html:assert_rtv('issue-id', ctx.issue_0.id)
  html:assert_rtv('issue-state', 'finished_without_winner')
  html:assert_rtv('initiative-rank:1', ctx.initiative_1.id,
    'correct initiative is still ranked 1 even though there is no winner')
    
  t.do_view{
    module = 'area',
    view = 'show',
    id = ctx.area.id,
    member_id = ctx.member_determined_poitras.id,
  }
  html = t.HTML:new()
  html:assert_rtv('area-current_external_reference', 'test_ref',
    'area current_external_reference remains as before')
  
end)
