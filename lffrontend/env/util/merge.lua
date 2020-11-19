-- merge a bunch of tables into the target
-- util.merge({}, { {foo='bar'}, {bing='bong'} }) -> { foo='bar', bing='bong' }
function util.merge(target, sources)
  for _, source in pairs(sources) do
    for k, v in pairs(source) do
        target[k] = v
    end
  end
  return target
end
