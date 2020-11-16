local model_factories = require 'test.model_factories'
local ctx_factories = require 'test.ctx_factories'
local t = require 'test.testing'

t.it('show area', function()
	local ctx = ctx_factories.build_base_ctx()

	t.do_view{
		path = 'area/show/' .. ctx.area.id .. '.html',
		member_id = ctx.members.determined_poitras.id,
	}

	t.eq(
		request.data.area_id, ctx.area.id,
		'payload should have correct id')

	t.eq(
		request.data.area_name, ctx.area.name,
		'payload should have correct name')
end)

function test_create_initiative()
	local ctx = ctx_factories.build_base_ctx()

	t.do_action{
		path = 'initiative/create',
		member_id = ctx.members.determined_poitras.id,
		params = {
			area_id = {tostring(ctx.area.id)},
			tk = {jwt.encode({
					BaseSHA = ctx.area.external_reference,
					SHA = 'new_sha',
					ModifiedSectionIDs = { '123' },
			})},
			policy_id = {tostring(ctx.policy.id)},
			name = {'my+initiative'},
			draft = {'foo bar'},
		},
	}

	local redirect_data = request.get_redirect_data()
	t.eq(request._redirect.module, 'initiative', 'should redirect to initiative')
	t.eq(request._redirect.view, 'show', 'should redirect to initiative')
	local initiative_id = request._redirect.id

	t.do_lf_update()

	t.do_view{
		path = string.sub(encode.url(redirect_data), 2),
		member_id = ctx.members.determined_poitras.id,
	}
	t.eq(request.data.initiative_id, initiative_id, 'redirects to new initiative')
	t.eq(request.data.issue_state, 'admission', 'issue is in admission state')

	return { ctx=ctx, initiative_id=initiative_id, issue_id=request.data.issue_id };
end

t.it('create initiative', test_create_initiative)

t.it('issue should be canceled if initiative quorum not reached', function()
	td = test_create_initiative()
	local ctx, initiative_id =  td.ctx, td.initiative_id

	t.do_time_warp('61 minutes')
	t.do_lf_update()
	
	t.do_view{
		path = 'initiative/show/' .. initiative_id .. '.html',
		member_id = ctx.members.determined_poitras.id,
	}

	t.eq(
		request.data.issue_state, 'canceled_issue_not_accepted',
		'issue is in canceled state')
end)

t.it('issue should enter discussion if quorum reached', function()
	td = test_create_initiative()
	local ctx, initiative_id, issue_id = td.ctx, td.initiative_id, td.issue_id

	t.do_action{
		path='initiative/add_support',
		member_id=ctx.members.tender_hugle.id,
		params={
			_webmcp_id={tostring(initiative_id)},
		}
	}

	t.do_lf_update()

	t.do_view{
		path = 'initiative/show/' .. initiative_id .. '.html',
		member_id = ctx.members.determined_poitras.id,
	}

	t.eq(
		request.data.issue_state, 'discussion',
		'issue should be in discussion state')
end)
