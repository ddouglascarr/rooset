--[[--
request.redirect{
  external = external,   -- external URL (instead of specifying base, module, etc. below)
  base     = base,       -- optional string containing a base URL of a WebMCP application
  static   = static,     -- an URL relative to the static file directory
  module   = module,     -- a module name of the WebMCP application
  view     = view,       -- a view name of the WebMCP application
  action   = action,     -- an action name of the WebMCP application
  id       = id,         -- optional id to be passed to the view or action to select a particular data record
  params   = params,     -- optional parameters to be passed to the view or action
  anchor   = anchor      -- anchor in URL
}

Calling this function causes the WebMCP to do a 303 HTTP redirect after the current view or action and all filters have finished execution. If routing mode "redirect" has been chosen, then this function is called automatically after an action and all its filters have finished execution. Calling request.redirect{...} (or request.forward{...}) explicitly inside an action will cause routing information from the browser to be ignored. To preserve GET/POST parameters of an action, use request.forward{...} instead. Currently no redirects to external (absolute) URLs are possible, there will be an implementation in future though.

--]]--

function request.redirect(args)
  args = table.new(args)
  if type(args.external) ~= "string" and type(args.static) ~= "string" then
    if type(args.module) ~= "string" then
      error("No module string passed to request.redirect{...}.")
    end
    if type(args.view) ~= "string" then
      error("No view string passed to request.redirect{...}.")
    end
    if args.params ~= nil and type(args.params) ~= "table" then
      error("Params array passed to request.redirect{...} is not a table.")
    end
    if args.anchor ~= nil and type(args.anchor) ~= "string" then
      error("Anchor passed to request.redirect{...} must be a string or nil.")
    end
  end
  if request.is_rerouted() then
    error("Tried to redirect after another forward or redirect.")
  end
  request._redirect = args
  if args.module and args.view then  -- TODO: support for external redirects
    trace.redirect{ module = args.module, view = args.view }
  end
end
