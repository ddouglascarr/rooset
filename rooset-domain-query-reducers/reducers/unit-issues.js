fromAll()
.partitionBy(function(e) { return e.body.payload.unitId || 'na'; })
.when({
  
  $init: function(s, e) {
    return { issues: {} };
  },
  
  NEW_INITIATIVE_CREATED_EVENT: function(s, e) {
    var initiatives = {};
    initiatives[e.body.payload.initiativeId] = { supporters: 1 };
    s.issues[e.body.payload.id] = {
      initiatives: initiatives,
    };
  },

});
