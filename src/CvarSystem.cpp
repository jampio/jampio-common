#include <cstring>
#include "CvarSystem.h"
#include "com.h"

static bool Cvar_ValidateString(const char *s) {
	if (!s) return false;
	if (strchr(s, '\\')) return false;
	if (strchr(s, '\"')) return false;
	if (strchr(s, ';')) return false;
	return true;
}

CvarSystem::CvarSystem()
	: cvar_modifiedFlags(0)
	, m_table()
{}

std::optional<std::reference_wrapper<Cvar>> CvarSystem::FindVar(const char *name) {
	auto search = m_table.find(name);
	if (search == m_table.end()) return {};
	return search->second;
}

float CvarSystem::VariableValue(const char *name) {
	auto var = FindVar(name);
	if (var) var->get().name();
	return 0;
}

int CvarSystem::VariableIntegerValue(const char *name) {
	auto var = FindVar(name);
	if (var) return var->get().integer();
	return 0;
}

const char *CvarSystem::VariableString(const char *name) {
	auto var = FindVar(name);
	if (var) return var->get().string();
	return "";
}

void CvarSystem::VariableStringBuffer(const char *name, char *buffer, int bufsize) {
	auto var = FindVar(name);
	if (var) Q_strncpyz(buffer, var->get().string(), bufsize);
	else *buffer = 0;
}

void CvarSystem::CommandCompletion(void (*callback)(const char *name)) {
	for (auto& pair : m_table) {
		auto& cvar = pair.second;
		// Dont show internal cvars
		if (cvar.flags() & CVAR_INTERNAL) continue;
		callback(cvar.name());
	}
}

Cvar &CvarSystem::Get(const char *name, const char *value, int flags) {
	if (!name || !value) {
		Com_Error(ERR_FATAL, "CvarSystem::Get: NULL parameter");
 	}

	if (!Cvar_ValidateString(name)) {
		Com_Printf("invalid cvar name string: %s\n", name);
		name = "BADNAME";
	}

	if (auto opt_var = FindVar(name); opt_var) {
		Cvar& var = *opt_var;
		// if the C code is now specifying a variable that the user already
		// set a value for, take the new value as the reset value
		if (var.has_flag(CVAR_USER_CREATED) && !(flags & CVAR_USER_CREATED) && value[0]) {
			var.remove_flag(CVAR_USER_CREATED);
			var.m_resetString = value;

			// ZOID--needs to be set so that cvars the game sets as 
			// SERVERINFO get sent to clients
			cvar_modifiedFlags |= flags;
		}
		var.add_flag(flags);
		// only allow one non-empty reset string without a warning
		if (var.m_resetString.empty()) {
			// we don't have a reset string yet
			var.m_resetString = value;
		} else if (value[0] && strcmp(var.resetString(), value)) {
			Com_DPrintf("Warning: cvar \"%s\" given initial values: \"%s\" and \"%s\"\n",
				name, var.resetString(), value);
		}
		// if we have a latched string, take that value now
		if (!var.m_latchedString.empty()) {
			Set2(name, var.latchedString(), true);
			var.m_latchedString = "";
		}
		return var;
	}

	Cvar cvar;
	cvar.m_name = name;
	cvar.m_string = value;
	cvar.m_modified = true;
	cvar.m_modificationCount = 1;
	cvar.m_value = atof(cvar.m_string.c_str());
	cvar.m_integer = atoi(cvar.m_string.c_str());
	cvar.m_resetString = value;
	cvar.m_flags = flags;

	auto res = m_table.insert(std::make_pair(std::string(value), std::move(cvar)));
	return res.first->second;
}

