/* ---------- ---------- ---------- ---------- ---------- ---------- ----------
   OMX2GPX - A simple Geonaute OnMove 220 GPS watch OMH/OMD to GPX converter.
   Code: Copyright (C) 2016 Clement CORDE - c1702@yahoo.com
   Project started on 2016/10/08.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
   ---------- ---------- ---------- ---------- ---------- ---------- ---------- */

// Includes.
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ctypes.h"
#include "format.h"
#include "elevation.h"
#include "main.h"

// Main program.
int main(int argc, char **argv)
{
  char	*pFilename = NULL;	// ptr on filename in args.

  char pFnHeader[256];	// Header file name.
  char pFnData[256];		// Data file name.
  char pFnGpx[256];		// GPX file name (output).


  // Tool name.
  printf("*** OMX2GPX v%d.%02d, Geonaute OnMove 220 OMH/OMD to GPX.\n", VER_MAJOR, VER_MINOR);
  printf("*** By Clement CORDE - c1702@yahoo.com\n\n");
  // Reset options.
  memset(&gOptions, 0, sizeof(struct SOptions));
  gOptions.nOptions = e_OPT_DefaultOptions;
  // Do we have arguments?
  if (argc < 2)
	{
	  Help_Display();
	  exit(1);
	}

  // Check command line's arguments.
  CommandLine_Check(argc, argv, &pFilename);

  // Do we have a filename?
  if (pFilename == NULL)
	{
	  printf("*** FATAL: No filename given!\n\n");
	  Help_Display();
	  exit(1);
	}
  // Create filenames.
  strncpy(pFnHeader, pFilename, sizeof(pFnHeader) - 4);
  strncpy(pFnData, pFilename, sizeof(pFnData) - 4);
  strncpy(pFnGpx, pFilename, sizeof(pFnGpx) - 4);
  strcat(pFnHeader, ".OMH");
  strcat(pFnData, ".OMD");
  strcat(pFnGpx, ".gpx");

  Options_Display();

  // *** Header ***
  // Load header (OMH).
  struct SHeader	*pHeader;
  pHeader = Load_Header(pFnHeader, &pHeader);

  if (pHeader == NULL) {
	return -1;
  }
	
  // *** Data ***
  if (pHeader->nGPSOff != 0) {
	free(pHeader);
	return 0;
  }

  // Load data file (OMD).
  printf("*** GPS data ***\n");
  Load_Data(pFnData, pFnGpx, &pHeader);

  return 0;
}



