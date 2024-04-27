/*
 * SnakeGame.c
 *
 * Created: 2024-02-17 5:11:28 PM
 * Author : Jim
 */

#define F_CPU 8000000UL
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "MAX7219_Lib.c"

//Need to replace name of functions 
//Use global flags
bool isDownBool = false;
bool isUpBool = false;
bool isLeftBool = true;
bool isRightBool = false; 

bool dead = false;
bool eat = false;

int display_array[8];

typedef struct{
	int row;
	int column;
	struct node * previous_node; 
	struct node * next_node;
}node;

typedef struct{
	int row;
	int column;
}food;

bool isDown(){
	if((PINC & (1 << 0))!= 0){
		if((isDownBool == false) && (isUpBool == false))
			{isDownBool = true;
			isUpBool = false;
			isLeftBool = false;
			isRightBool = false;}
		else{}
		}
	else{}
}

bool isUp(){
	if((PINC & (1 << 1))!= 0){
		if((isDownBool == false) && (isUpBool == false))
			{isDownBool = false;
			isUpBool = true;
			isLeftBool = false;
			isRightBool = false;}
		else{}
		}
	else{}
}

bool isLeft(){
	if((PINC & (1 << 2))!= 0){
		if((isRightBool == false) && (isLeftBool == false))
			{isDownBool = false;
			isUpBool = false;
			isLeftBool = true;
			isRightBool = false;}
		else{}
		}
	else{}
}

bool isRight(){
	if((PINC & (1 << 3))!= 0){
		if((isLeftBool == false) && (isRightBool == false))
			{isDownBool = false;
			isUpBool = false;
			isLeftBool = false;
			isRightBool = true;}
		else{}
		}
	else{}
}

void restart(node * headptr, food * foodptr){
	node * temp = headptr;
	
	//only keep 3 node of the snake
	for(int i = 0;  i < 2; i++){
		temp->row = 1;
		temp->column = (1 << (2-i));
		temp = temp->previous_node;
	}
	//cut off the rest of the body
	node * temp2 = temp->previous_node;
	temp2->next_node = 0;
	temp->previous_node = 0; 
	
	//restart boolean values
	isDownBool = false;
	isUpBool = false;
	isLeftBool = true;
	isRightBool = false;
	
	foodptr->row = 6;
	foodptr->column = 1 << 5;
	
	dead = false;
	printSnake(headptr, foodptr);
}

int main(void)
{
	setup_7219();
	

	DDRC = DDRC & (~(1 << 0) | ~(1 << 1) | ~(1 << 2) | ~(1 << 3));
	//set the pin 23, 24, 25, and 26 into input.
	//pin 23 is down, 24 is up, 25 is left, 26 is right.
	DDRB  = DDRB |  (1 << 0); 
	//set pin 14 to be output
	
	
	setup_7219();
	test_individual_7219();
	test_vertical_7219();
	test_horizontal_7219();
	
	//create head node
	node head;
	node tailtest;
	node tailtest1;
	
	head.row = 1;
	head.column = (1 << 2);
	head.previous_node = &tailtest;
	head.next_node = 0;
	
	tailtest.row = 1;
	tailtest.column = (1 << 1);
	tailtest.previous_node = &tailtest1;
	tailtest.next_node = &head;
	
	tailtest1.row = 1;
	tailtest1.column = (1 << 0);
	tailtest1.previous_node = 0;
	tailtest1.next_node = &tailtest;
	
	food food_piece;
	food_piece.row = 6;
	food_piece.column = 1 << 5;
	
	//create pointer to food piece
	food * food_ptr = &food_piece;
	//create pointer to node head
	node * head_pointer = &head; 
	
	isDownBool = false;
	isUpBool = false;
	isLeftBool = true;
	isRightBool = false;

	dead = false;
	eat = false;
	printSnake(head_pointer, food_ptr);
    while (1) 
    {
		isDown();
		isUp();
		isLeft();
		isRight();
		if(isDownBool == true){moveDown(head_pointer, food_ptr);}
		else if(isUpBool == true){moveUp(head_pointer, food_ptr);}
		else if(isLeftBool == true){moveLeft(head_pointer, food_ptr);}
		else if(isRightBool == true){moveRight(head_pointer, food_ptr);}
	}
}

