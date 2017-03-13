module.exports = {
  bool: v => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetBool(${v});
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());`;
  },

  email: v => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetString(${v}.c_str(), ${v}.size(), d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  defeatStrength: v => {
    const v_value = `${v}_value`;
    const v_str = `${v}_str`;
    return `
          Value ${v_value};
          auto ${v_str} = EnumUtils::serializeDefeatStrength(${v});
          ${v_value}.SetString(${v_str}.c_str(), ${v_str}.size(), d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  exceptionCode: v => {
    const v_value = `${v}_value`;
    const v_str = `${v}_str`;
    return `
          Value ${v_value};
          auto ${v_str} = EnumUtils::serializeExceptionCode(${v});
          ${v_value}.SetString(${v_str}.c_str(), ${v_str}.size(), d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  initiativeContentType: v => `
          payload.AddMember(
              "${v}",
              JsonUtils::serializeString(
                  EnumUtils::serializeInitiativeContentType(${v}),
                  d->GetAllocator()),
              d->GetAllocator());`,

  jsonObject: v => `
          payload.AddMember(
              "${v}",
              JsonUtils::parse(${v}),
              d->GetAllocator());`,

  schulzeBallot: v => {
    const v_value = `${v}_value`;
    return `
          auto ${v_value} = rooset::SchulzeBallot::serialize(${v}, d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  string: v => `
          payload.AddMember(
              "${v}",
              JsonUtils::serializeString(${v}, d->GetAllocator()),
              d->GetAllocator());`,

  uuid: v => `
          payload.AddMember(
              "${v}",
              JsonUtils::serializeUuid(${v}, d->GetAllocator()),
              d->GetAllocator());`,

  uuidNullable: v => `
          payload.AddMember(
              "${v}",
              JsonUtils::serializeUuid(${v}, d->GetAllocator()),
              d->GetAllocator());`,

  uuidArray: v => {
    const v_value = `${v}_value`;
    const v_str = `${v}_str`;
    return `
          auto ${v_value} = JsonUtils::serializeArray<uuid>(
              ${v},
              [](const uuid& id, rapidjson::Document::AllocatorType& allocator) {
                return JsonUtils::serializeUuid(id, allocator);
              },
              d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());
    `;
  },

  votingAlgorithm: v => `
          payload.AddMember(
              "${v}",
              JsonUtils::serializeString(
                  EnumUtils::serializeVotingAlgorithm(${v}),
                  d->GetAllocator()),
              d->GetAllocator());`,

  date: v => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetUint(${v});
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  int: v => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetInt(${v});
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());     `;
  },

  uint: v => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetUint(${v});
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());     `;
  },

  duration: v => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetUint(${v});
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },
};
