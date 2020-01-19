#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ctypes.h"
#include "format.h"
#include "elevation.h"
#include "main.h"

void Free_All(u8 *pDataBuf, FILE *pGpxFile, double *pElev) {
  if (pGpxFile != NULL) fclose(pGpxFile);
  if (pDataBuf != NULL) free(pDataBuf);
  if (pElev != NULL) free(pElev);
}

void Load_Data(char *pFnData, char *pFnGpx, const struct SHeader *pHeader) {
  u8	*pDataBuf = NULL;
  FILE	*pGpxFile = NULL;
  u32		nDataSz;
  double	*pElev = NULL;
  time_t	nTimeStamp = Get_TimeStamp(pHeader);

  
  if (nTimeStamp == -1) {
	fprintf(stderr, "mktime() error.\n");
	Free_All(pDataBuf, pGpxFile, pElev);
	return;
  }

  FileLoad(pFnData, &pDataBuf, &nDataSz);

  // Check: Data size ok?
  if ( ((nDataSz / sizeof(struct SDataRecordType)) * sizeof(struct SDataRecordType)) != nDataSz)
	{
	  fprintf(stderr, "OMD: Error! Incorrect data size.\n");
	  Free_All(pDataBuf, pGpxFile, pElev);
	  return;
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
	  Free_All(pDataBuf, pGpxFile, pElev);
	  return;
	}

  // GPX stuff.
  if (gOptions.nOptions & e_OPT_GPXFile)
	{
	  // Open output file.
	  if ((pGpxFile = fopen(pFnGpx, "w")) == NULL)
		{
		  fprintf(stderr, "GPX: Error creating '%s'. Aborted.\n", pFnGpx);
		  Free_All(pDataBuf, pGpxFile, pElev);
		  return;
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
		  Free_All(pDataBuf, pGpxFile, pElev);
		  return;
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
			  Free_All(pDataBuf, pGpxFile, pElev);
			  return;
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
			  Free_All(pDataBuf, pGpxFile, pElev);
			  return;
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
			  Free_All(pDataBuf, pGpxFile, pElev);
			  return;
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

  Free_All(pDataBuf, pGpxFile, pElev);
  return;
}
