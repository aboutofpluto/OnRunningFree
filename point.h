#include <stdint.h>

#include "ctypes.h"
#include "format.h"

void Point_Display(u32 nGPSPtNo, time_t nTimeStamp, struct SDataRecordGPS *pGPS, struct SHalfCurve *pCurve, double *pElev, FILE *pGpxFile);
