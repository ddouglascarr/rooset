module.exports = {
  bool: (v) => `
            ${v}(d["payload"]["${v}"].GetBool())`,

  email: (v) => `
            ${v}(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength()))`,

  exceptionCode: (v) => `
            ${v}(JsonUtils::parseExceptionCode(string(
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
};
