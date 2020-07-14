function build_member(login, name)
  m = Member:new()
  m.login = login
  m.active = true
  m.activated = 'now'
  m.last_activity = 'now'
  m.name = name
  m:set_password('login')
  m:save()
  return m
end

function build_the_usual_suspect_members()
  members = {}

  members.determined_poitras = build_member('determined_poitras', 'Determined Poitras')
  members.tender_hugle = build_member('tender_hugle', 'Tender Hugle')

  return members
end

function build_ctx() 
  ctx = {}

  ctx.members = build_the_usual_suspect_members()

  return ctx
end

function test_test()
  assert(
    config.database.dbname == 'rooset_test',
    'database must be called rooset_test')

  assert(os.execute('dropdb --if-exists rooset_test && createdb -T rooset_template rooset_test'), "could not create database")

  execute.postfork_initializers()
  ctx = build_ctx()

  member_1 = Member:by_id(ctx.members.tender_hugle.id)
  assert(member_1, 'can retrieve a member by id')
  assert(member_1.name == ctx.members.tender_hugle.name, 'retrieves the correct member by id')

  execute.finalizers()
end

test_test()
