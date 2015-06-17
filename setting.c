#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


/**
* set_user_color
* input : color_e color
* output : nothig
* functionality : updates computer's and user's color (and their struct as well)
* updates turn according to the player with white color (player white white color starts first)
*/
void set_user_color(color_e color)
{
	if (color == WHITE)
	{
		user.color = color;
		computer.color = BLACK;
		Turn = USER;
	}
	else
	{
		user.color = color;
		computer.color = WHITE;
		Turn = COMPUTER;
	}
}

/**
* set_minimax_depth
* input : int depth
* output : nothig
* functionality : sets minimax_depth and updating globle iny Minimax_depth
* if depth > 6 or depth <1 it's not legal depth
*/
void set_minimax_depth(int depth){
	if ((depth > 6) || (depth < 1))
	{
		print_message(WRONG_MINIMAX_DEPTH);
	}
	else
	{
		Minimax_Depth = depth;
	}
}

/**
* set_disc
* input : char char_on_board, int row, int col, color_e tool_color, type_e tool_type
* output : int 
* functionality : sets a given disc on the board updates compuer's and user's men and kings linked list
* return : -1 if the was memory allocation problems, 1 seccesfull set on the board 
*/
int set_disc(char char_on_board, int row, int col, color_e tool_color, type_e tool_type)
{
	coordinate_t *temp_linkedlist;

	if ((!is_valid_position(row, col)))
	{
		print_message(WRONG_POSITION);
		return 1;
	}
	game_board[row][col] = char_on_board;
	if (State == GAME_STATE)
	{
		if (user.color == tool_color)
		{
			temp_linkedlist = creat_linkedList_pointer(tool_type, USER);
			if (tool_type == KING)
			{
				user.num_of_kings++;
			}
			else
			{
				user.num_of_men++;
			}
		}
		else
		{
			temp_linkedlist = creat_linkedList_pointer(tool_type, COMPUTER);
			if (tool_type == KING)
			{
				computer.num_of_kings++;
			}
			else
			{
				computer.num_of_men++;
			}
		}

		temp_linkedlist = updating_linked_list(row, col, temp_linkedlist);
		if (temp_linkedlist == NULL)
		{
			return -1;
		}
	}
	return 1;
}

/**
* clear
* input : nothing
* output : nothing
* functionality : removes all discs from game board
* return :  1 seccesfullclear of the board
*/
int clear(){

	int i, j;

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = BOARD_SIZE; j >= 0; j--)
		{
			game_board[i][j] = EMPTY;
		}
	}
	user.num_of_kings = 0;
	user.num_of_men = 0;
	computer.num_of_kings = 0;
	computer.num_of_men = 0;

	return 1;
}

/**
* remove_disc
* input : int row, int col, player_e rm_from_this_player
* output : int
* functionality : removes a given disc on the board 
* return : -1 if the position is not valid, 1 seccesfull set on the board
*/
int remove_disc(int row, int col, player_e rm_from_this_player){

	if (!is_valid_position(row, col))
	{
		print_message(WRONG_POSITION);
		return -1;
	}

	game_board[row][col] = EMPTY;
	return 1;
}

void print_line(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

void print_board()
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", game_board[j][i]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

/**
* start
* input : noting
* output : int
* functionality : starts a game
* return : -1 if the initiliztion of the board is not valid , 1 successfull start
* changes State to setting state
*/
int start()
{
	first_updating_MenKings_coordinate();
	if (!is_valid_initialization())
	{
		print_message(WROND_BOARD_INITIALIZATION);
		return -1;
	}
	State = GAME_STATE;

	return 1;
}
