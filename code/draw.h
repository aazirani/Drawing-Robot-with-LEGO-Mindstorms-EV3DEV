/*
 *  \file  draw.c
 *  \brief  drawing mechanism.
 *  \author  Amin, Basti, Semir and Daniel 
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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




//move the robot to the coordinate of the destination from the current coordinate. This functions returns the destination coordinate, when the movement is successful.
void moveToCoordinate(int currentX, int currentY, int destinationX, int destinationY){
	float distance; 
	float degree;
	//calculate the distance the robot needs to go
	distance = sqrt((pow((destinationX-currentX), 2)+pow((destinationY-currentY), 2)));
	//calculate the degree the robot needs to rotate
	degree = asinf(abs((destinationY-currentY))/distance) * (180.0 / M_PI);
	printf("%f\n", distance);
	printf("%f\n", degree);
}
int main( void )
{
	moveToCoordinate(0,5,0,0);
	return ( 0 );
}
/*
//use the coordinates to draw the picture
void draw(Node* allCoordinates){
	int processedCoordinates = 0;
	boolean isPenDown = false;
	//save the to be processed coordinates
	Node* previousCoordinate = allCoordinates;
	Node* currentCoordinate = allCoordinates;
	while(currentCoordinate->next != NULL){
		//If we have just started drawing
		if(processedCoordinates == 0){
			//navigate to the first coordinate
			currentCoordinate = moveToCoordinate(0, 0, currentCoordinate->x, currentCoordinate->y);
			previousCoordinate = currentCoordinate;
			//move the pen down to start drawing
			movePen(DOWN);
			isPenDown = true;
		} else if (currentCoordinate->y == -1){
			//move the pen up
			movePen(UP);
			isPenDown = false;
		} else {
			moveToCoordinate(previousCoordinate->x, previousCoordinate->y, currentCoordinate->x, currentCoordinate->y);
			previousCoordinate = currentCoordinate;
			//if the pen is up, move the pen down
			if(!isPenDown){
				movePen(DOWN);
				isPenDown = true;
			}
		}
		//increment the number of processed coordinates.
		processedCoordinates++;
		//go to next coordinate
		currentCoordinate = currentCoordinate->next;
	}
}
*/

