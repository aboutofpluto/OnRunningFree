#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ctypes.h"
#include "format.h"
#include "core_header.h"
#include "core_data.h"

#define MAX_LEN 400
char buffer[MAX_LEN+1] = {0};

int wrap(char *pFilename, char *pOutputDir) {
  char pFnHeader[256];	// Header file name.
  char pFnData[256];	// Data file name.
  char pFnGpx[256];		// GPX file name (output).

  strncpy(pFnHeader, pFilename, sizeof(pFnHeader) - 4);
  strncpy(pFnData, pFilename, sizeof(pFnData) - 4);

  strcpy(pFnGpx, pOutputDir);
  strcat(pFnGpx, pFnData);

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
	free(pHeader);
	return NO_ERROR;
  }

  // Load data file (OMD).
  return Load_Data(pFnData, pFnGpx, pHeader);
}

char *pywrap(char *filename) {
  int out_pipe[2];
  int saved_stdout;
  
  char **argv = malloc(sizeof(char *) * 2);
  argv[0] = "py_wrapper";
  argv[1] = filename;

  saved_stdout = dup(STDOUT_FILENO);
  if( pipe(out_pipe) != 0 ) {
    exit(1);
  }

  dup2(out_pipe[1], STDOUT_FILENO);
  close(out_pipe[1]);
  
  wrap(filename, "/tmp/");

  fflush(stdout);
  if (read(out_pipe[0], buffer, MAX_LEN) == 0) {
	printf("No output from omx2gpx");
  }

  dup2(saved_stdout, STDOUT_FILENO);

  return buffer;
}
