#define	ELEVATION	1		// Comment to remove all elevation stuff.
#define	GOOGLE_MAPS_API_KEY	"***************************************"		// *YOUR* API key.

#ifdef ELEVATION

#define	GOOGLE_MAPS_API_KEY	"***************************************"		// *YOUR* API key.
#define	GOOGLE_MAPS_API_SZ_MAX	(2048)	// Possibly 8192? (Google API limit).

#define	HTTP_REQ_PART1			"wget -qO- \"https://maps.googleapis.com/maps/api/elevation/xml?locations="
#define	HTTP_REQ_PART2			"&key="
#define	HTTP_REQ_PART3			"\""
#define	HTTP_REQ_PART4			" | awk 'tolower($0) ~ /^ *<elevation>.*<\\/elevation> *$/ { sub(/^ *<elevation>/, \"\", $0); sub(/<\\/elevation> *$/, \"\", $0); print $0; }'"

#define	REQ_LOCATIONS_MAX_LN	(GOOGLE_MAPS_API_SZ_MAX - (sizeof(HTTP_REQ_PART1) + sizeof(HTTP_REQ_PART2) + sizeof(GOOGLE_MAPS_API_KEY) + sizeof(HTTP_REQ_PART3)))	// HTTP request, GPS records max len.
#define	REQ_LOCATIONS_MAX_NB	(512)		// Max number of coordinates per request (Google API limit).

#define	HTTP_REQ_MAX_LN			(sizeof(HTTP_REQ_PART1) + REQ_LOCATIONS_MAX_LN + sizeof(HTTP_REQ_PART2) + sizeof(GOOGLE_MAPS_API_KEY) + sizeof(HTTP_REQ_PART3) + sizeof(HTTP_REQ_PART4) + 1)

#endif
