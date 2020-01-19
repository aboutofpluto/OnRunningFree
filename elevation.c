#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ctypes.h"
#include "format.h"
#include "elevation.h"
#include "main.h"

#ifdef ELEVATION

#define	LN_SZ	(256)
#define	CUR_MAX_LN	(11+1+11+1)		// Single GPS record max string len. (11: 1 optinal minus sign, up to 3 digits, 1 point, up to 6 decimal places).

// HTTP API request.
int ApiRequest(char *pLocations, double *pElev, u32 nExpectedPoints)
{
	u32	nRetrievedPoints = 0;
	char	pHttpRequest[HTTP_REQ_MAX_LN];

	snprintf(pHttpRequest, HTTP_REQ_MAX_LN, "%s%s%s%s%s%s", HTTP_REQ_PART1, pLocations, HTTP_REQ_PART2, GOOGLE_MAPS_API_KEY, HTTP_REQ_PART3, HTTP_REQ_PART4);

	char	pLn[LN_SZ];
	FILE	*pFlux;

	if ((pFlux = popen(pHttpRequest, "r")) != NULL)
	{
		while (fgets(pLn, LN_SZ, pFlux) != NULL)
		{
			double	nEle;
			if (sscanf(pLn, "%lf", &nEle) != 1)
			{
				fprintf(stderr, "ApiRequest(): Problem while retrieving elevations. Elevations cancelled.\n");
				gOptions.nOptions &= ~e_OPT_Elevation;
				return (1);
			}
			*pElev++ = nEle;
			nRetrievedPoints++;
		}
		pclose(pFlux);
	}

	// Last check.
	if (nRetrievedPoints != nExpectedPoints)
	{
		fprintf(stderr, "ApiRequest(): Number of retrieved points (%d) != Number of expected points (%d). Elevations cancelled.\n", nRetrievedPoints, nExpectedPoints);
		gOptions.nOptions &= ~e_OPT_Elevation;
		return (1);
	}

	return (0);
}

// Get GPS' points via Google Maps API.
// (Grouped to limit the number of http requests (day limit is 2500)).
void AltitudeGet(struct SDataRecordType *pData, u32 nDataSz, double *pElev)
{
	char	pReqCoords[REQ_LOCATIONS_MAX_LN];

	u32	nRemainingBlks = nDataSz / sizeof(struct SDataRecordType);
	u32	nReqNo = 0;
	pReqCoords[0] = 0;
	u32	nStoIdx = 1;			// Storage index. Default pos is 1 (no zero point).

	printf("Retrieving elevation from Google Maps:\n");

	// Zero point?
	if (gOptions.nOptions & e_OPT_ZeroPt)
	{
		snprintf(pReqCoords, sizeof(pReqCoords), "%f,%f",
			(double)gOptions.sZeroGPS.nLatitude / 1000000, (double)gOptions.sZeroGPS.nLongitude / 1000000);
		nReqNo++;
		nStoIdx = 0;			// Storage index. Zero point => begin storage at pos 0.
	}

	while (nRemainingBlks)
	{
		switch (pData->nType)
		{
		case e_OMD_GPS_RECORD:
			{
				char	pCur[CUR_MAX_LN];
				struct SDataRecordGPS	*pRecGPS = (struct SDataRecordGPS *)pData;
				snprintf(pCur, sizeof(pCur), "%f,%f", (double)pRecGPS->nLatitude / 1000000, (double)pRecGPS->nLongitude / 1000000);

				if (nReqNo) strcat(pReqCoords, "|");
				nReqNo++;
				strcat(pReqCoords, pCur);

				// Current cluster complete?
				if (strlen(pReqCoords) + CUR_MAX_LN >= REQ_LOCATIONS_MAX_LN || nReqNo >= REQ_LOCATIONS_MAX_NB)
				{
					printf("Sending request for points %d to %d.\n", nStoIdx, nStoIdx + nReqNo - 1);
					if (ApiRequest(pReqCoords, pElev + nStoIdx, nReqNo)) return;
					nStoIdx += nReqNo;
					nReqNo = 0;
					pReqCoords[0] = 0;
				}
			}
			break;

		default:
			break;
		}
		pData++;
		nRemainingBlks--;
	}

	// Last cluster.
	if (nReqNo)
	{
		printf("Sending request for points %d to %d.\n", nStoIdx, nStoIdx + nReqNo - 1);
		if (ApiRequest(pReqCoords, pElev + nStoIdx, nReqNo)) return;
		nStoIdx += nReqNo;
	}

	printf("Done!\n");
}

#endif
