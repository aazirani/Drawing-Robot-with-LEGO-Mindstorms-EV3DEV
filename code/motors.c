/*
 *  \file  motors.c
 *  \brief  communication with the robot's motors.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include "coroutine.h"
#include "brick.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif
