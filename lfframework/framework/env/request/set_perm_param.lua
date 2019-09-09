--[[--
request.set_perm_param(
  name,                  -- name of parameter
  value                  -- value of parameter
)

Setting a so-called "permanent parameter" with this function will cause a key value pair to be included in every HTTP link inside the application. It is used as a cookie replacement, where cookies are not suitable, e.g. because you want multiple browser windows to not interfere with each other. 

--]]--

function request.set_perm_param(name, value)
  request.configure(function()
    request._perm_params[name] = value
  end)
end
