#include "CommandLine.h"
#include "common.h"
#include <algorithm>

// combines into a single string
// skips the first arg
static std::string combine(int argc, const char **argv) {
	std::string res;
	for (decltype(argc) i = 1; i < argc; i++) {
		res += argv[i];
	}
	return res;
}

// splits at + or \n
static std::vector<std::string> split(const std::string& str) {
	std::vector<std::string> res;
	std::string acc;
	for (auto ch : str) {
		if (ch == '+' || ch == '\n') {
			if (!acc.empty()) {
				res.push_back(std::move(acc));
				acc = std::string();
			}
		} else {
			acc.push_back(ch);
		}
	}
	if (!acc.empty()) {
		res.push_back(std::move(acc));
	}
	return res;
}

CommandLine::CommandLine(int argc, const char **argv)
	: m_lines(split(combine(argc, argv)))
{}

bool CommandLine::SafeMode() {
	bool res = false;
	std::remove_if(m_lines.begin(), m_lines.end(), [&](auto& str) {
		Cmd_TokenizeString(str.c_str());
		if (!Q_stricmp(Cmd_Argv(0), "start") || !Q_stricmp(Cmd_Argv(0), "cvar_restart")) {
			res = true;
			return true;
		}
		return false;
	});
	return res;
}

void CommandLine::StartupVariable(const char *match) {
	for (auto& cmd : m_lines) {
		Cmd_TokenizeString(cmd.c_str());
		if (Q_stricmp(Cmd_Argv(0), "set")) continue;
		auto s = Cmd_Argv(1);
		if (!match || !Q_stricmp(s, match)) {
			Cvar_Set(s, Cmd_Argv(2));
			auto cv = Cvar_Get(s, "", 0);
			cv->flags |= CVAR_USER_CREATED;
		}
	}
}

bool CommandLine::AddStartupCommands() {
	bool added = false;
	for (auto& cmd : m_lines) {
		// set commands won't override menu startup
		if (Q_stricmpn(cmd.c_str(), "set", 3)) {
			added = true;
		}
		Cbuf_AddText(cmd.c_str());
		Cbuf_AddText("\n");
	}
	return added;
}
