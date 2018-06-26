#include <cstring>
#include "generic_parser.h"

static char	token[MAX_TOKEN_SIZE];

char *GP_GetToken(char **text, bool allowLineBreaks, bool readUntilEOL) {
	char	*pointer = *text;
	int		length = 0;
	int		c = 0;
	bool	foundLineBreak;

	token[0] = 0;
	if (!pointer)
	{
		return token;
	}

	while(1)
	{
		foundLineBreak = false;
		while(1)
		{
			c = *pointer;
			if (c > ' ')
			{
				break;
			}
			if (!c)
			{
				*text = 0;
				return token;
			}
			if (c == '\n')
			{
				foundLineBreak = true;
			}
			pointer++;
		}
		if (foundLineBreak && !allowLineBreaks)
		{
			*text = pointer;
			return token;
		}

		c = *pointer;

		// skip single line comment
		if (c == '/' && pointer[1] == '/')
		{
			pointer += 2;
			while (*pointer && *pointer != '\n') 
			{
				pointer++;
			}
		}
		// skip multi line comments
		else if (c == '/' && pointer[1] == '*') 
		{
			pointer += 2;
			while (*pointer && (*pointer != '*' || pointer[1] != '/')) 
			{
				pointer++;
			}
			if (*pointer) 
			{
				pointer += 2;
			}
		}
		else
		{	// found the start of a token
			break;
		}
	}

	if (c == '\"')
	{	// handle a string
		pointer++;
		while (1)
		{
			c = *pointer++;
			if (c == '\"')
			{
//				token[length++] = c;
				break;
			}
			else if (!c)
			{
				break;
			}
			else if (length < MAX_TOKEN_SIZE)
			{
				token[length++] = c;
			}
		}
	}
	else if (readUntilEOL)
	{
		// absorb all characters until EOL
		while(c != '\n' && c != '\r')
		{
			if (c == '/' && ((*(pointer+1)) == '/' || (*(pointer+1)) == '*'))
			{
				break;
			}

			if (length < MAX_TOKEN_SIZE)
			{
				token[length++] = c;
			}
			pointer++;
			c = *pointer;
		}
		// remove trailing white space
		while(length && token[length-1] < ' ')
		{
			length--;
		}
	}
	else
	{
		while(c > ' ')
		{
			if (length < MAX_TOKEN_SIZE)
			{
				token[length++] = c;
			}
			pointer++;
			c = *pointer;
		}
	}

	if (token[0] == '\"')
	{	// remove start quote
		length--;
		memmove(token, token+1, length);

		if (length && token[length-1] == '\"')
		{	// remove end quote
			length--;
		}
	}

	if (length >= MAX_TOKEN_SIZE)
	{
		length = 0;
	}
	token[length] = 0;
	*text = (char *)pointer;

	return token;
}
