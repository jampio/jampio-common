#pragma once

#include <jampio/shared/shared.h>
#include <array>

class CommandArgs {
private:
	int cmd_argc;
	char *cmd_argv[MAX_STRING_TOKENS];
	char cmd_tokenized[BIG_INFO_STRING+MAX_STRING_TOKENS];
	CommandArgs();
	void internal_tokenize(const char *text_in);
public:
	// Parses the given string into command line tokens.
	// The text is copied to a seperate buffer and 0 characters
	// are inserted in the apropriate place, The argv array
	// will point into this temporary buffer.
	static CommandArgs TokenizeString(const char *text);
	int Argc() const;
	const char *Argv(int arg) const;
	void ArgvBuffer(int arg, char *buffer, int bufferLength);
	std::array<char, MAX_STRING_CHARS> Args() const;
};