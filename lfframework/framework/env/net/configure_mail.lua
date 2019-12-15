--[[--
net.configure_mail{
  command              = command,              -- table with binary name and static command line arguments
  envelope_from_option = envelope_from_option  -- command line option to select "envelope from", e.g. "-f"
}

Configures the mail delivery system.
Default: net.configure_mail{ command={"sendmail", "-t", "-i"}, envelope_from_option="-f" }

--]]--

function net.configure_mail(args)
  local mail_config = {
    command              = table.new(args.command),
    envelope_from_option = envelope_from_option
  }
  request.configure(function()
    net._mail_config = mail_config
  end)
end
