#include <algorithm>
#include <jampio/shared/shared.h>
#include "CommandLine.h"
#include "CommandArgs.h"

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

CommandLine::CommandLine(CommandBuffer& cbuf, CvarSystem& cvars, int argc, const char **argv)
	: m_cbuf(cbuf)
	, m_cvars(cvars)
	, m_lines(split(combine(argc, argv)))
{}

bool CommandLine::SafeMode() {
	bool res = false;
	std::remove_if(m_lines.begin(), m_lines.end(), [&](auto& str) {
		auto args = CommandArgs::TokenizeString(str.c_str());
		if (!Q_stricmp(args.Argv(0), "start") || !Q_stricmp(args.Argv(0), "cvar_restart")) {
			res = true;
			return true;
		}
		return false;
	});
	return res;
}

void CommandLine::StartupVariable(const char *match) {
	for (auto& cmd : m_lines) {
		auto args = CommandArgs::TokenizeString(cmd.c_str());
		if (Q_stricmp(args.Argv(0), "set")) continue;
		auto s = args.Argv(1);
		if (!match || !Q_stricmp(s, match)) {
			m_cvars.Set(s, args.Argv(2));
			auto cv = m_cvars.Get(s, "", 0);
			cv.add_flag(CVAR_USER_CREATED);
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
		m_cbuf.AddText(cmd.c_str());
		m_cbuf.AddText("\n");
	}
	return added;
}
