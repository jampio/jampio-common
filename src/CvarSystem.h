#pragma once

#include <jampio/shared/shared.h>
#include <map>
#include <string>
#include <optional>
#include <functional>
#include <array>
#include "Cvar.h"
#include "CommandArgs.h"

class CvarSystem {
private:
	int cvar_modifiedFlags;
	struct IgnorecaseCompare {
		int operator()(const std::string& a, const std::string& b) {
			return Q_stricmp(a.c_str(), b.c_str());
		}
	};
	std::map<std::string, Cvar, IgnorecaseCompare> m_table;
public:
	Cvar &Set2(const char *var_name, const char *value, bool force);
	CvarSystem();
	std::optional<std::reference_wrapper<Cvar>> FindVar(const char *name);
	float VariableValue(const char *name);
	int VariableIntegerValue(const char *name);
	const char *VariableString(const char *name);
	void VariableStringBuffer(const char *name, char *buffer, int bufsize);
	void CommandCompletion(void (*callback)(const char *cvar_name));
	// If the variable already exists, the value will not be set unless CVAR_ROM
	// The flags will be or'ed in if the variable exists.
	Cvar &Get(const char *name, const char *value, int flags);
	void Set(const char *name, const char *value);
	void SetLatched(const char *name, const char *value);
	void SetValue(const char *name, float value);
	void Reset(const char *name);
	void SetCheatState();
	// Handles variable inspection and changing from the console
	bool Command(CommandArgs& args);
	void Register(vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags);
	void Update(vmCvar_t *vmCvar);
	std::size_t size() const;
	void removeModifiedFlags(int flags);
	bool hasModifiedFlags(int flags) const;
	void WriteVariables(fileHandle_t f);
	// Resets all cvars to their hardcoded values
	void Restart();
	std::array<char, MAX_INFO_STRING> InfoString(int bit);
	std::array<char, BIG_INFO_STRING> InfoStringBig(int bit);
	template <typename F>
	void CvarIter(F&& f) {
		for (auto& pair : m_table) {
			auto& cvar = pair.second;
			// Dont show internal cvars
			if (cvar.flags() & CVAR_INTERNAL) continue;
			f(cvar);
		}
	}
};