fromCategory("aggregate")
.partitionBy(function(event) {
  return event.body.payload.urlParameterName || "na";
})
.when({
  $init: function() {
    return { id: null };
  },

  UNIT_CREATED_EVENT: function(s, e) {
    s.id = e.body.payload.id;
  }
})