Cvar &CvarSystem::Set2(const char *var_name, const char *value, bool force) {
	if (!Cvar_ValidateString(var_name)) {
		Com_Printf("invalid cvar name string: %s\n", var_name);
		var_name = "BADNAME";
	}
	auto opt_var = FindVar(var_name);
	if (!opt_var) {
		if (!value) {
			Com_Printf("WARNING: Cvar::Set2, value is null when setting var_name: %s\n", var_name);
			return Get(var_name, "", 0);
		}
		// create it
		if (!force) return Get(var_name, value, CVAR_USER_CREATED);
		return Get(var_name, value, 0);
	}
	Cvar& var = *opt_var;
	// Dont display the update when its internal
	if (!var.has_flag(CVAR_INTERNAL)) {
		Com_DPrintf( "Cvar_Set2: %s %s\n", var_name, value );
	}
	if (!value ) {
		value = var.resetString();
	}
	if (!strcmp(value, var.string())) {
		return var;
	}
	// note what types of cvars have been modified (userinfo, archive, serverinfo, systeminfo)
	cvar_modifiedFlags |= var.flags();

	if (!force)
	{
		if (var.flags() & CVAR_ROM)
		{
			Com_Printf ("%s is read only.\n", var_name);
			return var;
		}
		if (var.flags() & CVAR_INIT)
		{
			Com_Printf ("%s is write protected.\n", var_name);
			return var;
		}
		if (var.flags() & CVAR_LATCH)
		{
			if (!var.m_latchedString.empty())
			{
				if (strcmp(value, var.latchedString()) == 0)
					return var;
				var.m_latchedString = "";
			}
			else
			{
				if (strcmp(value, var.string()) == 0)
					return var;
			}
			Com_Printf("%s will be changed upon restarting.\n", var_name);
			var.m_latchedString = value;
			var.m_modified = true;
			var.m_modificationCount++;
			return var;
		}
		if ((var.flags() & CVAR_CHEAT) && !VariableIntegerValue("sv_cheats"))
		{
			Com_Printf("%s is cheat protected.\n", var_name);
			return var;
		}
	}
	else
	{
		if (!var.m_latchedString.empty())
		{
			var.m_latchedString = "";
		}
	}
	if (!strcmp(value, var.string()))
		return var;		// not changed

	var.m_modified = true;
	var.m_modificationCount++;
	var.m_string = value;
	var.m_value = atof(var.string());
	var.m_integer = atoi(var.string());
	return var;
}

bool CvarSystem::Command(CommandArgs& args) {
	// check variables
	auto opt_var = FindVar(args.Argv(0));
	if (!opt_var) return false;
	Cvar &v = *opt_var;
	// perform a variable print or set
	if (args.Argc() == 1) {
		Com_Printf("\"%s\" is:\"%s" S_COLOR_WHITE "\" default:\"%s" S_COLOR_WHITE "\"\n", v.name(), v.string(), v.resetString());
		if (!v.m_latchedString.empty()) {
			Com_Printf("latched: \"%s\"\n", v.latchedString());
		}
		return qtrue;
	}
	// JFM toggle test
	const char *value;
	value = args.Argv(1);
	if (value[0] == '!') {// toggle
		char buff[5];
		sprintf(buff, "%i", !v.value());
		Set2(v.name(), buff, false); // toggle the value
	} else {
		Set2(v.name(), value, false); // set the value if forcing isn't required
	}
	return true;
}

void CvarSystem::Register(vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags) {
	auto& cv = Get(varName, defaultValue, flags);
	if (!vmCvar) {
		return;
	}
	vmCvar->handle = (int) std::addressof(cv);
	vmCvar->modificationCount = -1;
	Update(vmCvar);
}

void CvarSystem::Update(vmCvar_t *vmCvar) {
	assert(vmCvar); // bk
	auto cv = (Cvar *) vmCvar->handle;
	if (cv->modificationCount() == vmCvar->modificationCount) {
		return;
	}
	if (cv->m_string.empty()) {
		return;		// variable might have been cleared by a cvar_restart
	}
	vmCvar->modificationCount = cv->modificationCount();
	// bk001129 - mismatches.
	if ( strlen(cv->string())+1 > MAX_CVAR_VALUE_STRING ) 
	  Com_Error( ERR_DROP, "Cvar_Update: src %s length %d exceeds MAX_CVAR_VALUE_STRING",
		     cv->string(), 
		     strlen(cv->string()), 
		     sizeof(vmCvar->string) );
	// bk001212 - Q_strncpyz guarantees zero padding and dest[MAX_CVAR_VALUE_STRING-1]==0 
	// bk001129 - paranoia. Never trust the destination string.
	// bk001129 - beware, sizeof(char*) is always 4 (for cv->string). 
	//            sizeof(vmCvar->string) always MAX_CVAR_VALUE_STRING
	//Q_strncpyz( vmCvar->string, cv->string, sizeof( vmCvar->string ) ); // id
	Q_strncpyz(vmCvar->string, cv->string(),  MAX_CVAR_VALUE_STRING); 

	vmCvar->value = cv->value();
	vmCvar->integer = cv->integer();
}

void CvarSystem::CvarIter(void (*callback)(Cvar& cvar)) {
	for (auto& pair : m_table) {
		auto& cvar = pair.second;
		// Dont show internal cvars
		if (cvar.flags() & CVAR_INTERNAL) continue;
		callback(cvar);
	}
}

std::size_t CvarSystem::size() const {
	return m_table.size();
}