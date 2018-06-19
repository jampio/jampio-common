#pragma once

#include <functional>
#include <map>
#include <string>

class CommandSystem {
private:
	int m_wait;
	std::map<std::string, std::function<void()>> m_table;
public:
	CommandSystem();
	// handler may be null
	void AddCommand(std::string name, std::function<void()> handler = nullptr);
	void RemoveCommand(const char *name);
	bool ShouldWait();
	// A complete command line has been parsed, so try to execute it
	void ExecuteString(const char *text);
	void CommandCompletion(void (*callback)(const char *cmd));
};