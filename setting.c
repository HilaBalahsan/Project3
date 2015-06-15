#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>



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
	}
}

void set_minimax_depth(int depth){
	if ((depth > 6) || (depth < 1))
	{
		printf(WRONG_MINIMAX_DEPTH);
	}
	else
	{
		Minimax_Depth = depth;
	}
}

int set_disc(char char_on_board, int row, int col, color_e tool_color, type_e tool_type)
{
	coordinate_t *temp_linkedlist;

	if ((!is_valid_position(row, col)) || (!is_empty_position(row, col)))
	{
		printf(WRONG_POSITION);
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

int remove_disc(int row, int col, player_e rm_from_this_player){

	if (!is_valid_position(row, col))
	{
		printf(WRONG_POSITION);
		return -1;
	}

	game_board[col][row] = EMPTY;
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
			//printf("| %c ", game_board[i][j]);
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

int start()
{
	first_updating_MenKings_coordinate();
	if (!is_valid_initialization())
	{
		printf(WROND_BOARD_INITIALIZATION);
		return -1;
	}
	State = GAME_STATE;

	return 1;
}
