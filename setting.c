#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>



void set_user_color(color_e color)
{
	if (color == WHITE)
	{
		user.minimax_depth = player_a;
		user.color = color;
		computer.color = BLACK;
		computer.minimax_depth = player_b;
		turn = USER;
	}
	else
	{
		user.minimax_depth = player_b;
		user.color = color;
		computer.color = WHITE;
		computer.minimax_depth = player_a;
	}
}

int set_minimax_depth(int depth){
	if ((depth > 6) || (depth < 1))
	{
		printf(WRONG_MINIMAX_DEPTH);
	}
	else
	{
		Minimax_Depth = depth;
		player_a = (depth / 2) + 1;
		player_b = (depth / 2);
	}
}

int set_disc(char char_on_board, int row, int col, color_e tool_color, type_e tool_type)
{
	coordinate_t *temp_linkedlist;
	int return_val;

	if ((!is_valid_position(row, col)) || (!is_empty_position(row, col)))
	{
		printf(WRONG_POSITION);
		return -1;
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

	if (State == GAME_STATE)
	{
		if ((computer.kings_coordinate != NULL) && (user.kings_coordinate != NULL)
			&& (computer.men_coordinate != NULL) && (user.men_coordinate != NULL))
		{
			free_linked_list(computer.kings_coordinate);
			free_linked_list(user.kings_coordinate);
			free_linked_list(computer.men_coordinate);
			free_linked_list(user.men_coordinate);
		}
	}
	return 1;
}

int remove_disc(int row, int col, player_e rm_from_this_player){
	type_e tool_type;
	coordinate_t* specific_link;

	if (!is_valid_position(row, col))
	{
		printf(WRONG_POSITION);
		return -1;
	}

	if (State == GAME_STATE)
	{
		
		if ((game_board[row][col] == BLACK_M) || (game_board[row][col] == WHITE_M))
		{
			tool_type = MAN;
		}
		else
		{
			tool_type = KING;
		}

		specific_link = creat_linkedList_pointer(tool_type, rm_from_this_player);
		delete_link_from_linked_list(specific_link);
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
