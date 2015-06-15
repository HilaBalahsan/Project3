#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool is_legal_move(path_t* user_input_path)
{
	bool comparison;
	int i;
	comparison = FALSE;
	get_moves(Turn);
	if (paths_number == 0)
	{
		return comparison;
	}
	else
	{
		for(i = 0; i < paths_number; i++)
		{
			comparison = compare_two_paths(paths_arr[i], user_input_path);
			if (comparison)
			{
				break;
			}
		}
	}
	return comparison;
}

bool is_empty_position(int row, int col){
	bool empty = TRUE;
	if (game_board[row][col] != EMPTY)
	{
		empty = FALSE;
	}
	return empty;
}

bool is_at_the_edge(int row, int col){
	bool edge;
	edge = FALSE;
	if ((row == 0) && (col == 0))
	{
		edge = TRUE;
	}
	else if ((row == 0) && (col == BOARD_SIZE - 1))
	{
		edge = TRUE;
	}
	else if ((col == 0) && (row == BOARD_SIZE - 1))
	{
		edge = TRUE;
	}
	else if ((col == BOARD_SIZE - 1) && (row == BOARD_SIZE - 1))
	{
		edge = TRUE;
	}
	return edge;
}

bool is_a_winner(player_e turn){
	bool winner = FALSE;
	coordinate_t* temp_king, *temp_man;

	if (turn == COMPUTER)
	{
		temp_king = creat_linkedList_pointer(KING, USER);
		temp_man = creat_linkedList_pointer(MAN, USER);
	}
	else
	{
		temp_king = creat_linkedList_pointer(KING, COMPUTER);
		temp_man = creat_linkedList_pointer(MAN, COMPUTER);
	}
	if ((temp_king == NULL) && (temp_man == NULL)) // enemy don't have any tools.
	{
		winner = TRUE;
	}
	else
	{
		get_moves(turn^1);
		if (paths_arr[0] == NULL)
		{
			winner = TRUE;
		}
	}

	return winner;
}

bool is_valid_position(int row, int col){

	// define
	bool b;

	// initalize
	b = TRUE;

	//if (((col % 2 == 1) && (row % 2 == 0)) || ((col % 2 == 0) && (row % 2 == 1)))

	if (((row + col) % 2) != 0)      //assamption: indexes starts from ziro
	{
		b = FALSE;
	}

	if ((row > 9) || (row < 0) || (col > 9) || (col < 0))
	{
		b = FALSE;
	}

	return b;
}

bool is_valid_initialization()
{
	// define
	bool b;
	int num_of_white, num_of_black;

	// initalize
	b = TRUE;
	if (user.color == WHITE)
	{
		num_of_white = user.num_of_men + user.num_of_kings;
		num_of_black = computer.num_of_men + computer.num_of_kings;
	}
	else
	{
		num_of_white = computer.num_of_men + computer.num_of_kings; 
		num_of_black = user.num_of_men + user.num_of_kings;
	}

	if ((num_of_white > 20) || (num_of_black > 20) || ((num_of_white == 0) && (num_of_black != 0)) || ((num_of_white != 0) && (num_of_black == 0)) || ((num_of_black == 0) && (num_of_white == 0)))
	{
		b = FALSE;
	}
	
	return b;
}

bool is_enemy_position(int row, int col){
	char tool;
	bool enemy;
	color_e mine;

	enemy = FALSE;
	tool = game_board[row][col];

	if ((is_valid_position(row, col)) && (game_board[row][col] != EMPTY))
	{
		if (Turn == USER)
		{
			mine = user.color;
		}
		else
		{
			mine = computer.color;
		}

		if (mine == WHITE)
		{
			if ((tool == BLACK_M) || (tool == BLACK_K))
			{
				enemy = TRUE;
			}
		}
		else
		{
			if ((tool == WHITE_M) || (tool == WHITE_K))
			{
				enemy = TRUE;
			}
		}
	}
	return enemy;
}

bool is_become_king(int row, int col, player_e turn){

	bool king;
	color_e mine;

	king = FALSE;

	if (turn == USER)
	{
		mine = user.color;
	}
	else
	{
		mine = computer.color;
	}

	if (mine == WHITE)
	{
		if (((row == BOARD_SIZE - 1) && (is_empty_position(row, col)) && (is_valid_position(row, col))))
		{
			king = TRUE;
		}
	}
	else
	{
		if (((row == 0) && (is_empty_position(row, col)) && (is_valid_position(row, col))))
		{
			king = TRUE;
		}
	}

	return king;
}