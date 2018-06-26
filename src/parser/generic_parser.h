#pragma once

constexpr auto MAX_TOKEN_SIZE = 1024;

char *GP_GetToken(char **text, bool allowLineBreaks, bool readUntilEOL = false);