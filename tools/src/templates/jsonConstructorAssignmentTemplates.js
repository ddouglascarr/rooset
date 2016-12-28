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

  uuidArray: (v) => `
            ${v}(JsonUtils::parseArray<uuid>(
                d["payload"]["${v}"],
                [&](const rapidjson::Value& v) { return idTools->parse(string(v.GetString(), v.GetStringLength())); }))`,

  date: (v) => `
            ${v}(boost::posix_time::from_time_t(
                time_t(d["payload"]["${v}"].GetDouble())))`,

  int: (v) => `
            ${v}(d["payload"]["${v}"].GetInt())`,

  uint: (v) => `
            ${v}(d["payload"]["${v}"].GetUint())`,

  duration: (v) => `
            ${v}(boost::posix_time::seconds(d["payload"]["${v}"].GetDouble()))`,
};
