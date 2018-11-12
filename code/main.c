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
	int x;
	int y;
 
	node* head = NULL;
	node* tmp = NULL;
	callback disp = display;
	
	printf( "Initializing the robot...\n" );
	//check if the 
	if ( !brick_init()) return ( 1 );
	if ( ev3_init() < 1 ) return ( 1 );
	if(init() && ev3_sensor_init()){
		printf( "The drawing process is starting...\n" );
		
		// head = prepend(head,1,1);
// 		head = append(head,2,2);
// 		head = append(head,3,3);
// 		head = append(head,4,4);
// 		head = append(head,5,5);
// 		head = append(head,6,6);
// 		display(head->next);
// 		draw(head);
		//sample coordinates
		// moveToCoordinate(0, 0, 0, 5, getGyroVal());
		// 		moveToCoordinate(0, 5, 1, 8, getGyroVal());
		// 		moveToCoordinate(1, 8, 2, 9, getGyroVal());
		// 		moveToCoordinate(2, 9, 5, 10, getGyroVal());
		// 		moveToCoordinate(5, 10, 8, 9, getGyroVal());
		// 		moveToCoordinate(8, 9, 9, 8, getGyroVal());
		// 		moveToCoordinate(9, 8, 10, 0, getGyroVal());
		
		// int bugsbunney[158][2] = {
// 			{-8, -8}, {-7, -7}, {-6, -5}, {-6, -4}, {-5, -2}, {-5, -1}, {-7, 1}, {-9, 4}, {-10, 6}, {-11, 9}, {-12, 13}, {-12, 16}, {-11, 19}, {-10, 19}, {-9, 16}, {-5, 9}, {-4, 6}, {-3, 4}, {-2, 1}, {-1, 0}, {0, 1}, {1, 0}, {2, 1}, {3, 4}, {4, 6}, {5, 9}, {9, 16}, {10, 19}, {11, 19}, {12, 16}, {12, 13}, {11, 9}, {10, 6}, {9, 4}, {7, 1}, {5, -1}, {5, -2}, {6, -4}, {6, -5}, {7, -7}, {8, -8}, {10, -8}, {12, -9}, {13, -11}, {13, -12}, {12, -14}, {11, -15}, {9, -16}, {7,-18}, {5,-19}, {4,-20}, {1,-21}, {-1,-21}, {-4,-20}, {-5,-19}, {-7,-18}, {-9,-16}, {-11,-15}, {-12,-14}, {-13,-12}, {-13,-11}, {-12,-9}, {-10,-8}, {-7,-8}, {-4,-9}, {-2,-10}, {0,-12}, {2,-10}, {4,-9}, {7,-8}, {8,-8},
//
// 			{-4, -9}, {-5, -7}, {-5,-4}, {-4,-2}, {-3,-2}, {-2,-4}, {-1,-7}, {-1,-9}, {-2,-10}, {-2,-7}, {-1,-9}, {-2, -10}, {-2,-7}, {-3,-6}, {-4,-7}, {-4,-9},
//
// 			{4,-9}, {5,-7}, {5,-4}, {4,-2}, {3,-2}, {2,-4}, {1,-7}, {1,-9}, {2,-10}, {2,-7}, {3,-6}, {4,-7}, {4,-9},
//
// 			{-4,1}, {-4,2}, {-7,9}, {-8,11}, {-9,14}, {-10,16}, {-11,17}, {-11,12}, {-10,9}, {-9,7}, {-7,4}, {-4,1},
//
// 			{4,1}, {4,2}, {7,9}, {8,11}, {9,14}, {10,16}, {11,17}, {11,12}, {10,9}, {9,7}, {7,4}, {4,1},
//
// 			{-5,-1}, {-4,0}, {-3,0}, {-2,-1},
//
// 			{-3,-12}, {-2,-16}, {0,-17}, {2,-16}, {3,-12}, {2,-11},
//
// 			{0,-13}, {0,-17},
//
// 			{5,-19}, {6,-17},
//
// 			{-5,-19}, {-6,-17},
//
// 			{3,-11}, {11,-8}, {13,-8},
//
// 			{4,-11}, {10,-10}, {14,-10},
//
// 			{5,-11}, {8,-11}, {14,-12},
//
// 			{-3,-11}, {-11,-8}, {-13,-8},
//
// 			{-4,-11}, {-10,-10}, {-14,-10},
//
// 			{-5,-11}, {-8,-11}, {-14,-12}
// 		};
		int batman[70][2] = {
			{0,-12}, {4,-6}, {5,-5}, {6,-5}, {7,-6}, {8,-8}, {9,-6}, {10,-5}, {11,-5}, {12,-6}, {13,-8}, {13,-9}, {12,-11}, {11,-12}, {14,-11}, {17,-9}, {19,-7}, {20,-5}, {21,-2}, {21,1}, {20,4}, {19,6}, {17,8}, {14,10}, {11,11}, {12,10}, {13,8}, {13,7}, {12,5}, {11,4}, {9,3}, {6,3}, {3,4}, {3,12}, {1,9}, {-1,9}, {-3,12}, {-3,4}, {-6,3}, {-9,3}, {-11,4}, {-12,5}, {-13,7}, {-13,8}, {-12,10}, {-11,11}, {-14,10}, {-17,8}, {-19,6}, {-20,4}, {-21,1}, {-21,-2}, {-20,-5}, {-19,-7}, {-17,-9}, {-14,-11}, {-11,-12}, {-12,-11}, {-13,-9}, {-13,-8}, {-12,-6}, {-11,-5}, {-10,-5}, {-9,-6}, {-8,-8}, {-7,-6}, {-6,-5}, {-5,-5}, {-4,-6}, {0,-12}
		};
				int i;
				int j;
				// for ( i = 0; i < 158; i++ ) {
// 					for ( j = 0; j < 2; j++ ) {
// 						bugsbunney[i][j] = bugsbunney[i][j] + 19;
// 						bugsbunney[i][j] = bugsbunney[i][j]/2;
// 					}
// 				}
// 				moveToCoordinate(0, 0, bugsbunney[0][0], bugsbunney[0][1], getGyroVal());
// 				for ( i = 1; i < 158; i++ ) {
//
// 					moveToCoordinate(bugsbunney[i-1][0], bugsbunney[i-1][1], bugsbunney[i][0], bugsbunney[i][1], getGyroVal());
//
// 				}
				
				for ( i = 0; i < 70; i++ ) {
					for ( j = 0; j < 2; j++ ) {
						batman[i][j] = batman[i][j] + 21;
					}
				}
				moveToCoordinate(0, 0, batman[0][0], batman[0][1], getGyroVal());
				for ( i = 1; i < 70; i++ ) {
				
					moveToCoordinate(batman[i-1][0], batman[i-1][1], batman[i][0], batman[i][1], getGyroVal());
					
				}
		
		// int bugsbunney[12][2] = {
		// 			{-8, -8},
		// 			{7, 0},
		// 			{7, 3},
		// 			{6, 4},
		// 			{5, 4},
		// 			{4, 3},
		// 			{4, 0},
		// 			{5, -2},
		// 			{6, -2},
		// 			{6, 0},
		// 			{5, 1},
		// 			{4, 0}
		// 		};
		// 		int i;
		// 		int j;
		// 		for ( i = 0; i < 12; i++ ) {
		// 			for ( j = 0; j < 2; j++ ) {
		// 				bugsbunney[i][j] = bugsbunney[i][j] + 2;
		// 			}
		// 		}
		// 		moveToCoordinate(0, 0, bugsbunney[0][0], bugsbunney[0][1], getGyroVal());
		// 		for ( i = 1; i < 12; i++ ) {
		// 			moveToCoordinate(bugsbunney[i-1][0], bugsbunney[i-1][1], bugsbunney[i][0], bugsbunney[i][1], getGyroVal());
		// 		}
		
		
		
		//moveToCoordinate(0, 0, 50, 50, getGyroVal());
		//moveToCoordinate(50, 50, 0, 0, getGyroVal());
		//moveToCoordinate(0, 0, 0, 50, getGyroVal());
		//moveToCoordinate(0, 50, 50, 0, getGyroVal());
		printf( "The drawing process terminated.\n" );
	}
	//uninitialize the robot
	brick_uninit();
	return ( 0 );
}
