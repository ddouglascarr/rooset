module.exports = {
  bool: (v) => `const bool ${v};`,
  email: (v) => `const string ${v};`,
  exceptionCode: (v) `const rooset::ExceptionCode ${v};`,
  string: (v) => `const string ${v};`,
  uuid: (v) => `const uuid ${v};`,
  date: (v) => `const boost::posix_time::ptime ${v};`,
};
