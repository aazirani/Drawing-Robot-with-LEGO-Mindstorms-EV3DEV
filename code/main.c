/*
 *  \file  main.c
 *  \brief  main function to execute the program.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include "brick.h"
#include "ev3_tacho.h"
#include "motors.h"
#include "sensors.h"
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
	printf( "The drawing process is starting...\n" );
	//check if the functions work properly
	if ( !brick_init()) return ( 1 );
	if ( ev3_init() < 1 ) return ( 1 );
	if(init() && ev3_sensor_init()){
		
		while(getGyroVal() < 90){
			rotate(RIGHT, 80);
		}
		printf("%d\n", getGyroVal());
	}
	brick_uninit();
	return ( 0 );
}