void moveDown(node * headptr, food * foodptr){
	int temp_column = headptr->column;
	int temp_row = headptr->row;
	
	//1. Move head of snake
	if(headptr->row < 8){
		headptr->row = (headptr->row + 1);}
	else if(headptr->row >= 8){
		headptr->row = 1;}
		
	//2. Is snake dead?
	isDead(headptr);
	if(dead == true)
		{//3.a If snake is dead restart the game
		restart(headptr, foodptr);}
	else if(dead == false)
		{//3.b If snake is not dead, ask,
		 //4. Is snake eaten?
			isEat(headptr, foodptr);
			if(eat == true)
				{//Restart eat value
				eat = false;
				//updateNode(headptr, temp_column, temp_row);
				add_node(headptr, temp_column, temp_row);
				//repopulate the food
				populateFood(headptr, foodptr);}
			else if(eat == false)
				{//if snake has not eaten, update node
				updateNode(headptr, temp_column, temp_row);}
			
			_delay_ms(40);
			printSnake(headptr, foodptr);
		}
}

void moveUp(node *headptr, food * foodptr){
	//Move lead pixel up
	int temp_column = headptr->column;
	int temp_row = headptr->row;
	if(headptr->row > 1){headptr->row = (headptr->row - 1);}
	else if(headptr->row = 1){headptr->row = 8;}
	
	//2. Is snake dead?
	isDead(headptr);
	if(dead == true)
		//3.a If snake is dead restart the game
		{restart(headptr, foodptr);}
	else if(dead == false)
			{//3.b If snake is not dead, ask,
			//4. Is snake eaten?
			isEat(headptr, foodptr);
			if(eat == true)
				//update node
				{eat = false;
				//updateNode(headptr, temp_column, temp_row);
				add_node(headptr, temp_column, temp_row);
				populateFood(headptr, foodptr);}
			else if(eat == false)
			//if snake has not eaten, update node
				{updateNode(headptr, temp_column, temp_row);}
				_delay_ms(40);
			printSnake(headptr, foodptr);
			
	}
}

void moveLeft(node *headptr, food * foodptr){
	int temp_column = headptr->column;
	int temp_row = headptr->row;
	
	if(headptr->column < 128){headptr->column = headptr->column << 1;}
	else if(headptr->column >= 128){headptr->column = 1;}
	
		//2. Is snake dead?
		isDead(headptr);
		if(dead == true)
		{//3.a If snake is dead restart the game
		restart(headptr, foodptr);}
		else if(dead == false)
		{//3.b If snake is not dead, ask,
			//4. Is snake eaten?

			isEat(headptr, foodptr);
			if(eat == true)
			{//update node
				eat = false;
				//updateNode(headptr, temp_column, temp_row);
				add_node(headptr, temp_column, temp_row);
				populateFood(headptr, foodptr);}
			else if(eat == false)
			{//if snake has not eaten, update node
			updateNode(headptr, temp_column, temp_row);}
			_delay_ms(40);
			printSnake(headptr, foodptr);
		}
}

void moveRight(node * headptr, food * foodptr){
	int temp_column = headptr->column;
	int temp_row = headptr->row;
	
	if(headptr->column > 0b0001){headptr->column = headptr->column >> 1;}
	else if(headptr->column <= 1){headptr->column = 0b10000000;}
			
	//2. Is snake dead?
	isDead(headptr);
	if(dead == true)
	{//3.a If snake is dead restart the game
	restart(headptr, foodptr);}
	else if(dead == false)
	{//3.b If snake is not dead, ask,
		//4. Is snake eaten?
		isEat(headptr, foodptr);
		if(eat == true)
		{	eat = false;
			add_node(headptr, temp_column, temp_row);
			//updateNode(headptr, temp_column, temp_row);
			populateFood(headptr, foodptr);
		}
		else if(eat == false)
		{//if snake has not eaten, update node
		updateNode(headptr, temp_column, temp_row);}
		_delay_ms(40);
		printSnake(headptr, foodptr);
	}
}

void add_node(node * headptr, int temp_column, int temp_row){
	/*This function is to add another node 
	after the snake ate food.*/
	
	node * new_node = (node*) malloc(sizeof(node)); 
	//allocate a memory in space for struct node
	//create node pointer new_node
	node * temp = headptr; 
	node * temp2 = temp->previous_node; 
	
	new_node->previous_node = temp2;
	new_node->column = temp_column;
	new_node->row = temp_row;
	new_node->next_node = temp;
	temp2->next_node = new_node;
	temp->previous_node = new_node;
}

