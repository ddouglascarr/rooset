const { template } = require('lodash');

module.exports = (uriTmpl, vars) => {
  try {
    const opts = { interpolate:  /\{(.+?)\}/g };
    const tmpl = template(uriTmpl, opts);
    return tmpl(vars);
  } catch (err) {
    throw new Error(`
      applyUriTemplate failed with error: ${err.message}.
      uriTmpl: ${uriTmpl}
      vars: ${JSON.stringify(vars, null, 2)}`);
  }
}
