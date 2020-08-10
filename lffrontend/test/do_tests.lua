function do_test(test)
  assert(
		os.execute(
			'dropdb --if-exists rooset_test && createdb -T rooset_template rooset_test'),
		"could not create database")

  execute.postfork_initializers()
  test()
  execute.finalizers()
end

function do_tests(tests)
  assert(
		config.database.dbname == 'rooset_test',
		'database must be called rooset_test')

  for i, test in ipairs(tests) do
    do_test(test)
  end
end

-- require all tests
local my_tests = require('test.my_test')

all_tests = my_tests
do_tests(all_tests)
