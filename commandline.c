#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "ctypes.h"
#include "format.h"
#include "elevation.h"
#include "main.h"

void Options_Display()
{
  	// Display options.
	if (gOptions.nOptions != e_OPT_DefaultOptions)
	{
		if (gOptions.nOptions & e_OPT_HeaderOnly) printf("=> Display header only.\n");
		if ((gOptions.nOptions & e_OPT_GPXFile) == 0) printf("=> Do not generate GPX file.\n");
		if ((gOptions.nOptions & e_OPT_GPSDataDisplay) == 0) printf("=> Do not display GPS data.\n");
#ifdef ELEVATION
		if (gOptions.nOptions & e_OPT_Elevation) printf("=> Retrieve elevation (Google Maps Elevation API).\n");
#endif
		if (gOptions.nOptions & e_OPT_ZeroPt)
		{
			printf("=> Insert zero point (lat=%f,lon=%f",
				(double)gOptions.sZeroGPS.nLatitude / 1000000, (double)gOptions.sZeroGPS.nLongitude / 1000000);
			if (gOptions.sZeroCurve.nSpeed) printf(",spd=%.02f", (double)gOptions.sZeroCurve.nSpeed / 100);
			if (gOptions.sZeroCurve.nHeartRate) printf(",hr=%d", gOptions.sZeroCurve.nHeartRate);
			printf(").\n");
		}
		printf("\n");
	}
}

// Show help.
void Help_Display(void)
{
	printf("Usage: OMX2GPX [options] <ACT file name, without extension>\n\n");
	printf("Options:\n");
	printf("\t-h: Display help.\n");
	printf("\t-r: Display header only.\n");
	printf("\t-d: Do not display GPS data.\n");
	printf("\t-g: Do not generate GPX file.\n");
#ifdef ELEVATION
	printf("\t-e: Retrieve elevation (Google Maps Elevation API).\n");
#endif
	printf("\t-z:lat:lon[:spd[:hr]] Provide zero point information.\n");
	printf("\t    lat & lon: up to 6 decimal places. spd: up to 2 decimal places.\n");
	printf("\n");

	exit(1);
}

// Command line arguments check.
void CommandLine_Check(int nArgc, char **ppArgv, char **ppFilename)
{
	u32	nCur = 1;
	char	*pStr;

	while (nCur < (u32)nArgc)
	{
		pStr = *(ppArgv + nCur);	// Ptr sur l'arg en cours.

		if (*pStr == '-')
		{
			// Argument starts with '-'.
			pStr++;		// Skips the '-'.
			switch(*pStr)
			{
			// -h: Display help.
			case 'h':
			case 'H':
			case '?':
				Help_Display();
				break;

			// -r: Display header only.
			case 'r':
			case 'R':
				gOptions.nOptions |= e_OPT_HeaderOnly;
				break;

			// -g: Do not generate GPX file.
			case 'g':
			case 'G':
				gOptions.nOptions &= ~e_OPT_GPXFile;
				break;

			// -d: Do not display GPS data.
			case 'd':
			case 'D':
				gOptions.nOptions &= ~e_OPT_GPSDataDisplay;
				break;

#ifdef ELEVATION
			// -e: Retrieve elevation (Google Maps Elevation API).
			case 'e':
			case 'E':
				gOptions.nOptions |= e_OPT_Elevation;
				break;
#endif

			// -z:lat:lon[:spd[:hr]] Provide zero point information.
			case 'z':
			case 'Z':
				{
					double	nLat = 0.0, nLon = 0.0, nSpd = 0.0;
					u32	nHr = 0;
					u32	n;
					n = sscanf(pStr, "%*c:%lf:%lf:%lf:%d", &nLat, &nLon, &nSpd, &nHr);
					if (n < 2)
					{
						fprintf(stderr, "CommandLine_Check(): z: Could not read parameters.\n");
						exit(1);
					}
					// lat: -90.0 <= value <= 90.0
					if (nLat < -90.0 || nLat > 90.0)
					{
						fprintf(stderr, "CommandLine_Check(): z: Latitude should be in the [-90.0;90.0] interval.\n");
						exit(1);
					}
					// lon: -180.0 <= value < 180.0
					if (nLon < -180.0 || nLon > 180.0)
					{
						fprintf(stderr, "CommandLine_Check(): z: Longitude should be in the [-180.0;180.0] interval.\n");
						exit(1);
					}

					// Save info.
					gOptions.sZeroGPS.nLatitude = (s32)TRUNC(nLat * 1000000);
					gOptions.sZeroGPS.nLongitude = (s32)TRUNC(nLon * 1000000);
					if (n >= 3) gOptions.sZeroCurve.nSpeed = (u32)TRUNC(nSpd * 100);
					if (n >= 4) gOptions.sZeroCurve.nHeartRate = nHr;
					gOptions.nOptions |= e_OPT_ZeroPt;
				}
				break;

			default:
				fprintf(stderr, "CommandLine_Check(): Unknown parameter: '%s'.\n", *(ppArgv + nCur));
				break;
			}
		}
		else
		{
			// Argument doesn't start with '-', it's a file name.
			*ppFilename = pStr;
		}

		nCur++;		// Next arg.
	}

}
