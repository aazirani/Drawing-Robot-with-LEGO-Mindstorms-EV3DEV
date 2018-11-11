/*
 *  \file  draw.c
 *  \brief  drawing mechanism.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "motors.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

struct Node{
	int x;
	int y;
	struct Node *next;
};

typedef struct Node Node;

Node* makeNode(int x, int y,Node* next){
	Node* node = malloc(sizeof(Node));
	node->x=x;
	node->y=y;
	node->next=next;

	return node;
}

Node* appendList(Node* node, int x, int y){
	if(node == NULL)
		return makeNode(x, y, NULL);
	Node* temp;
	while(temp->next != NULL)
		temp=temp->next;
	temp->next = makeNode(x, y, NULL);
	return node;
}

Node* readFile(){
	int y;
	int x;
	FILE *fptr;
	Node* n;
	if((fptr = fopen("C:\\Users\\Semir\\Desktop\\coordinates.txt", "r")) == NULL){
		printf("Error! Opening file.\n");

		exit(1);
	}

	while(fscanf(fptr, "%d %d", &x, &y) == 2){
		n= appendList(n, x, y);
		//fscanf(fptr, "%d %d" &x, &y);
	}
	fclose(fptr);
	return n;
}

int main(void){
	Node* n = NULL;
	n = readFile();
	printf("%d %d\n", n->x, n->y);
	return 0;
}

//move the robot to the coordinate of the destination from the current coordinate based on its current degree.
void moveToCoordinate(int currentX, int currentY, int destinationX, int destinationY, int robotCurrentDegree){
	float distance;
	int degree;
	int calculatedDegree;
	//calculate the distance the robot needs to go
	distance = sqrt((pow((destinationX-currentX), 2)+pow((destinationY-currentY), 2)));
	//calculate the degree the robot needs to rotate
	degree = atan((float)(destinationY-currentY)/(float)(destinationX-currentX)) * (180.0 / M_PI);
	//to counter the effect of calculating the atan while moving from right to left
	if(destinationX < currentX){
		degree = degree + 180;
	}
	//to counter the effect of a negative calculated degree
	while (degree < 0){
		degree = degree+360;
	}
	//to counter the effect of a negative calculated current degree
	while (robotCurrentDegree < 0){
		robotCurrentDegree = robotCurrentDegree+360;
	}
	//calculate the shortest rotation needed to face the target
	calculatedDegree = abs(robotCurrentDegree - degree);
	//if the calculated degree is greater than 360, it is more efficient to rotate to the oder side. Fix this issue.
	if(calculatedDegree > 180){
		calculatedDegree = abs(360 - calculatedDegree);
	}
	//Calculate in which way the rotation needs to be done based on the current degree.
	//If the current degree is in the lower part of a circle (> pi) calculate the correct rotation direction
	if(robotCurrentDegree > 180){
		if(robotCurrentDegree - 180 < degree && degree < robotCurrentDegree){
			rotateDegree(RIGHT, calculatedDegree);
		} else {
			rotateDegree(LEFT, calculatedDegree);
		}
	} else {
		if(robotCurrentDegree + 180 > degree && degree > robotCurrentDegree){
			rotateDegree(LEFT, calculatedDegree);
		} else {
			rotateDegree(RIGHT, calculatedDegree);
		}
	}
	//move the robot forward for the calculated distance
	moveDistance(FORWARD, distance);
}

//use the coordinates to draw the picture
void draw(Node* allCoordinates){
	int processedCoordinates = 0;
	//0 for pen up an 1 for pen down
	int isPenDown = 0;
	//save the to be processed coordinates
	Node* previousCoordinate = allCoordinates;
	Node* currentCoordinate = allCoordinates;
	while(currentCoordinate->next != NULL){
		//If we have just started drawing
		if(processedCoordinates == 0){
			//navigate to the first coordinate
			currentCoordinate = moveToCoordinate(0, 0, currentCoordinate->x, currentCoordinate->y, getGyroVal());
			previousCoordinate = currentCoordinate;
			//move the pen down to start drawing
			movePen(DOWN);
			isPenDown = 1;
		} else if (currentCoordinate->y == -1){
			//move the pen up
			movePen(UP);
			isPenDown = 0;
		} else {
			moveToCoordinate(previousCoordinate->x, previousCoordinate->y, currentCoordinate->x, currentCoordinate->y, getGyroVal());
			previousCoordinate = currentCoordinate;
			//if the pen is up, move the pen down
			if(!isPenDown){
				movePen(DOWN);
				isPenDown = 1;
			}
		}
		//increment the number of processed coordinates.
		processedCoordinates++;
		//go to next coordinate
		currentCoordinate = currentCoordinate->next;
	}
}

