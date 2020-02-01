#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "ctypes.h"
#include "format.h"
#include "core_header.h"
#include "core_data.h"

#define MAX_LEN 400
char buffer[MAX_LEN+1] = {0};

int wrap(char *pFilename, char *pInputDir, char *pOutputDir) {
  char pFnHeader[256];	// Header file name.
  char pFnData[256];	// Data file name.
  char pFnGpx[256];		// GPX file name (output).

  strcpy(pFnHeader, pInputDir);
  strcat(pFnHeader, pFilename);

  strcpy(pFnData, pFnHeader);
  
  strcpy(pFnGpx, pOutputDir);
  strcat(pFnGpx, pFilename);

  strcat(pFnHeader, ".OMH");
  strcat(pFnData, ".OMD");
  strcat(pFnGpx, ".gpx");

  struct SHeader	*pHeader;
  int errorCode = Load_Header(pFnHeader, &pHeader);

  if (errorCode != 0) {
	return errorCode;
  }

  // Means no GPS in data?
  if (pHeader->nGPSOff != 0) {
	printf("GPSOff\n");
	free(pHeader);
	return NO_ERROR;
  }

  // Load data file (OMD).
  return Load_Data(pFnData, pFnGpx, pHeader);
}

char *timestamp(char *filename) {
  char pFnHeader[256];	// Header file name.

  strcpy(pFnHeader, filename);
  strcat(pFnHeader, ".OMH");

  struct SHeader	*pHeader;
  int errorCode = Load_Header(pFnHeader, &pHeader);

  if (errorCode == NO_ERROR) {
	u32	nHours, nMins, nSecs;
	nHours = pHeader->nTime / 3600;
	nMins = (pHeader->nTime / 60) % 60;
	nSecs = pHeader->nTime % 60;
	
	sprintf(buffer,
			"%02d-%02d-%04d %02d:%02d (%02d:%02d'%02d\" %.02f km)",
			pHeader->nDay, pHeader->nMonth, (2000 + pHeader->nYear),
			pHeader->nHours, pHeader->nMinutes,
			nHours, nMins, nSecs,
			(double)pHeader->nDistance / 1000);
  } else {
	strcpy(buffer, "Invalid data in header");
  }
  return buffer;
}

char *convert(char *filename, char *inputdir, char *outputdir) {
  int out_pipe[2];
  int saved_stdout;
  char errorValue[32] = {0};
	
  memset(&gOptions, 0, sizeof(struct SOptions));
  gOptions.nOptions = e_OPT_DefaultOptions;

  saved_stdout = dup(STDOUT_FILENO);
  if( pipe(out_pipe) != 0 ) {
    exit(1);
  }

  dup2(out_pipe[1], STDOUT_FILENO);
  close(out_pipe[1]);

  sprintf(errorValue, "\nERROR CODE: %d\n", wrap(filename, inputdir, outputdir));
  
  fflush(stdout);

  // Non-blocking fd
  int flags = fcntl(out_pipe[0], F_GETFL, 0);
  fcntl(out_pipe[0], F_SETFL, flags | O_NONBLOCK);
  
  if (read(out_pipe[0], buffer, MAX_LEN - 40) < 3) {
	strcpy(buffer, "No output from omx2gpx");
  }

  dup2(saved_stdout, STDOUT_FILENO);

  strcat(buffer, errorValue);
  return buffer;
}
