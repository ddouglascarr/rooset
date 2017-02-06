module.exports = (path) => {
  const pathLength = path.length;
  let pathVariables = [];
  let openIdx = null;
  for (let i = 0; i < pathLength; ++i) {
    if (path[i] === '{') {
      openIdx = i;
    } else if (path[i] === '}') {
      if (openIdx === null) throw new Error(
          "The closing uid path variable closing brace occurs before an opening one");
      pathVariables.push(path.slice(openIdx + 1, i));
      openIdx = null;
    }
  }
  return pathVariables;
};
