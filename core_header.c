#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ctypes.h"
#include "format.h"
#include "elevation.h"
#include "main.h"

struct SHeader *Load_Header(char *pFnHeader, u8 **pHeaderBuf)
{
    u32 nHeaderSz;
  
    FileLoad(pFnHeader, pHeaderBuf, &nHeaderSz);
	// Check: Header size ok?
	if (nHeaderSz != sizeof(struct SHeader))
	{
		fprintf(stderr, "OMH: Error! Incorrect header size.\n");
		return NULL;
	}

	struct SHeader	*pHeader = (struct SHeader *)pHeaderBuf;

	// Check: File numbers & magic numbers ok?
	if (!(pHeader->nFileNumber0 == pHeader->nFileNumber1 && 
		pHeader->nFileNumber1 == pHeader->nFileNumber2 &&
		pHeader->nMagicNumber1 == 0xFA && pHeader->nMagicNumber2 == 0xF0))
	{
		fprintf(stderr, "OMH: Wrong header!\n");
		return NULL;
	}

	printf("*** Session statistics ***\n");
	printf("Session: %02d/%02d/20%02d - %02d:%02d\n", pHeader->nDay, pHeader->nMonth, pHeader->nYear, pHeader->nHours, pHeader->nMinutes);
	printf("Distance: %.02f km (%d m)\n", (double)pHeader->nDistance / 1000, pHeader->nDistance);
	u32	nHours, nMins, nSecs;
	nHours = pHeader->nTime / 3600;
	nMins = (pHeader->nTime / 60) % 60;
	nSecs = pHeader->nTime % 60;
	printf("Time: %02d:%02d'%02d\" (%d s)\n", nHours, nMins, nSecs, pHeader->nTime);
	printf("Avg speed: %.01f km/h (%d0 m/h)\n", (double)pHeader->nSpeedAvg / 100, pHeader->nSpeedAvg);
	printf("Max speed: %.01f km/h (%d0 m/h)\n", (double)pHeader->nSpeedMax / 100, pHeader->nSpeedMax);

	u32	t = (pHeader->nTime * 1000) / pHeader->nDistance;
	//nHours = t / 3600;
	nMins = (t / 60) % 60;
	nSecs = t % 60;
	printf("Avg pace: %02d'%02d\"/km\n", nMins, nSecs);

	printf("KCal: %0d\n", pHeader->nKCal);
	printf("Avg heart rate: ");  if (pHeader->nHeartRateAvg) printf("%d bpm\n", pHeader->nHeartRateAvg); else printf("N/A\n");
	printf("Max heart rate: "); if (pHeader->nHeartRateMax) printf("%d bpm\n", pHeader->nHeartRateMax); else printf("N/A\n");

	printf("GPS points in OMD: %d\n", pHeader->nGPSPoints);
	printf("GPS: %s\n", (pHeader->nGPSOff == 0 ? "On" : "Off"));
	printf("\n");

	// Target.
	if (pHeader->nTargetMode)
	{
		// Display mode, min and max limits.
		char	*pTargetModeTxt[e_OMH_TARGET_Max] = { NULL, "Speed", "Heart Rate", "Pace" };
		switch (pHeader->nTargetMode)
		{
		case e_OMH_TARGET_HR:
			printf("Target mode: %s\n", pTargetModeTxt[pHeader->nTargetMode]);
			printf("Min: %d bpm - Max: %d bpm\n", pHeader->nTargetHeartRateMin, pHeader->nTargetHeartRateMax);
			break;

		case e_OMH_TARGET_SPEED:
			printf("Target mode: %s\n", pTargetModeTxt[pHeader->nTargetMode]);
			printf("Min: %.01f km/h - Max: %.01f km/h\n",
				(double)pHeader->nTargetSpeedMin / 100, (double)pHeader->nTargetSpeedMax / 100);
			break;

		case e_OMH_TARGET_PACE:
			printf("Target mode: %s\n", pTargetModeTxt[pHeader->nTargetMode]);
			{
				u32	nMinMins, nMinSecs, nMaxMins, nMaxSecs;
				nMinMins = (pHeader->nTargetSpeedMin / 60) % 60;
				nMinSecs = pHeader->nTargetSpeedMin % 60;
				nMaxMins = (pHeader->nTargetSpeedMax / 60) % 60;
				nMaxSecs = pHeader->nTargetSpeedMax % 60;
				printf("Min: %02d'%02d\"/km - Max: %02d'%02d\"/km\n", nMinMins, nMinSecs, nMaxMins, nMaxSecs);
			}
			break;

		default:
			printf("Target: Unhandled target value: %d\n", pHeader->nTargetMode);
			break;
		}
		// Time percentages.
		printf("%.02f%% <min< %.02f%% <max< %.02f%%\n",
			(100.0 * (double)pHeader->nTargetTimeBelow) / (double)pHeader->nTime,
			(100.0 * (double)pHeader->nTargetTimeIn) / (double)pHeader->nTime,
			(100.0 * (double)pHeader->nTargetTimeAbove) / (double)pHeader->nTime);
		printf("\n");
	}

	//	if (gOptions.nOptions & e_OPT_HeaderOnly) return NULL;

	return pHeader;
}
