#pragma once

#include <string>

unsigned char *SE_LoadFileData(const char *psFileName, int *piLoadedLength = 0);
void SE_FreeFileDataAfterLoad(unsigned char *psLoadedFile);
int SE_BuildFileList(const char *psStartDir, std::string &strResults);
