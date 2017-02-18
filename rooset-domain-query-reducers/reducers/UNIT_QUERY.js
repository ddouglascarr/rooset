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

});
