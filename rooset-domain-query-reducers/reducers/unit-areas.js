fromAll()
.foreachStream()
.when({

  $init: function(s, e) {
    return { units: [] };
  },


  AREA_CREATED_EVENT: function(s, e) {
    s.units.push(e.body.payload.areaId);
  },

});
