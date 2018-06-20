#pragma once
/*
============================================================================

COMMAND LINE FUNCTIONS

+ characters seperate the commandLine string into multiple console
command lines.

All of these are valid:

quake3 +set test blah +map test
quake3 set test blah+map test
quake3 set test blah + map test

============================================================================
*/
#include <vector>
#include <string>
#include "CommandBuffer.h"
#include "CvarSystem.h"

class CommandLine {
private:
	std::vector<std::string> m_lines;
	CommandBuffer& m_cbuf;
	CvarSystem& m_cvars;
public:
	CommandLine(CommandBuffer& cbuf, CvarSystem& cvars, int argc, const char **argv);
	/**
	 * Check for "safe" on the command line, which will
	 * skip loading of jampconfig.cfg
	 */
	bool SafeMode();
	/**
	 * Searches for command line parameters that are set commands.
	 * If match is not NULL, only that cvar will be looked for.
	 * That is necessary because cddir and basedir need to be set
	 * before the filesystem is started, but all other sets shouls
	 * be after execing the config and default.
	 */
	void StartupVariable(const char *match);
	/**
	 * Adds command line parameters as script statements
	 * Commands are seperated by + signs
	 * Returns qtrue if any late commands were added, which
	 * will keep the demoloop from immediately starting
	 */
	bool AddStartupCommands();
};