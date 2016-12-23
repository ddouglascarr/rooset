module.exports = {

  bool: (v, prefix) => `${v}(${prefix}${v})`,
  email: (v, prefix) => `${v}(${prefix}${v}) `,
  defeatStrength: (v, prefix) => `${v}(${prefix}${v})`,
  exceptionCode: (v, prefix) => `${v}(${prefix}${v})`,
  string: (v, prefix) => `${v}(${prefix}${v}) `,
  uuid: (v, prefix) => `${v}(${prefix}${v}) `,
  uuidArray: (v, prefix) => `${v}(${prefix}${v})`,
  date: (v, prefix) => `${v}(${prefix}${v}) `,
  int: (v, prefix) => `${v}(${prefix}${v}) `,
  duration: (v, prefix) => `${v}(${prefix}${v}) `,

};
