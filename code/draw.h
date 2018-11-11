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

// struct Node{
// 	int x;
// 	int y;
// 	struct Node *next;
// };
//
// typedef struct Node Node;
//
// Node* makeNode(int x, int y,Node* next){
// 	Node* node = malloc(sizeof(Node));
// 	node->x=x;
// 	node->y=y;
// 	node->next=next;
//
// 	return node;
// }
//
// Node* appendList(Node* node, int x, int y){
// 	if(node == NULL)
// 		return makeNode(x, y, NULL);
// 	Node* temp;
// 	while(temp->next != NULL)
// 		temp=temp->next;
// 	temp->next = makeNode(x, y, NULL);
// 	return node;
// }
//
// Node* readFile(){
// 	int y;
// 	int x;
// 	FILE *fptr;
// 	Node* n;
// 	if((fptr = fopen("C:\\Users\\Semir\\Desktop\\coordinates.txt", "r")) == NULL){
// 		printf("Error! Opening file.\n");
//
// 		exit(1);
// 	}
//
// 	while(fscanf(fptr, "%d %d", &x, &y) == 2){
// 		n= appendList(n, x, y);
// 		//fscanf(fptr, "%d %d" &x, &y);
// 	}
// 	fclose(fptr);
// 	return n;
// }
//
// int main(void){
// 	Node* n = NULL;
// 	n = readFile();
// 	printf("%d %d\n", n->x, n->y);
// 	return 0;
// }

typedef struct node
{
    int x;
	int y;
    struct node* next;
} node;
 
typedef void (*callback)(node* data);
 
/*
    create a new node
    initialize the data and next field
 
    return the newly created node
*/
node* create(int x, int y,node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->x = x;
	new_node->y = y;
    new_node->next = next;
 
    return new_node;
}
 
/*
    add a new node at the beginning of the list
*/
node* prepend(node* head,int x, int y)
{
    node* new_node = create(x,y,head);
    head = new_node;
    return head;
}
 
/*
    add a new node at the end of the list
*/
node* append(node* head, int x, int y)
{
    if(head == NULL)
        return NULL;
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
 
    /* create a new node */
    node* new_node =  create(x,y,NULL);
    cursor->next = new_node;
 
    return head;
}
 
/*
    insert a new node after the prev node
*/
node* insert_after(node *head, int x, int y, node* prev)
{
    if(head == NULL || prev == NULL)
        return NULL;
    /* find the prev node, starting from the first node*/
    node *cursor = head;
    while(cursor != prev)
        cursor = cursor->next;
 
    if(cursor != NULL)
    {
        node* new_node = create(x,y,cursor->next);
        cursor->next = new_node;
        return head;
    }
    else
    {
        return NULL;
    }
}
 
/*
    insert a new node before the nxt node
*/
node* insert_before(node *head, int x, int y, node* nxt)
{
    if(nxt == NULL || head == NULL)
        return NULL;
 
    if(head == nxt)
    {
        head = prepend(head,x,y);
        return head;
    }
 
    /* find the prev node, starting from the first node*/
    node *cursor = head;
    while(cursor != NULL)
    {
        if(cursor->next == nxt)
            break;
        cursor = cursor->next;
    }
 
    if(cursor != NULL)
    {
        node* new_node = create(x,y,cursor->next);
        cursor->next = new_node;
        return head;
    }
    else
    {
        return NULL;
    }
}
 
/*
    traverse the linked list
*/
void traverse(node* head,callback f)
{
    node* cursor = head;
    while(cursor != NULL)
    {
        f(cursor);
        cursor = cursor->next;
    }
}
/*
    remove node from the front of list
*/
node* remove_front(node* head)
{
    if(head == NULL)
        return NULL;
    node *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last node in the list */
    if(front == head)
        head = NULL;
    free(front);
    return head;
}
 
/*
    remove node from the back of the list
*/
node* remove_back(node* head)
{
    if(head == NULL)
        return NULL;
 
    node *cursor = head;
    node *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }
 
    if(back != NULL)
        back->next = NULL;
 
    /* if this is the last node in the list*/
    if(cursor == head)
        head = NULL;
 
    free(cursor);
 
    return head;
}
 
/*
    remove a node from the list
*/
node* remove_any(node* head,node* nd)
{
    if(nd == NULL)
        return NULL;
    /* if the node is the first node */
    if(nd == head)
        return remove_front(head);
 
    /* if the node is the last node */
    if(nd->next == NULL)
        return remove_back(head);
 
    /* if the node is in the middle */
    node* cursor = head;
    while(cursor != NULL)
    {
        if(cursor->next == nd)
            break;
        cursor = cursor->next;
    }
 
    if(cursor != NULL)
    {
        node* tmp = cursor->next;
        cursor->next = tmp->next;
        tmp->next = NULL;
        free(tmp);
    }
    return head;
 
}
/*
    display a node
*/
void display(node* n)
{
    if(n != NULL)
        printf("X:%d Y:%d", n->x, n->y);
	
}
 
/*
    Search for a specific node with input data
 
    return the first matched node that stores the input data,
    otherwise return NULL
*/
node* search(node* head,int x, int y)
{
 
    node *cursor = head;
    while(cursor!=NULL)
    {
        if(cursor->x == x && cursor->y == y){
        	return cursor;
        }
            
        cursor = cursor->next;
    }
    return NULL;
}
 
/*
    remove all element of the list
*/
void dispose(node *head)
{
    node *cursor, *tmp;
 
    if(head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while(cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}
/*
    return the number of elements in the list
*/
int count(node *head)
{
    node *cursor = head;
    int c = 0;
    while(cursor != NULL)
    {
        c++;
        cursor = cursor->next;
    }
    return c;
}
/*
    sort the linked list using insertion sort based on x value
*/
node* insertion_sort(node* head)
{
    node *x, *y, *e;
 
    x = head;
    head = NULL;
 
    while(x != NULL)
    {
        e = x;
        x = x->next;
        if (head != NULL)
        {
            if(e->x > head->x)
            {
                y = head;
                while ((y->next != NULL) && (e->x> y->next->x))
                {
                    y = y->next;
                }
                e->next = y->next;
                y->next = e;
            }
            else
            {
                e->next = head;
                head = e ;
            }
        }
        else
        {
            e->next = NULL;
            head = e ;
        }
    }
    return head;
}
 
/*
    reverse the linked list
*/
node* reverse(node* head)
{
    node* prev    = NULL;
    node* current = head;
    node* next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    return head;
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
void draw(node* allCoordinates){
	int processedCoordinates = 0;
	//0 for pen up an 1 for pen down
	int isPenDown = 0;
	//save the to be processed coordinates
	node* previousCoordinate = allCoordinates;
	node* currentCoordinate = allCoordinates;
	while(currentCoordinate->next != NULL){
		//If we have just started drawing
		if(processedCoordinates == 0){
			//navigate to the first coordinate
			moveToCoordinate(0, 0, currentCoordinate->x, currentCoordinate->y, getGyroVal());
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

