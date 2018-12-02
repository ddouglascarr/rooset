require 'openssl'
require 'jwt'  # https://rubygems.org/gems/jwt

################################################################################
#
# build-token.rb
#
# Copy-pasted from github's instructions on signing their JWT tokens.
# See: https://developer.github.com/apps/building-github-apps/authenticating-with-github-apps/#authenticating-as-a-github-app
#
# Usage:
#   $ gem install jwt
#   $ ruby etc/build-token.rb
#
#   TODO: implement in Golang, or, remove when done experimenting in shell
#
################################################################################

# Private key contents
private_pem = ENV["ROOSET_GITHUB_APP_PRIVATE_KEY"]
private_key = OpenSSL::PKey::RSA.new(private_pem)

# Generate the JWT
payload = {
  # issued at time
  iat: Time.now.to_i,
  # JWT expiration time (10 minute maximum)
  exp: Time.now.to_i + (10 * 60),
  # GitHub App's identifier
  iss: 20936
}

jwt = JWT.encode(payload, private_key, "RS256")
puts jwt
