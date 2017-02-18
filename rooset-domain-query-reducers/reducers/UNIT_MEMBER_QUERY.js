fromAll()
.foreachStream()
.when({

  $init: function() {
    return {
      id: null,
      members: [],
      totalWeight: 0,
    };
  },

  UNIT_CREATED_EVENT: function(s, e) {
    s.id = e.body.payload.id;
  },

  PRIVILEGE_GRANTED_EVENT: function(s, e) {
    const p = e.body.payload;
    s.members.push({
      id: p.id,
      memberId: p.memberId,
      pollingRight: p.pollingRight,
      votingRight: p.votingRight,
      initiativeRight: p.initiativeRight,
      managementRight: p.managementRight,
      weight: p.weight,
    });
    s.totalWeight += p.weight;
  },

})
