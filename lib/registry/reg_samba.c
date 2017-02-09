/* 
   Unix SMB/CIFS implementation.
   Copyright (C) Jelmer Vernooij			2004.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "includes.h"
#include "registry.h"

/**
 * @file
 * @brief Samba-specific registry functions
 */

static WERROR reg_samba_get_predef (struct registry_context *ctx, uint32_t hkey, struct registry_key **k)
{
	WERROR error;
	const char *conf;
	char *backend;
	const char *location;
	const char *hivename = reg_get_predef_name(hkey);

	*k = NULL;

	conf = lp_parm_string(-1, "registry", hivename);
	
	if (!conf) {
		return WERR_NOT_SUPPORTED;
	}

	location = strchr(conf, ':');
	if (location) {
		backend = talloc_strndup(ctx, conf, (int)(location - conf));
		location++;
	} else {
		backend = talloc_strdup(ctx, "ldb");
		location = conf;
	}

	/* FIXME: Different hive backend for HKEY_CLASSES_ROOT: merged view of HKEY_LOCAL_MACHINE\Software\Classes
	 * and HKEY_CURRENT_USER\Software\Classes */

	/* FIXME: HKEY_CURRENT_CONFIG is an alias for HKEY_LOCAL_MACHINE\System\CurrentControlSet\Hardware Profiles\Current */

	/* FIXME: HKEY_PERFORMANCE_DATA is dynamically generated */

	/* FIXME: HKEY_LOCAL_MACHINE\Hardware is autogenerated */

	/* FIXME: HKEY_LOCAL_MACHINE\Security\SAM is an alias for HKEY_LOCAL_MACHINE\SAM */

	error = reg_open_hive(ctx, backend, location, ctx->session_info, ctx->credentials, k);

	talloc_free(backend);

	return error;
}

_PUBLIC_ WERROR reg_open_local (TALLOC_CTX *mem_ctx, 
				struct registry_context **ctx, 
				struct auth_session_info *session_info, 
				struct cli_credentials *credentials)
{
	*ctx = talloc(mem_ctx, struct registry_context);
	(*ctx)->credentials = talloc_reference(*ctx, credentials);
	(*ctx)->session_info = talloc_reference(*ctx, session_info);
	(*ctx)->get_predefined_key = reg_samba_get_predef;
	
	return WERR_OK;
}