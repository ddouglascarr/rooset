function format.percent_floor(p, q)
  if q > 0 then
    return math.floor(100 * p / q) .. "%"
  else
    return "0%"
  end
end
