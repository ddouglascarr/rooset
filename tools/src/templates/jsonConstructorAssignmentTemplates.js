module.exports = {
  bool: (v) => `
            ${v}(d["payload"]["${v}"].GetBool())`,

  email: (v) => `
            ${v}(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength()))`,

  defeatStrength: (v) => `
            ${v}(EnumUtils::parseDefeatStrength(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength())))`,

  exceptionCode: (v) => `
            ${v}(EnumUtils::parseExceptionCode(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength())))`,

  string: (v) => `
            ${v}(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength()))`,

  uuid: (v) => `
            ${v}(idTools->parse(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength())))`,

  date: (v) => `
            ${v}(boost::posix_time::from_time_t(
                time_t(d["payload"]["${v}"].GetDouble())))`,

  int: (v) => `
            ${v}(d["payload"]["${v}"].GetInt())`,

  duration: (v) => `
            ${v}(boost::posix_time::seconds(d["payload"]["${v}"].GetDouble()))`,
};
