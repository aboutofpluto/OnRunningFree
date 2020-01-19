#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ctypes.h"

void onError(FILE *pFile, u8 **ppBuf) {
  if (pFile != NULL) fclose(pFile);
  if (*ppBuf != NULL) free(*ppBuf);
}

// Load file.
// Out: malloc and load file into ppBuf, return file size through pnSz.
void FileLoad(char *pFilename, u8 **ppBuf, u32 *pnSz)
{
	FILE	*pFile = NULL;
	*ppBuf = NULL;

	// Open file.
	pFile = fopen(pFilename, "rb");
	if (pFile == NULL) {
		fprintf(stderr, "FileLoad(): Error opening file '%s'.\n", pFilename);
		onError(pFile, ppBuf);
		return;
	}
	// Retrieve file size.
	fseek(pFile, 0, SEEK_END);
	*pnSz = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	// Malloc.
	if ((*ppBuf = (u8 *)malloc(*pnSz)) == NULL)
	{
		fprintf(stderr, "FileLoad(): Error allocating memory.\n");
		onError(pFile, ppBuf);
		return;
	}
	// Loading.
	u32	nSz;
	nSz = fread(*ppBuf, 1, *pnSz, pFile);
	fclose(pFile); pFile = NULL;
	// Size check.
	if (nSz != *pnSz)
	{
		fprintf(stderr, "FileLoad(): Read error! %d bytes expected, %d bytes read.\n", *pnSz, nSz);
		onError(pFile, ppBuf);
		return;
	}

	return;
}
