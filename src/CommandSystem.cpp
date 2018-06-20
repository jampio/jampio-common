#include "CommandSystem.h"
#include "com.h"
#include "com_vars.h"
#include "interface/server.h"
#include "interface/client.h"
#include "interface/ui.h"
#include "CommandArgs.h"

CommandSystem::CommandSystem(CvarSystem& cvars)
	: m_cvars(cvars)
	, m_wait(0)
	, m_table()
{}

bool CommandSystem::ShouldWait() {
	if (m_wait > 0) {
		m_wait--;
		return true;
	}
	return false;
}

void CommandSystem::AddCommand(std::string name, std::function<void(CommandArgs&)> handler) {
	auto search = m_table.find(name);
	if (search == m_table.end()) {
		m_table.insert(std::make_pair(std::move(name), std::move(handler)));
	} else {
		// allow completion-only commands to be silently doubled
		if (search->second) {
			Com_Printf("Command::AddCommand: %s already defined\n", name.c_str());
		}
	}
}

void CommandSystem::RemoveCommand(const char *name) {
	m_table.erase(name);
}

void CommandSystem::ExecuteString(const char *text) {
	// execute the command line
	auto args = CommandArgs::TokenizeString(text);
	if (!args.Argc()) {
		return;		// no tokens
	}

	auto search = m_table.find(args.Argv(0));
	if (search != m_table.end()) {
		if (search->second) {
			return search->second(args);
		}
	}
	
	// check cvars
	if (m_cvars.Command(args)) {
		return;
	}

	// check client game commands
	if ( com_cl_running && com_cl_running->integer() && CL_GameCommand() ) {
		return;
	}

	// check server game commands
	if ( com_sv_running && com_sv_running->integer() && SV_GameCommand() ) {
		return;
	}

	// check ui commands
	if ( com_cl_running && com_cl_running->integer() && UI_GameCommand() ) {
		return;
	}

	// send it as a server command if we are connected
	// this will usually result in a chat message
	CL_ForwardCommandToServer ( text );
}

void CommandSystem::CommandCompletion(void (*callback)(const char *cmd)) {
	for (auto& pair : m_table) {
		callback(pair.first.c_str());
	}
}