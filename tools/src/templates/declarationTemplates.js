module.exports = {
  bool: (v) => `const bool ${v};`,
  email: (v) => `const string ${v};`,
  defeatStrength: (v) => `const rooset::DefeatStrength ${v};`,
  exceptionCode: (v) `const rooset::ExceptionCode ${v};`,
  string: (v) => `const string ${v};`,
  schulzeBallot: (v) => `const rooset::SchulzeBallot ${v};`,
  uuid: (v) => `const uuid ${v};`,
  uuidArray: (v) => `const vector<uuid> ${v};`,
  date: (v) => `const boost::posix_time::ptime ${v};`,
  int: (v) => `const int ${v};`,
  uint: (v) => `const unsigned int ${v};`,
  duration: (v) => `const boost::posix_time::time_duration ${v};`,
};
