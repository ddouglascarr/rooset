/* This file contains error code mappings of LDAP error codes and OpenLDAP
 * error codes.
 *
 * The collection of error codes (mldap_errorcodes[]) has been derived from
 * the file ldap.h that is part of OpenLDAP Software. OpenLDAP's license
 * information is stated below:
 *
 * This work is part of OpenLDAP Software <http://www.openldap.org/>.
 * 
 * Copyright 1998-2013 The OpenLDAP Foundation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted only as authorized by the OpenLDAP
 * Public License.
 *
 * A copy of this license is available below:
 *
 * The OpenLDAP Public License
 * Version 2.8, 17 August 2003
 *
 * Redistribution and use of this software and associated documentation
 * ("Software"), with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 * 1. Redistributions in source form must retain copyright statements
 *    and notices,
 * 
 * 2. Redistributions in binary form must reproduce applicable copyright
 *    statements and notices, this list of conditions, and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution, and
 * 
 * 3. Redistributions must contain a verbatim copy of this document.
 * 
 * The OpenLDAP Foundation may revise this license from time to time.
 * Each revision is distinguished by a version number.  You may use
 * this Software under terms of this license revision or under the
 * terms of any subsequent revision of the license.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE OPENLDAP FOUNDATION AND ITS
 * CONTRIBUTORS ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT
 * SHALL THE OPENLDAP FOUNDATION, ITS CONTRIBUTORS, OR THE AUTHOR(S)
 * OR OWNER(S) OF THE SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * The names of the authors and copyright holders must not be used in
 * advertising or otherwise to promote the sale, use or other dealing
 * in this Software without specific, written prior permission.  Title
 * to copyright in this Software shall at all times remain with copyright
 * holders.
 * 
 * OpenLDAP is a registered trademark of the OpenLDAP Foundation.
 * 
 * Copyright 1999-2003 The OpenLDAP Foundation, Redwood City,
 * California, USA.  All Rights Reserved.  Permission to copy and
 * distribute verbatim copies of this document is granted.
 *
 * End of OpenLDAP Public License
 *
 * Portions Copyright (c) 1990 Regents of the University of Michigan.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of Michigan at Ann Arbor. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 * End of OpenLDAP's license information
 */

// type for entry in mldap_errorcodes[] array:
struct mldap_errorcode {
  const char *ident;
  int code;
};

// NULL terminated array of error code strings with error code integers
// derived from ldap.h (see above copyright notice):
static const struct mldap_errorcode mldap_errorcodes[] = {
  {"operations_error", 1},
  {"protocol_error", 2},
  {"timelimit_exceeded", 3},
  {"sizelimit_exceeded", 4},
  {"compare_false", 5},
  {"compare_true", 6},
//  {"auth_method_not_supported", 7},
  {"strong_auth_not_supported", 7},
  {"strong_auth_required", 8},
//  {"stronger_auth_required", 8},
  {"partial_results", 9},
  {"referral", 10},
  {"adminlimit_exceeded", 11},
  {"unavailable_critical_extension", 12},
  {"confidentiality_required", 13},
  {"sasl_bind_in_progress", 14},
  {"no_such_attribute", 16},
  {"undefined_type", 17},
  {"inappropriate_matching", 18},
  {"constraint_violation", 19},
  {"type_or_value_exists", 20},
  {"invalid_syntax", 21},
  {"no_such_object", 32},
  {"alias_problem", 33},
  {"invalid_dn_syntax", 34},
  {"is_leaf", 35},
  {"alias_deref_problem", 36},
  {"x_proxy_authz_failure", 47},
  {"inappropriate_auth", 48},
  {"invalid_credentials", 49},
  {"insufficient_access", 50},
  {"busy", 51},
  {"unavailable", 52},
  {"unwilling_to_perform", 53},
  {"loop_detect", 54},
  {"naming_violation", 64},
  {"object_class_violation", 65},
  {"not_allowed_on_nonleaf", 66},
  {"not_allowed_on_rdn", 67},
  {"already_exists", 68},
  {"no_object_class_mods", 69},
  {"results_too_large", 70},
  {"affects_multiple_dsas", 71},
  {"vlv_error", 76},
  {"other", 80},
  {"cup_resources_exhausted", 113},
  {"cup_security_violation", 114},
  {"cup_invalid_data", 115},
  {"cup_unsupported_scheme", 116},
  {"cup_reload_required", 117},
  {"cancelled", 118},
  {"no_such_operation", 119},
  {"too_late", 120},
  {"cannot_cancel", 121},
  {"assertion_failed", 122},
  {"proxied_authorization_denied", 123},
  {"sync_refresh_required", 4096},
  {"x_sync_refresh_required", 16640},
  {"x_assertion_failed", 16655},
  {"x_no_operation", 16654},
  {"x_no_referrals_found", 16656},
  {"x_cannot_chain", 16657},
  {"x_invalidreference", 16658},
  {"x_txn_specify_okay", 16672},
  {"x_txn_id_invalid", 16673},
  {"server_down", (-1)},
  {"local_error", (-2)},
  {"encoding_error", (-3)},
  {"decoding_error", (-4)},
  {"timeout", (-5)},
  {"auth_unknown", (-6)},
  {"filter_error", (-7)},
  {"user_cancelled", (-8)},
  {"param_error", (-9)},
  {"no_memory", (-10)},
  {"connect_error", (-11)},
  {"not_supported", (-12)},
  {"control_not_found", (-13)},
  {"no_results_returned", (-14)},
  {"more_results_to_return", (-15)},
  {"client_loop", (-16)},
  {"referral_limit_exceeded", (-17)},
  {"x_connecting", (-18)},
  {NULL, 0}
};

void mldap_set_errorcodes(lua_State *L) {
  // stores mldap_errorcodes[] mappings in the Lua table on top of the stack
  // in both directions (string mapped to integer and vice versa)

  const struct mldap_errorcode *errorcode;  // pointer to entry in mldap_errorcodes[] array

  // iterate through entries in mldap_errorcodes[] array:
  for (errorcode=mldap_errorcodes; errorcode->ident; errorcode++) {

    // store a mapping from the string to the integer:
    lua_pushstring(L, errorcode->ident);
    lua_pushinteger(L, errorcode->code);
    lua_settable(L, -3);

    // store a mapping from the integer to the string:
    lua_pushinteger(L, errorcode->code);
    lua_pushstring(L, errorcode->ident);
    lua_settable(L, -3);

  }

}

