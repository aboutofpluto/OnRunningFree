%module omx2gpx

%{
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ctypes.h"
#include "format.h"
#include "core_header.h"
#include "core_data.h"
#include "pywrap.h"
#include "load.h"
%}

%include "pywrap.h"
