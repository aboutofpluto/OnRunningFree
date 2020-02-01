#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "ctypes.h"
#include "format.h"

// Display GPS point info.
void Point_Display(u32 nGPSPtNo, time_t nTimeStamp, struct SDataRecordGPS *pGPS, struct SHalfCurve *pCurve, double *pElev, FILE *pGpxFile)
{
	if (pGPS->nTime != pCurve->nTime) printf("ERROR! GPS time: %d - Curve time: %d\n", pGPS->nTime, pCurve->nTime);

	nTimeStamp += pGPS->nTime;				// Add GPS' point time to starting time.

	struct tm *sTime2;
	tzset();
	sTime2 = localtime(&nTimeStamp);		// Retrieve date and time from current timestamp.

	// Display info.
	if (gOptions.nOptions & e_OPT_GPSDataDisplay)
	{
		printf("%d - Lat: %f - Long: %f - ",
			nGPSPtNo,
			(double)pGPS->nLatitude / 1000000, (double)pGPS->nLongitude / 1000000);
#ifdef ELEVATION
		if (gOptions.nOptions & e_OPT_Elevation)
			printf("Ele: %f - ", *pElev);
#endif
		printf("Dist: %dm - Time: %02d:%02d'%02d\" (%ds) - Speed: %.02fkm/h - KCal: %d - Heart: ",
			pGPS->nDistance,
			sTime2->tm_hour, sTime2->tm_min, sTime2->tm_sec, pGPS->nTime,
			(double)pCurve->nSpeed / 100,
			pCurve->nKCal
			);
		if (pCurve->nHeartRate) printf("%d", pCurve->nHeartRate); else printf("N/A");
		printf("\n");
	}

	// Save info into GPX.
	if (gOptions.nOptions & e_OPT_GPXFile)
	{
		// Base values: lat, lon, time and speed.
		fprintf(pGpxFile, "\t\t<trkpt lat=\"%f\" lon=\"%f\">\n",
			(double)pGPS->nLatitude / 1000000, (double)pGPS->nLongitude / 1000000);
#ifdef ELEVATION
		if (gOptions.nOptions & e_OPT_Elevation)
			fprintf(pGpxFile, "\t\t\t<ele>%f</ele>\n", *pElev);
#endif
		fprintf(pGpxFile, "\t\t\t<time>%04d-%02d-%02dT%02d:%02d:%02dZ</time>\n",
			sTime2->tm_year + 1900, sTime2->tm_mon + 1, sTime2->tm_mday, sTime2->tm_hour, sTime2->tm_min, sTime2->tm_sec);
		fprintf(pGpxFile, "\t\t\t<speed>%.02f</speed>\n", (double)pCurve->nSpeed / 100);

		// Double tests, in case other info has to be included in the <extensions> tag.
		if (pCurve->nHeartRate != 0)
		{
			fprintf(pGpxFile, "\t\t\t<extensions>\n\t\t\t\t<gpxtpx:TrackPointExtension>\n");
			if (pCurve->nHeartRate != 0)
				fprintf(pGpxFile, "\t\t\t\t\t<gpxtpx:hr>%d</gpxtpx:hr>\n", pCurve->nHeartRate);
			fprintf(pGpxFile, "\t\t\t\t</gpxtpx:TrackPointExtension>\n\t\t\t</extensions>\n");
		}
		fprintf(pGpxFile, "\t\t</trkpt>\n");
	}

}
