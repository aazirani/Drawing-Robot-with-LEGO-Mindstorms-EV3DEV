/*
 *  \file  main.c
 *  \brief  main function to execute the program.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include "brick.h"
#include "ev3_tacho.h"
#include "draw.h"
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

int main( void )
{
	printf( "Initializing the robot...\n" );
	//check if the 
	if ( !brick_init()) return ( 1 );
	if ( ev3_init() < 1 ) return ( 1 );
	if(init() && ev3_sensor_init()){
		printf( "The drawing process is starting...\n" );
		//sample coordinates
		moveToCoordinate(0, 0, 0, 5, getGyroVal());
		moveToCoordinate(0, 5, 1, 8, getGyroVal());
		moveToCoordinate(1, 8, 2, 9, getGyroVal());
		moveToCoordinate(2, 9, 5, 10, getGyroVal());
		moveToCoordinate(5, 10, 8, 9, getGyroVal());
		moveToCoordinate(8, 9, 9, 8, getGyroVal());
		moveToCoordinate(9, 8, 10, 0, getGyroVal());
		printf( "The drawing process terminated.\n" );
	}
	//uninitialize the robot
	brick_uninit();
	return ( 0 );
}
