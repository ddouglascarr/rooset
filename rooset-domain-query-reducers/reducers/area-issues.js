fromAll()
.partitionBy(function(e) { return e.body.payload.areaId || 'na'; })
.when({
  $init: function(s, e) {
    return { issues: [] };
  },

  NEW_INITIATIVE_CREATED_EVENT: function(s, e) {
    s.issues.push({
      id: e.body.payload.id,
      created: e.body.payload.created,
    });
  },
})
