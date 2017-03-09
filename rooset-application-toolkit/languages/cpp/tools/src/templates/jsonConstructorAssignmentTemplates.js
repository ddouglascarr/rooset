module.exports = {
  bool: v => `
            ${v}(d["payload"]["${v}"].GetBool())`,

  email: v => `
            ${v}(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength()))`,

  initiativeContentType: v => `
            ${v}(EnumUtils::parseInitiativeContentType(
                JsonUtils::parseString(d["paylaod"]["${v}"])))`,

  defeatStrength: v => `
            ${v}(EnumUtils::parseDefeatStrength(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength())))`,

  exceptionCode: v => `
            ${v}(EnumUtils::parseExceptionCode(string(
                d["payload"]["${v}"].GetString(),
                d["payload"]["${v}"].GetStringLength())))`,

  jsonObject: v => `
            ${v}(JsonUtils::parseString(d["payload"]["${v}"]))`,
            
  schulzeBallot: v => `
            ${v}(rooset::SchulzeBallot::parse(d["payload"]["${v}"]))`,

  string: v => `
            ${v}(JsonUtils::parseString(d["payload"]["${v}"]))`,

  uuid: v => `
            ${v}(JsonUtils::parseUuid(d["payload"]["${v}"]))`,

  uuidNullable: v => `
            ${v}(JsonUtils::parseUuid(d["payload"]["${v}"]))`,

  uuidArray: v => `
            ${v}(JsonUtils::parseArray<uuid>(
                d["payload"]["${v}"],
                [&](const rapidjson::Value& v) { return JsonUtils::parseUuid(v); }))`,

  date: v => `
            ${v}(d["payload"]["${v}"].GetUint())`,

  int: v => `
            ${v}(d["payload"]["${v}"].GetInt())`,

  uint: v => `
            ${v}(d["payload"]["${v}"].GetUint())`,

  duration: v => `
            ${v}(d["payload"]["${v}"].GetUint())`,
};
