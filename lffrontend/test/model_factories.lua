local model_factories = {}

TEST_INTERVAL = '1 hour'

-- quorum_num / 100
function model_factories.build_policy(index, name, quorum_num)
  p = Policy:new()
  p.index = index
  p.name = name

  p.initiative_quorum_den = 100
  p.initiative_quorum_num = quorum_num
  p.issue_quorum_den = 100
  p.issue_quorum_num = quorum_num

  p.min_admission_time = '0 seconds'
  p.max_admission_time = TEST_INTERVAL
  p.discussion_time = TEST_INTERVAL
  p.verification_time = TEST_INTERVAL
  p.voting_time = TEST_INTERVAL
  p:save()
  return p
end

function model_factories.build_unit(name)
  u = Unit:new()
  u.name = name
  u:save()
  return u
end

function model_factories.build_area(unit_id, name, external_reference)
  a = Area:new()
  a.unit_id = unit_id
  a.name = name
  a.external_reference = external_reference
  a:save()
  return a
end

function model_factories.build_allowed_policy(area_id, policy_id, is_default_policy)
  ap = AllowedPolicy:new()
  ap.area_id = area_id
  ap.policy_id = policy_id
  ap.default_policy = is_default_policy
  ap:save()
  return ap
end

function model_factories.build_member(login, name)
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

function model_factories.build_privilege(unit_id, member_id)
  p = Privilege:new()
  p.unit_id = unit_id
  p.member_id = member_id
  p.voting_right = true
  p.initiative_right = true
  p:save()
  return p
end

function model_factories.build_membership(area_id, member_id)
  m = Membership:new()
  m.area_id = area_id
  m.member_id = member_id
  m:save()
  return m
end

function model_factories.build_interest(issue_id, member_id)
  i = Interest:new()
  i.issue_id = issue_id
  i.member_id = member_id
  i:save()
  return i
end



return model_factories
