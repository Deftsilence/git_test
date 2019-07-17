#pragma once

#include "ap_version.h"

#define THISFIRMWARE "cx07 V0.0.3-dev"
#define FIRMWARE_VERSION 0,0,3,FIRMWARE_VERSION_TYPE_DEV

#ifndef GIT_VERSION
#define FIRMWARE_STRING THISFIRMWARE
#else
#define FIRMWARE_STRING THISFIRMWARE " (" GIT_VERSION ")"
#endif

#this is a develop branch!
this is a test!
111
