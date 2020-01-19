#define	VER_MAJOR	1
#define	VER_MINOR	3

#define	TRUNC(x) ((x) >= 0 ? (x) + 0.5 : (x) - 0.5)

struct SOptions
{
	u8	nOptions;

	// Zero point data, if provided.
	struct SDataRecordGPS	sZeroGPS;
	struct SHalfCurve		sZeroCurve;
};
struct SOptions	gOptions;

enum
{
	e_OPT_HeaderOnly		= 1,		// Display header only.
	e_OPT_GPXFile			= 1 << 1,	// Generate GPX file.
	e_OPT_GPSDataDisplay	= 1 << 2,	// Display GPS data to screen.
	e_OPT_Elevation			= 1 << 3,	// Retrieve elevation.
	e_OPT_ZeroPt			= 1 << 4,	// 1 = Zero point information provided.

	e_OPT_DefaultOptions	= (e_OPT_GPSDataDisplay | e_OPT_GPXFile)
};


// Load file.
// Out: malloc and load file into ppBuf, return file size through pnSz.
void FileLoad(char *pFilename, u8 **ppBuf, u32 *pnSz);

// Show active options
void Options_Display(void);

// Show help.
void Help_Display(void);

// Command line arguments check.
void CommandLine_Check(int nArgc, char **ppArgv, char **ppFilename);

int Load_Header(char *pFnHeader, struct SHeader **pHeader);

time_t Get_TimeStamp(const struct SHeader *pHeader);

int Load_Data(char *pFnData, char *pFnGpx, const struct SHeader *pHeader);

// Display GPS point info.
void Point_Display(u32 nGPSPtNo, time_t nTimeStamp, struct SDataRecordGPS *pGPS, struct SHalfCurve *pCurve, double *pElev, FILE *pGpxFile);

#ifdef ELEVATION

#define	GOOGLE_MAPS_API_SZ_MAX	(2048)	// Possibly 8192? (Google API limit).

#define	HTTP_REQ_PART1			"wget -qO- \"https://maps.googleapis.com/maps/api/elevation/xml?locations="
#define	HTTP_REQ_PART2			"&key="
#define	HTTP_REQ_PART3			"\""
#define	HTTP_REQ_PART4			" | awk 'tolower($0) ~ /^ *<elevation>.*<\\/elevation> *$/ { sub(/^ *<elevation>/, \"\", $0); sub(/<\\/elevation> *$/, \"\", $0); print $0; }'"

#define	REQ_LOCATIONS_MAX_LN	(GOOGLE_MAPS_API_SZ_MAX - (sizeof(HTTP_REQ_PART1) + sizeof(HTTP_REQ_PART2) + sizeof(GOOGLE_MAPS_API_KEY) + sizeof(HTTP_REQ_PART3)))	// HTTP request, GPS records max len.
#define	REQ_LOCATIONS_MAX_NB	(512)		// Max number of coordinates per request (Google API limit).

#define	HTTP_REQ_MAX_LN			(sizeof(HTTP_REQ_PART1) + REQ_LOCATIONS_MAX_LN + sizeof(HTTP_REQ_PART2) + sizeof(GOOGLE_MAPS_API_KEY) + sizeof(HTTP_REQ_PART3) + sizeof(HTTP_REQ_PART4) + 1)

// HTTP API request.
int ApiRequest(char *pLocations, double *pElev, u32 nExpectedPoints);

// Get GPS' points via Google Maps API.
// (Grouped to limit the number of http requests (day limit is 2500)).
void AltitudeGet(struct SDataRecordType *pData, u32 nDataSz, double *pElev);
#endif

int main(int argc, char **argv);
