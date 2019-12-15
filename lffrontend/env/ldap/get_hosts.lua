-- generate a ready-to-use list of the configures LDAP servers
-- --------------------------------------------------------------------------
-- result is in order of preference, including a round robin style random
-- selection of the preference of hosts with the same preference
--
-- returns:
--   hostlist: flattened list of LDAP servers in order of preference

function ldap.get_hosts()
  
  math.randomseed(os.time())
  
  local hostlist = {}
  
  -- iterate through all entries on base level
  for i, host in ipairs (config.ldap.hosts) do
    
    -- a single host entry
    if host.uri then
      hostlist[#hostlist+1] = host
      
    -- or a list of host entries
    else
      local subhostlist = {}
      
      -- sort list of host entries by random
      for j, subhost in ipairs(host) do
        subhost.priority = math.random()
        subhostlist[#subhostlist+1] = subhost
      end
      table.sort(subhostlist, function (a,b)
        return a.priority < b.priority
      end)
      
      -- and add them to the main list
      for i, subhost in ipairs(subhostlist) do
        hostlist[#hostlist+1] = subhost
      end
      
    end
    
  end
  
  return hostlist
  
end
