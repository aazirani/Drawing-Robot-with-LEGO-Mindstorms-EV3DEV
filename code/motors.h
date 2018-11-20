/*
 *  \file  motors.c
 *  \brief  communication with the robot's motors.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include "brick.h"
#include "ev3_tacho.h"
#include "sensors.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

#define SPEED_LINEAR    4  /* Motor speed to move backward and forward, in percents */
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
	//make the motor stop as soon as the movement ends
	tacho_set_stop_action(MOTOR_BOTH, TACHO_HOLD);
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
	//make the motor brake as soon as the movement ends
	tacho_set_stop_action(MOTOR_BOTH, TACHO_BRAKE);
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
		if(direction == UP){
			//set the the holder to move up
			set_tacho_polarity_inx(sn, TACHO_NORMAL);
		} else if(direction == DOWN){
			//set the holder to move down
			set_tacho_polarity_inx(sn, TACHO_INVERSED);
		} else {
			//if the input value is wrong, do nothing
			return;
		}
		//move the holder for 1000 milliseconds
		set_tacho_time_sp( sn, 500 );
		//move the holder
		set_tacho_command_inx( sn, TACHO_RUN_TIMED );
	} else {
		printf( "The medium motor is not connected.\n" );
	}
}

//rotate the robot for "degree" degrees in the given direction
int rotateDegree(direction_type direction, int degree){
	int currentDegree;
	//save the current degree
	currentDegree = getGyroVal();
	if (direction == LEFT) {
		//calculate the target degree
		int summedUpDegree = (currentDegree + degree);
		//fix the issue of a degree greater than 360
		while (summedUpDegree >=360){
			summedUpDegree = summedUpDegree - 360;
		}
		//printf("summedUpDegree: %d\n", summedUpDegree);
		//do not execute the command if the target degree is the same
		if(summedUpDegree == currentDegree){
			return currentDegree;
		}
		//rotate to the left until the correct degree is found
		while(getGyroVal() != summedUpDegree){
			int tempDegree = getGyroVal();
			int counter360Problem = summedUpDegree;
			if(summedUpDegree == 0){
				summedUpDegree = 360;
			}
			//4 different rotation speeds in order to be accurate and fast at the same time. If the target degree is farther away, the rotation is faster.
			//printf("tempDegree: %d\n", tempDegree);
			if( mod((summedUpDegree-tempDegree), 360) >= 20 ){
				rotate(LEFT, 300);
			} else if( mod((summedUpDegree-tempDegree), 360) >= 10 ){
				rotate(LEFT, 100);
			} else if( mod((summedUpDegree-tempDegree), 360) >= 3 ){
				rotate(LEFT, 60);
			} else {
				rotate(LEFT, 40);
			}
			if(counter360Problem == 0){
				summedUpDegree = 0;
			}
		}
	} else if (direction == RIGHT) {
		//calculate the target degree
		int differenceDegree = (currentDegree - degree);
		//fix the issue of a degree less than 0
		while (differenceDegree < 0){
			differenceDegree = differenceDegree + 360;
		}
		//printf("differenceDegree: %d\n", differenceDegree);
		//do not execute the command if the target degree is the same
		if(differenceDegree == currentDegree){
			return currentDegree;
		}
		//rotate to the right until the correct degree is found
		while(getGyroVal() != differenceDegree){
			int tempDegree = getGyroVal();
			//4 different rotation speeds in order to be accurate and fast at the same time. If the target degree is farther away, the rotation is faster.
			//printf("tempDegree: %d\n", tempDegree);
			if( mod((tempDegree-differenceDegree), 360) >= 20 ){
				rotate(RIGHT, 300);
			} else if( mod((tempDegree-differenceDegree), 360) >= 10 ){
				rotate(RIGHT, 100);
			} else if( mod((tempDegree-differenceDegree), 360) >= 3 ){
				rotate(RIGHT, 60);
			} else {
				rotate(RIGHT, 40);
			}
		}
	}
	//return the final degree when rotation terminates
	return getGyroVal();
}

//move the robot for the specified distance. This accepts 1 centimeter as minimum movement.
void moveDistance(direction_type direction, float distance){
	//after conducting careful tests, based on the speed of 4% of the maximum motor speed, a centimeter takes 400 milliseconds.
	//move(direction, round(400*distance));
	//since the process of drawing is proportional to this function, the speed can be choosen freely.
	move(direction, round(300*distance));
}
//calculate modulo
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}