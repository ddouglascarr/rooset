-- needed to get new_params_list into global scope
require('moonbridge_http')

local testing = {}

function testing.it(description, fn)
	if _G.rooset_tests == nil then
		_G.rooset_tests = {}
	end

	_G.rooset_tests[#_G.rooset_tests+1] = {
		description=description,
		fn=fn,
	}
end

function do_test(fn)
  assert(
		os.execute(
			'dropdb --if-exists rooset_test && createdb -T rooset_template rooset_test'),
		"could not create database")

  execute.finalizers()
  execute.postfork_initializers()

  fn()

  execute.finalizers()
end

function testing.run_tests()
  assert(
		config.database.dbname == 'rooset_test',
		'database must be called rooset_test')

	for _, test in ipairs(_G.rooset_tests) do
		io.write(test.description .. ' .. ')
		do_test(test.fn)
		io.write('OK\n')
	end
end

function testing.initialize_request()
  execute.finalizers()
  execute.postfork_initializers()
	request.initialize()

	-- mock all the things needed in request
	request._http_request = {
	}
	request._http_request.get_params_list, request._http_request.get_params = new_params_list()
	request._http_request.post_params_list, request._http_request.post_params = new_params_list()
	app.html_title = {}
	request._relative_baseurl = '/'
end

function testing.initialize_session(member_id)
	app.session = Session:new()
	app.session.member_id = member_id
	app.session:save()
end

function testing.do_view(args)
	local params = args.params
	local member_id = args.member_id
	local module = args.module
	local view = args.view
	local id = args.id

	testing.initialize_request()

	if member_id then
		testing.initialize_session(member_id)
	end

	request._http_request.path = string.sub(
		encode.url({ module = module,	view = view, id = id }),
		2)

	if params then
		for k, v in pairs(params) do
		  request._http_request.get_params_list[k] = v
			request._http_request.post_params_list[k] = v
		end
	end
  request._route = request.router()
	
	execute.view{
			module = request.get_module(),
			view = request.get_view(),
	}
end

function testing.do_action(args)
	local id = args.id
	local params = args.params or {}
	local member_id = args.member_id
	local module = args.module
	local action = args.action

	testing.initialize_request()

	if member_id then
		testing.initialize_session(member_id)
	end

	-- get path, and strip leading /
	request._http_request.path = string.sub(
		encode.url{	module = module, action = action },
		2)

	if params then
		for k, v in pairs(params) do
			if type(v) ~= 'table' then
				v = { tostring(v) }
			end
		  request._http_request.get_params_list[k] = v
			request._http_request.post_params_list[k] = v
		end
	end
  request._route = request.router()

	-- this is a weird thing that happens with action forms. They get the model id for the
	-- module in the form under _webmcp_id somehow, and that id gets added to the route in
	-- handler.lua.
  do
		if id then
      request._http_request.post_params["_webmcp_id"] = tostring(id)
		end
    local post_id = request._http_request.post_params["_webmcp_id"]
    if post_id then
      request._route.id = post_id
    end
  end

	db:query('BEGIN')

	action_status = execute.action{
			module = request.get_module(),
			action = request.get_action(),
	}

	db:query('COMMIT')

	return action_status
end

function testing.do_time_warp(interval)
	db:query('BEGIN')
	db:query('SET TRANSACTION ISOLATION LEVEL REPEATABLE READ')
	db:query{'SELECT time_warp(?)', interval}
	db:query('COMMIT')
end

function testing.do_lf_update()
  assert(
		os.execute(
			'/opt/rooset/lfcore/lf_update dbname=${PGDATABASE}'),
		"could not run lf_update")
end


function testing.eq(a, b, message)
	message = message or ''
	assert(b ~= nil, 'the comparitor is nil, use testing.assert_is_nil')
	assert(a ~= nil, message .. ' : is nil')
	assert(a == b, message .. ' : ' .. a .. ' != ' .. b)
end

function testing.assert_is_nil(a, message)
	message = message or 'is not nil'
	assert(a == nil, message .. ' : ' .. a .. ' != nil')
end

return testing 
