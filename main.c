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
	u8	*pHeaderBuf = NULL;
	u8	*pDataBuf = NULL;
	u32	nDataSz;
	double	*pElev = NULL;
	FILE	*pGpxFile = NULL;

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
	pHeader = Load_Header(pFnHeader, &pHeaderBuf);

	if (pHeader == NULL) {
	  goto _err0;
	}
	time_t	nTimeStamp;
	struct tm sTime;

	memset(&sTime, 0, sizeof(struct tm));
	sTime.tm_min = pHeader->nMinutes;
	sTime.tm_hour = pHeader->nHours;
	sTime.tm_mday = pHeader->nDay;
	sTime.tm_mon = pHeader->nMonth - 1;		// (range: 0 to 11).
	sTime.tm_year = (2000 + pHeader->nYear) - 1900;
	sTime.tm_isdst = -1;

	if ((nTimeStamp = mktime(&sTime)) == -1) {
	  fprintf(stderr, "mktime() error.\n");
	  goto _err0;
	}

	// *** Data ***
	if (pHeader->nGPSOff == 0)
	{

	printf("*** GPS data ***\n");

	// Load data file (OMD).
	FileLoad(pFnData, &pDataBuf, &nDataSz);
	// Check: Data size ok?
	if ( ((nDataSz / sizeof(struct SDataRecordType)) * sizeof(struct SDataRecordType)) != nDataSz)
	{
		fprintf(stderr, "OMD: Error! Incorrect data size.\n");
		goto _err0;
	}
	u32	nTotalBlks = nDataSz / sizeof(struct SDataRecordType);

	// Check: Number of GPS points in OMD ok?
	struct SDataRecordType	*pData = (struct SDataRecordType *)pDataBuf;
	u32	nRemainingBlks = nTotalBlks;
	u32	nGPSPtNo = 0;
	//
	while (nRemainingBlks)
	{
		switch (pData->nType)
		{
		case e_OMD_GPS_RECORD:
			nGPSPtNo++;
			break;

		default:
			break;
		}
		pData++;
		nRemainingBlks--;
	}
	if (nGPSPtNo != pHeader->nGPSPoints)
	{
		fprintf(stderr, "OMD: Error! Incorrect GPS points number. Found:%d, expected:%d.\n", nGPSPtNo, pHeader->nGPSPoints);
		goto _err0;
	}

	// GPX stuff.
	if (gOptions.nOptions & e_OPT_GPXFile)
	{
		// Open output file.
		if ((pGpxFile = fopen(pFnGpx, "w")) == NULL)
		{
			fprintf(stderr, "GPX: Error creating '%s'. Aborted.\n", pFnGpx);
			goto _err0;
		}
		// GPX header.
		fprintf(pGpxFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		fprintf(pGpxFile, "<gpx version=\"1.1\" creator=\"OMX2GPX\" xmlns=\"http://www.topografix.com/GPX/1/1\">\n");
		fprintf(pGpxFile, "\t<metadata>\n\t\t<author>\n\t\t\t<name>OMX2GPX</name>\n\t\t</author>\n\t</metadata>\n");
		fprintf(pGpxFile, "\t<trk>\n\t\t<trkseg>\n");
	}

	pData = (struct SDataRecordType *)pDataBuf;
#ifdef ELEVATION
	if (gOptions.nOptions & e_OPT_Elevation)
	{
		// Memory allocation for elevations. (+1 for possible zero point, though there should be more than enough blocks with the F2 records).
		if ((pElev = (double *)malloc((nTotalBlks + 1) * sizeof(double))) == NULL)
		{
			fprintf(stderr, "Elevation: Error allocating memory.\n");
			goto _err0;
		}
		// Clear array.
		u32	i;
		for (i = 0; i < nTotalBlks + 1; i++) *(pElev + i) = 0.0;
		// Retrieve elevations.
		AltitudeGet(pData, nDataSz, pElev);
	}
#endif

	// Expected data: F1/F1/F2 / F1/F1/F2 / ... Last sequence can be either F1/F1/F2 or F1/F2.
	struct SDataRecordGPS	*pSeqGPS0 = NULL;
	struct SDataRecordGPS	*pSeqGPS1 = NULL;
	struct SDataRecordCurve	*pSeqCurve = NULL;
	u32	nSeqGPS = 0;
	u32	nSeqCurve = 0;

	nGPSPtNo = 0;		// GPS points counter.
	// Zero point?
	if (gOptions.nOptions & e_OPT_ZeroPt)
	{
		Point_Display(nGPSPtNo, nTimeStamp, &gOptions.sZeroGPS, &gOptions.sZeroCurve, pElev, pGpxFile);
	}
	nGPSPtNo++;			// Counter start at 0 if zero point is provided, 1 if not.

	u32	nAbsRecNo = 0;	// Record number in OMD (all types), absolute, for debugging.
	nRemainingBlks = nTotalBlks;
	while (nRemainingBlks)
	{
		switch (pData->nType)
		{
		case e_OMD_GPS_RECORD:
			if (nSeqGPS == 0)
			{
				pSeqGPS0 = (struct SDataRecordGPS *)pData;
			}
			else if (nSeqGPS == 1)
			{
				pSeqGPS1 = (struct SDataRecordGPS *)pData;
			}
			else
			{
				fprintf(stderr, "OMD_GPS_RECORD: Unexpected block order! Aborted.\n");
				fprintf(stderr, "\t(debug: r=%d g=%d c=%d)\n", nAbsRecNo, nSeqGPS, nSeqCurve);
				goto _err0;
			}
			nSeqGPS++;
			break;

		case e_OMD_CURVE_RECORD:
			if (nSeqCurve == 0)
			{
				pSeqCurve = (struct SDataRecordCurve *)pData;
			}
			else
			{
				fprintf(stderr, "OMD_CURVE_RECORD: Unexpected block order! Aborted.\n");
				fprintf(stderr, "\t(debug: r=%d g=%d c=%d)\n", nAbsRecNo, nSeqGPS, nSeqCurve);
				goto _err0;
			}
			nSeqCurve++;
			break;

		default:
			fprintf(stderr, "Unknown data block! (debug: Type: %02X, record no: %d)\n", pData->nType, nAbsRecNo);
			break;
		}
		pData++;
		nAbsRecNo++;
		nRemainingBlks--;

		// Des infos à traiter ? (=> F1/F1/F2)
		if ((nSeqGPS == 2 && nSeqCurve == 1) || nRemainingBlks == 0)
		{
			if (pSeqGPS0 == NULL || pSeqCurve == NULL)
			{
				fprintf(stderr, "Shit happened...\n");
				goto _err0;
			}

			// Point 0.
			Point_Display(nGPSPtNo, nTimeStamp, pSeqGPS0, &pSeqCurve->sHalf0, pElev + nGPSPtNo, pGpxFile);
			nGPSPtNo++;
			// Point 1 (if exists).
			if (pSeqGPS1 != NULL)
			{
				Point_Display(nGPSPtNo, nTimeStamp, pSeqGPS1, &pSeqCurve->sHalf1, pElev + nGPSPtNo, pGpxFile);
				nGPSPtNo++;
			}

			// Reset ptrs and stuff.
			pSeqGPS0 = NULL;
			pSeqGPS1 = NULL;
			pSeqCurve = NULL;
			nSeqGPS = 0;
			nSeqCurve = 0;
		}

	}

	// GPX stuff.
	if (gOptions.nOptions & e_OPT_GPXFile)
	{
		// GPX's final tag.
		fprintf(pGpxFile, "\t\t</trkseg>\n\t</trk>\n</gpx>\n");
		printf("\n'%s' generated.\n", pFnGpx);
	}

	}	// endif GPS on


	// Leaving.
_err0:
	if (pHeaderBuf != NULL) free(pHeaderBuf);
	if (pDataBuf != NULL) free(pDataBuf);
#ifdef ELEVATION
	if (pElev != NULL) free(pElev);
#endif
	if (pGpxFile != NULL) fclose(pGpxFile);
	return (0);

}



