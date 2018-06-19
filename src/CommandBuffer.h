#pragma once

#include <jampio/shared/shared.h>
#include <string>
#include "CommandSystem.h"

class CommandBuffer {
private:
	CommandSystem& m_cmd;
	std::string m_buffer;
public:
	CommandBuffer(CommandSystem& cmd);
	// Adds command text at the end of the buffer, does NOT add a final \n
	void AddText(const char *text);
	// Adds command text immediately after the current command
	// Adds a \n to the text
	void InsertText(const char *text);
	void ExecuteText(cbufExec_t exec, const char *text);
	void Execute();
};