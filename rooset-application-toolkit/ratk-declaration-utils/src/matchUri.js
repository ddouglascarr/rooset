// Compare A Spring Uri patter  e.g. /units/{id}/members,
// with an actual path e.g /units/464b1ebb-32c1-460c-8e9e-111111111111/members
// return true if they match
const { cloneDeep } = require('lodash');

module.exports = (_path, _target) => {
  let path = _path.split('/');
  let target = _target.split('/'); // dont mutate parameters

  if (path.length !== target.length) return false;
  for (let i = 0; i < path.length; ++i) {
    const isPathTemplateSection = path[i].slice(0,1) === '{';
    if (!isPathTemplateSection && path[i] !== target[i]) return false;
  }
  return true;
};

