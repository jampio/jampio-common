#pragma once

#include <functional>
#include <map>
#include <string>
#include "CvarSystem.h"
#include "CommandArgs.h"

class CommandSystem {
private:
	int m_wait;
	std::map<std::string, std::function<void(CommandArgs&)>> m_table;
	CvarSystem& m_cvars;
public:
	CommandSystem(CvarSystem& cvars);
	// handler may be null
	void AddCommand(std::string name, std::function<void(CommandArgs&)> handler = nullptr);
	#if 0
	template <typename F>
	void AddCommand(std::string name, F&& f) {
		AddCommand(std::move(name), std::move(f));
	}
	#endif
	void RemoveCommand(const char *name);
	bool ShouldWait();
	// A complete command line has been parsed, so try to execute it
	void ExecuteString(const char *text);
	void CommandCompletion(void (*callback)(const char *cmd));
	template <typename F>
	void Iter(F&& f) {
		for (auto& pair : m_table) {
			const char *name = pair.first.c_str();
			f(name);
		}
	}
};