#include "stringed_interface.h"
#include "stringed_ingame.h"
#include "fs.h"

// this just gets the binary of the file into memory, so I can parse it. Called by main SGE loader
//
//  returns either char * of loaded file, else NULL for failed-to-open...
//
unsigned char *SE_LoadFileData(const char *psFileName, int *piLoadedLength /* = 0 */)
{
	unsigned char *psReturn = NULL;
	if ( piLoadedLength )
	{
		*piLoadedLength = 0;
	}
	{
		// local filename, so prepend the base dir etc according to game and load it however (from PAK?)
		//		
		unsigned char *pvLoadedData;		
		int iLen = FS_ReadFile( psFileName, (void **)&pvLoadedData );

		if (iLen>0)
		{
			psReturn = pvLoadedData;
			if ( piLoadedLength )
			{
				*piLoadedLength = iLen;
			}
		}
	}

	return psReturn;
}


// called by main SGE code after loaded data has been parsedinto internal structures...
//
void SE_FreeFileDataAfterLoad( unsigned char *psLoadedFile )
{
	if ( psLoadedFile )
	{
		FS_FreeFile( psLoadedFile );
	}	
}

// quake-style method of doing things since their file-list code doesn't have a 'recursive' flag...
//
int giFilesFound;
static void SE_R_ListFiles( const char *psExtension, const char *psDir, std::string &strResults )
{
//	Com_Printf(va("Scanning Dir: %s\n",psDir));

	char	**sysFiles, **dirFiles;
	int		numSysFiles, i, numdirs;

	dirFiles = FS_ListFiles( psDir, "/", &numdirs);
	for (i=0;i<numdirs;i++)
	{
		if (dirFiles[i][0] && dirFiles[i][0] != '.')	// skip blanks, plus ".", ".." etc
		{
			char	sDirName[MAX_QPATH];
			sprintf(sDirName, "%s/%s", psDir, dirFiles[i]);
			//
			// for some reason the quake filesystem in this game now returns an extra slash on the end,
			//	didn't used to. Sigh...
			//
			if (sDirName[strlen(sDirName)-1] == '/')
			{
				sDirName[strlen(sDirName)-1] = '\0';
			}
			SE_R_ListFiles( psExtension, sDirName, strResults );
		}
	}

	sysFiles = FS_ListFiles( psDir, psExtension, &numSysFiles );
	for(i=0; i<numSysFiles; i++)
	{
		char	sFilename[MAX_QPATH];
		sprintf(sFilename,"%s/%s", psDir, sysFiles[i]);		
			
//		Com_Printf("%sFound file: %s",!i?"\n":"",sFilename);

		strResults += sFilename;
		strResults += ';';
		giFilesFound++;

		// read it in...
		//
/*		byte *pbData = NULL;
		int iSize = FS_ReadFile( sFilename, (void **)&pbData);

		if (pbData)
		{

			FS_FreeFile( pbData );
		}
*/
	}
	FS_FreeFileList( sysFiles );
	FS_FreeFileList( dirFiles );
}


// replace this with a call to whatever your own code equivalent is.
//
// expected result is a ';'-delineated string (including last one) containing file-list search results
//
int SE_BuildFileList(const char *psStartDir, std::string &strResults)
{
	giFilesFound = 0;
	strResults = "";

	SE_R_ListFiles(sSE_INGAME_FILE_EXTENSION, psStartDir, strResults);

	return giFilesFound;
}