void isDead(node * headptr){
	/*This function is to determine
	whether or not the snake just
	ate itself, and update the global flag*/
	node * temp = headptr->previous_node;
	bool flag = false;
	while((temp->previous_node != 0) && (flag == false)){
		if((temp->column == headptr->column) && (temp->row == headptr->row)){
			flag = true;}
		temp = temp->previous_node;
	}
	if((temp->column == headptr->column) && (temp->row == headptr->row)){
		flag = true;}
	dead = flag;
}

void isEat(node * headptr, food * foodptr){
	if((foodptr->row == headptr->row) && (foodptr->column == headptr->column)){
		eat = true;}
	else{
		eat = false;}
}

void populateFood(node * headptr, food * foodptr){
	/*This function is to populate the food on the array*/
	int rand_row; 
	/*8 is upper bound, 1 is lower bound*/
	/*Populate   between   number   between  1   and   8*/

	node * temp = headptr; 
	int array_temp_1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	
	/*Create  a  temp  array  to  store all  column  values
	Column values can only have value such as 0b01 or 0b10.
	It can only be  power of 2. So  by adding  column value 
	there  shouldn't  be  any  carry  out*/
	while(temp->previous_node != 0)
	{	array_temp_1[(temp->row) - 1] += temp->column;
		temp = temp->next_node;}
	array_temp_1[(temp->row) - 1] += temp->column;
	
	bool flag = true;
	while(flag == true){
		/*while flag is still true, re-populate the row  value
		if the value of that array index is 255, it means that
		it is full. If it is not 255 then there  is a space.*/
		rand_row = rand() % 7 + 1;;
		if(array_temp_1[rand_row - 1] == 255)
			{//PORTB = 0b1;
			//_delay_ms(10);
			//PORTB = 0b0;
			//_delay_ms(10);
			}
		else{flag = false;}
		/*Once  flag is  false,  then  row  value  is  set. */
	}
	
	int array_temp_2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int temp_2 = array_temp_1[rand_row - 1];
	/*The following for loop get the value  of the sum of column
	values, based on the rand_row  value.  Then it separate the 
	value into 1 and 0, and store each bit inside array_temp_2*/
	for(int i = 0; i < 8; i++){
		if(temp_2 & 0b1 == 1)    
		   {array_temp_2[i] = 1;}
		else if(temp_2 & 0b1 == 0)  
		   {array_temp_2[i] = 0;}
		temp_2 = temp_2 >> 1;
		}
	
	/*The following code create a rand number to populate column
	value. If the rand_column value value already exist, it will
	populate  a new value*/
	flag = true;
	int rand_column;
	while(flag == true){
		rand_column = rand() % 7 + 1;
		if(array_temp_2[rand_column - 1] != 1)
		{flag = false;}
		else{}
	}
	
	foodptr->row = rand_row;
	foodptr->column = 1 << rand_column;
}

void updateNode(node * headptr, int temp_column, int temp_row){
	/*this function updates all of the nodes
	to have the row and column value of their 
	next nodes*/
	node * temp = headptr;
	
	while(temp->previous_node != 0){
		//navigate to the last node.
		temp = temp->previous_node;
	}
		node * temp2;
		while(temp->next_node != headptr){
			temp2 = temp->next_node;
			temp->row = temp2->row;
			temp->column = temp2->column;
			temp = temp->next_node;
		}
		temp->row = temp_row;
		temp->column = temp_column;
}

void printSnake(node * headptr, food * foodptr){
	//scan all nodes to get rows and column values
	node * temp = headptr;
	
	//creating a zero array
	for(int i = 0; i < 8; i++){
		display_array[i] = 0;}
	
	while(temp->previous_node != 0){
		display_array[(temp->row)-1] += (temp->column);
		temp = temp->previous_node;}
		
	//final element of the node
	display_array[(temp->row)-1] += temp->column;
	//include food piece in array
	display_array[(foodptr->row)-1] += foodptr->column;
	
	for(int i = 0; i < 8; i++)
		{write_7219(i + 1, display_array[i]); update_7219();}
}

