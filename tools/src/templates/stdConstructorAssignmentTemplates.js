module.exports = {

  bool: (v, prefix) => `${v}(${prefix}${v})`,
  email: (v, prefix) => `${v}(${prefix}${v}) `,
  exceptionCode: (v, prefix) => `${v}(${prefix}${v})`,
  string: (v, prefix) => `${v}(${prefix}${v}) `,
  uuid: (v, prefix) => `${v}(${prefix}${v}) `,
  date: (v, prefix) => `${v}(${prefix}${v}) `,

};
