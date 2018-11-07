/*
 *  \file  motors.c
 *  \brief  communication with the robot's motors.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include "brick.h"
#include "ev3_tacho.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

#define SPEED_LINEAR    10  /* Motor speed to move backward and forward, in percents */
#define SPEED_CIRCULAR  10  /* ... for rotation */
#define SPEED_TACHO  10  /* ... for holder */

int max_speed;         /* Motor maximal speed (will be detected) */

#define MOTOR_LEFT     OUTC
#define MOTOR_RIGHT    OUTB
#define MOTOR_BOTH     ( MOTOR_LEFT | MOTOR_RIGHT )

typedef enum  {FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN} direction_type;

//Initialize the motors in order to move the robot
int init( void ){
	//Check if the motor is connected
	if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) {
		max_speed = tacho_get_max_speed( MOTOR_LEFT, 0 );
		tacho_reset( MOTOR_BOTH );
	} else {
		printf( "Please, plug the left motor into port C,\n"
			"the right motor into port B and try again.\n"
				);
		//Exit if the motors are not connected
		return ( 0 );
	}
	return ( 1 );
}

//move the robot "FORWARD" or "BACKWARD" for "time" milliseconds
void move(direction_type direction, int time){
	int speed_linear;
	//set the speed
	speed_linear = max_speed * SPEED_LINEAR / 100;
	if(direction == FORWARD){
		//set the speed to both motors
		tacho_set_speed_sp( MOTOR_BOTH, speed_linear );
	} else if(direction == BACKWARD){
		//set the negative speed to both motors in order to move backward
		tacho_set_speed_sp( MOTOR_BOTH, -speed_linear );
	} else {
		//if the input value is wrong, do nothing
		return;
	}
	//start the motors
	tacho_run_forever( MOTOR_BOTH );
	//run for "time" milliseconds
	sleep_ms( time );
	//stop the motors
	tacho_stop( MOTOR_BOTH );
}

void rotate(direction_type direction, int time){
	int speed_linear, speed_circular;
	//set the speed
	speed_circular = max_speed * SPEED_CIRCULAR / 100;
	if(direction == LEFT){
		//set the the speed of the two motors to the same but negative value
		tacho_set_speed_sp( MOTOR_LEFT, speed_circular );
		tacho_set_speed_sp( MOTOR_RIGHT, -speed_circular );
	} else if(direction == RIGHT){
		//set the the speed of the two motors to the same but negative value
		tacho_set_speed_sp( MOTOR_LEFT, -speed_circular );
		tacho_set_speed_sp( MOTOR_RIGHT, speed_circular );
	} else {
		//if the input value is wrong, do nothing
		return;
	}
	//start the motors
	tacho_run_forever( MOTOR_BOTH );
	//run for "time" milliseconds
	sleep_ms( time );
	//stop the motors
	tacho_stop( MOTOR_BOTH );
	
}

void movePen(direction_type direction){
	uint8_t sn;
	int speed_tacho;
	//search for the medium motor
	if ( ev3_search_tacho( LEGO_EV3_M_MOTOR, &sn, 0 )) {
		int max_speed;
		//set the medium motor's maximum speed
		get_tacho_max_speed( sn, &max_speed );
		speed_tacho = max_speed * SPEED_TACHO / 100;
		//set the type of action when the tacho stops.
		set_tacho_stop_action_inx( sn, TACHO_BRAKE);
		//set the speed
		set_tacho_speed_sp( sn, speed_tacho);
		if(direction == DOWN){
			//set the the holder to move up
			set_tacho_polarity_inx(sn, TACHO_NORMAL);
		} else if(direction == UP){
			//set the holder to move down
			set_tacho_polarity_inx(sn, TACHO_INVERSED);
		} else {
			//if the input value is wrong, do nothing
			return;
		}
		//move the holder for 1000 milliseconds
		set_tacho_time_sp( sn, 1000 );
		//move the holder
		set_tacho_command_inx( sn, TACHO_RUN_TIMED );
	} else {
		printf( "The medium motor is not connected.\n" );
	}
}

int main( void )
{
	//check if the functions work properly
	if ( !brick_init()) return ( 1 );
	if(init()){
		// movePen(DOWN);
		// Sleep( 2000 );
		// movePen(UP);
		// rotate(LEFT, 1000);
		// rotate(RIGHT, 1000);
	}
	brick_uninit();
	return ( 0 );
}