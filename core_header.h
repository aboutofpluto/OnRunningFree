#include <stdint.h>
#include <time.h>

#include "ctypes.h"
#include "format.h"

int Load_Header(char *pFnHeader, struct SHeader **ppHeader);
time_t Get_TimeStamp(const struct SHeader *pHeader);
