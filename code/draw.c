/*
 *  \file  draw.c
 *  \brief  drawing mechanism.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif