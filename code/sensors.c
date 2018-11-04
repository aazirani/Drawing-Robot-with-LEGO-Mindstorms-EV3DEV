/*
 *  \file  sensors.c
 *  \brief  data gathering and communication with the robot's sensors.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include <string.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif
