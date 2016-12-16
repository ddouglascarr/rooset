module.exports = {

  bool: (v) => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetBool(${v});
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());`;
  },

  email: (v) => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetString(${v}.c_str(), ${v}.size(), d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  defeatStrength: (v) => {
    const v_value = `${v}_value`;
    const v_str = `${v}_str`;
    return `
          Value ${v_value};
          auto ${v_str} = EnumUtils::serializeDefeatStrength(${v});
          ${v_value}.SetString(${v_str}.c_str(), ${v_str}.size(), d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  exceptionCode: (v) => {
    const v_value = `${v}_value`;
    const v_str = `${v}_str`;
    return `
          Value ${v_value};
          auto ${v_str} = EnumUtils::serializeExceptionCode(${v});
          ${v_value}.SetString(${v_str}.c_str(), ${v_str}.size(), d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  string: (v) => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetString(${v}.c_str(), ${v}.size(), d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  uuid: (v) => {
    const v_value = `${v}_value`;
    const v_str = `${v}_str`;
    return `
          Value ${v_value};
          string ${v_str} = idTools->serialize(${v});
          ${v_value}.SetString(${v_str}.c_str(), ${v_str}.size(), d->GetAllocator());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

  date: (v) => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetDouble(boost::posix_time::to_time_t(${v}));
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;

  },

  int: (v) => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetInt(${v});
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());     `;
  },

  duration: (v) => {
    const v_value = `${v}_value`;
    return `
          Value ${v_value};
          ${v_value}.SetDouble(${v}.seconds());
          payload.AddMember("${v}", ${v_value}, d->GetAllocator());    `;
  },

};