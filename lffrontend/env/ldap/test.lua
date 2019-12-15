-- Part of test case for ldap4lqfb

function ldap.test()

  local global_count = 0
  local global_failed = 0
  
  local function checkMember(uid, login, name, units_with_voting_priv, units_with_polling_priv)

    local failed = false
    
    local function testError(err, expected, got)
      failed = true
      global_failed = global_failed + 1
      if expected then
        print ("[" .. uid .. "] FAILED: " .. err .. " (expected: '" .. expected .. "' but got '" .. tostring(got) .. "')") 
      else
        print ("[" .. uid .. "] FAILED: " .. err)
      end
    end

    local function testOk(test, expected)
      if test then
        print ("[" .. uid .. "] " .. test .. " ok ('" .. expected .. "')")
      else
        print ("[" .. uid .. "] success")
      end
    end
    
    local function test(field, expected, value)
      global_count = global_count + 1
      if expected ~= value then
        testError(field, expected, value)
        return
      end
      testOk(field, expected)
    end
    
    local members = Member:new_selector()
      :add_field{ "authority_login" }
      :add_where{ "authority = ? AND authority_uid = ?", "ldap", uid }
      :exec()

    if #members < 1 then
      testError("Member not found in DB")
      return
    end
    
    if #members > 1 then
      testError("Found more than one DB entry")
      return
    end
    
    local member = members[1]
    
    if login == nil then
      if not member.locked then
        testError("Member not locked")
      else
        testOk("Member is locked", "true")
      end
      return
    end
      
    test("login", login, member.authority_login)
    test("name", name, member.name)
    
    for i, unit_id in ipairs(units_with_voting_priv) do
      global_count = global_count + 1
      local privilege = Privilege:by_pk(unit_id, member.id)
      if privilege and privilege.voting_right then
        testOk("voting_right", unit_id)
      else
        testError("voting_right", unit_id, "")
      end
    end
    
    local privileges_selector = Privilege:new_selector()
      :add_where{ "member_id = ?", member.id }
      :add_where("voting_right = true")
    if #units_with_voting_priv > 0 then
      local privileges = privileges_selector:add_where{ "unit_id NOT IN ($)", units_with_voting_priv }
    end
    local privileges = privileges_selector:exec()
      
    if #privileges > 0 then
      testError("voting_right", "count: 0", "count: " .. #privileges)
    else
      testOk("voting_right", "count: 0")
    end
    
    for i, unit_id in ipairs(units_with_polling_priv) do
      global_count = global_count + 1
      local privilege = Privilege:by_pk(unit_id, member.id)
      if privilege and privilege.polling_right then
        testOk("polling_right", unit_id)
      else
        testError("polling_right", unit_id, "")
      end
    end
    
    local privileges_selector = Privilege:new_selector()
      :add_where{ "member_id = ?", member.id }
      :add_where("polling_right = true")
    if #units_with_polling_priv > 0 then
      privileges_selector:add_where{ "unit_id NOT IN ($)", units_with_polling_priv }
    end
    local privileges = privileges_selector:exec()
      
    if #privileges > 0 then
      testError("polling_right", "count: " .. #units_with_polling_priv, "count: " .. #privileges)
    else
      testOk("polling_right", "count: " .. #units_with_polling_priv)
    end
    
    if not failed then
      return true
    end
    
    return false
    
  end
  
  checkMember("1000", "alice", "Alice Amberg", { 1 }, { })
  checkMember("1001", "bob", "Bob Bobbersen", { 1, 2 }, { 1, 3 })
  checkMember("1002", "chris", "Chris Carv", { 1, 2 }, { 1, 3 })
  checkMember("1003", "daisy", "Daisy Duck", { 3 }, { 1, 2 })
  checkMember("1004", "ernst", "Ernst Ernst", { 3 }, { 1, 2 })
  checkMember("1005", "fredi", "Frederike Frei", { 1 }, { })
  checkMember("1006")
  checkMember("1007", "helen", "Helen Hofstatter", { 1, 2 }, { 1, 3 })
  checkMember("1008", "iwan", "Iwan Iwanowski", { 1 }, { })
  checkMember("1009", "jasmina", "Jasmina Jasik", { 1 }, { })

  print()
  
  local success_count = global_count - global_failed
  
  print (success_count .. " of " .. global_count .. " tests succeeded.")
 
end
