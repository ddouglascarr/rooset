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

  schulzeBallot: (v) => `
            ${v}(rooset::SchulzeBallot::parse(d["payload"]["${v}"]))`,

  string: (v) => `
            ${v}(JsonUtils::parseString(d["payload"]["${v}"]))`,

  uuid: (v) => `
            ${v}(JsonUtils::parseUuid(d["payload"]["${v}"]))`,

  uuidNullable: (v) => `
            ${v}(JsonUtils::parseUuid(d["payload"]["${v}"]))`,

  uuidArray: (v) => `
            ${v}(JsonUtils::parseArray<uuid>(
                d["payload"]["${v}"],
                [&](const rapidjson::Value& v) { return JsonUtils::parseUuid(v); }))`,

  date: (v) => `
            ${v}(boost::posix_time::from_time_t(
                time_t(d["payload"]["${v}"].GetDouble())))`,

  int: (v) => `
            ${v}(d["payload"]["${v}"].GetInt())`,

  uint: (v) => `
            ${v}(d["payload"]["${v}"].GetUint())`,

  duration: (v) => `
            ${v}(d["payload"]["${v}"].GetUint())`,
};
