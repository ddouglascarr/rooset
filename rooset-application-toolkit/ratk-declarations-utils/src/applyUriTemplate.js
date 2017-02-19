const { template } = require('lodash');

module.exports = (uriTmpl, vars) => {
  const opts = { interpolate:  /\{(.+?)\}/g };
  const tmpl = template(uriTmpl, opts);
  return tmpl(vars);
}
