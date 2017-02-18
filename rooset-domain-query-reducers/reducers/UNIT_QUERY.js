fromAll()
.foreachStream()
.when({

  $init: function() {
    return {
      id: null,
      name: "",
      description: "",
      areas: [],
      policy: null,
    };
  },

  UNIT_CREATED_EVENT: function(s, e) {
    s.id = e.body.payload.id;
    s.name = e.body.payload.name;
    s.description = e.body.payload.description;
  }

});
