#include <cstring>
#include <algorithm>
#include "CommandBuffer.h"
#include "com.h"

CommandBuffer::CommandBuffer(CommandSystem& cmd) 
	: m_cmd(cmd)
	, m_buffer()
{}

void CommandBuffer::AddText(const char *text) {
	m_buffer += text;
}

void CommandBuffer::InsertText(const char *text) {
	m_buffer.insert(0, "\n");
	m_buffer.insert(0, text);
}

void CommandBuffer::ExecuteText(cbufExec_t exec, const char *text) {
	switch (exec) {
		case EXEC_NOW:
			if (text && strlen(text) > 0) {
				m_cmd.ExecuteString(text);
			} else {
				Execute();
			}
			break;
		case EXEC_INSERT:
			InsertText(text);
			break;
		case EXEC_APPEND:
			AddText(text);
			break;
		default:
			Com_Error(ERR_FATAL, "CommandBuffer::ExecuteText: bad exec_when");
	}
}

void CommandBuffer::Execute() {
	while (!m_buffer.empty()) {
		if (m_cmd.ShouldWait()) break;
		int i, quotes;
		for (i = 0, quotes = 0; i < m_buffer.size(); i++) {
			if (m_buffer[i] == '"')
				quotes++;
			if (!(quotes&1) && m_buffer[i] == ';')
				break;	// don't break if inside a quoted string
			if (m_buffer[i] == '\n' || m_buffer[i] == '\r' )
				break;
		}
		auto cmd = m_buffer.substr(0, i);
		m_buffer.erase(0, cmd.size() + 1);
		m_cmd.ExecuteString(cmd.c_str());
	}
}