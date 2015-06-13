#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>




char backup_board[BOARD_SIZE][BOARD_SIZE] = { 0 };

void copy_board(){
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			tmp_board[i][j] = game_board[i][j];
		}
	}
	return tmp_board;
}

void copy_gameboard_to_tmpboard(){

	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			tmp_board[i][j] = game_board[i][j];
		}
	}
}

void copy_tmpboard_to_gameboard()
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			game_board[i][j] = tmp_board[i][j];
		}
	}
}

void path_on_board(path_t* path_pointer)
{
	char char_on_board;
	type_e tool_type;
	color_e tool_color;
	int path_score, row, col;

	path_score = 0;
	row = 0;
	col = 0;

	row = path_pointer->head_position->row;
	col = path_pointer->head_position->col;

	tool_type = find_type(row, col); // tool type
	tool_color = find_color(row, col); // tool color
	char_on_board = game_board[row][col]; // whats in the board

	while (path_pointer->head_position != NULL)
	{
		row = path_pointer->head_position->row;
		col = path_pointer->head_position->col;
		if (is_enemy_position(row, col))
		{
			remove_disc(row, col, turn);
		}
		else
		{
			set_disc(char_on_board, row, col, tool_color, tool_type);
		}
		path_pointer->head_position = path_pointer->head_position->next_coordinate; // next
	} // end of while

	free_path(path_pointer);
}

void original_board()
{
	//char** backup_board;
	int i, j;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			backup_board[i][j] = game_board[i][j];
		}
	}

	//return (backup_board);
}

void back_up_players()
{
	back_up_user.num_of_kings = user.num_of_kings;
	back_up_user.num_of_men = user.num_of_men;
	back_up_user.color = user.color;
	back_up_user.kings_coordinate = user.kings_coordinate;
	back_up_user.men_coordinate = user.men_coordinate;
	back_up_user.minimax_depth = user.minimax_depth;

	back_up_computer.num_of_kings = computer.num_of_kings;
	back_up_computer.num_of_men = computer.num_of_men;
	back_up_computer.color = computer.color;
	back_up_computer.kings_coordinate = computer.kings_coordinate;
	back_up_computer.men_coordinate = computer.men_coordinate;
	back_up_computer.minimax_depth = computer.minimax_depth;

}

void return_player_to_original_satae()
{
	user.num_of_kings = back_up_user.num_of_kings;
	user.num_of_men = back_up_user.num_of_men;
	user.color = back_up_user.color;
	user.kings_coordinate = back_up_user.kings_coordinate;
	user.men_coordinate = back_up_user.men_coordinate;
	user.minimax_depth = back_up_user.minimax_depth;

	computer.num_of_kings = back_up_computer.num_of_kings;
	computer.num_of_men = back_up_computer.num_of_men;
	computer.color = back_up_computer.color;
	computer.kings_coordinate = back_up_computer.kings_coordinate;
	computer.men_coordinate = back_up_computer.men_coordinate;
	computer.minimax_depth = back_up_computer.minimax_depth;
}

int scoring(){

	int i, j, white_score, black_score;
	char char_on_board;

	white_score = 0;
	black_score = 0;
	char_on_board = EMPTY;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (use_tmp_board)
				char_on_board = tmp_board[i][j];
			if (!use_tmp_board)
				char_on_board = game_board[i][j];

			if (char_on_board == WHITE_M)
				white_score += 1;
			if (char_on_board == WHITE_K)
				white_score += 3;
			if (char_on_board == BLACK_M)
				black_score += 1;
			if (char_on_board == BLACK_K)
				black_score += 3;
		}
	}

	if ((turn == COMPUTER && computer.color == WHITE) || (turn == USER && user.color == WHITE))
		return (white_score - black_score);
	else
		return (black_score - white_score);
}

char** copy_game_board()
{
	int i, j;
	char board[BOARD_SIZE][BOARD_SIZE] = { 0 };
	for (i = 0; i < user.num_of_men; i ++)
	{
		if (user.color == WHITE)
			board[user.men_coordinate->row][user.men_coordinate->col] = WHITE_M;
		else
			board[user.men_coordinate->row][user.men_coordinate->col] = BLACK_M;
		user.men_coordinate = user.men_coordinate->next_coordinate;
	}

	for (i = 0; i < user.num_of_kings; i++)
	{
		if (computer.color == WHITE)
			board[computer.men_coordinate->row][computer.men_coordinate->col] = WHITE_M;
		else
			board[computer.men_coordinate->row][computer.men_coordinate->col] = BLACK_M;
		computer.men_coordinate = computer.men_coordinate->next_coordinate;
	}

	for (i = 0; i < user.num_of_kings; i++)
	{
		if (user.color == WHITE)
			board[user.kings_coordinate->row][user.kings_coordinate->col] = WHITE_M;
		else
			board[user.kings_coordinate->row][user.kings_coordinate->col] = BLACK_M;

		user.kings_coordinate = user.kings_coordinate->next_coordinate;
	}
	
	for (i = 0; i < user.num_of_kings; i++)
	{
		if (computer.color == WHITE)
			board[computer.kings_coordinate->row][computer.kings_coordinate->col] = WHITE_M;
		else
			board[computer.kings_coordinate->row][computer.kings_coordinate->col] = BLACK_M;

		computer.kings_coordinate = computer.kings_coordinate->next_coordinate;
	}

	return (board);
}

void copy_board_to_gameboard(board_t board)
{
	int i, j;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			game_board[i][j] = board[i][j];
		}
	}
}

int rec_minimax(char ** board, int depth, bool min_or_max)
{
	int bestValue, i, j;
//	char board[BOARD_SIZE][BOARD_SIZE];
	path_t** clone_path;
	clone_path = (path_t**)calloc(paths_number, sizeof(path_t*));

	

	// stop condiotion - max_depth / turn is winner
	if (depth >= (Minimax_Depth))
	{
		bestValue = scoring();
	}
	else if (min_or_max)
	{
		bestValue = -1000;
		path_t** clone_path;
		char** board;

		change_turn(turn);
		get_moves(turn);

		board = (char**)malloc(sizeof(char*));
		clone_path = (path_t**)calloc(paths_number, sizeof(path_t*));
		if ((clone_path == NULL) || (board == NULL))
		{
			printf("Error: fatal error during memory alocation, exiting.\n");
			return -1;
		}

		board = copy_game_board();
		clone_path = clone_path_arr();
		int n = paths_number;
		char** board;
		board = (char**)malloc(sizeof(char*));
		board = copy_game_board();
		for (i = 0; i < n; i++)
		{
			copy_board_to_gameboard(board);

			path_on_board(clone_path[i]);

			int max = rec_minimax(game_board, depth + 1, FALSE);
			if (max > bestValue)
			{
				bestValue = max;
				best_path = paths_arr[i]->head_position;
			}
			
		}
		return (bestValue);
	}
	else
	{
		bestValue = 1000;
		path_t** clone_path;
		char** board;

		change_turn(turn);
		get_moves(turn);
		
		board = (char**)malloc(sizeof(char*));
		clone_path = (path_t**)calloc(paths_number, sizeof(path_t*));
		if ((clone_path == NULL) || (board == NULL))
		{
			printf("Error: fatal error during memory alocation, exiting.\n");
			return -1;
		}

		board = copy_game_board();
		clone_path = clone_path_arr();
		int n = paths_number;
		char** board;
		board = (char**)malloc(sizeof(char*));
		board = copy_game_board();
		for (i = 0; i < n; i++)
		{
			copy_board_to_gameboard(board);

			path_on_board(clone_path[i]);
			int min = rec_minimax(game_board, depth + 1, TRUE);
			if (min < bestValue)
			{
				bestValue = min;
				best_path = clone_path[i]->head_position;
			}
		}
		return (bestValue);
	}
}

void minimax()
{
	int i, best_score, max_score, num;
	char** board;
	path_t** clone_path;
	clone_path = (path_t**)calloc(paths_number, sizeof(path_t*));
	if (clone_path == NULL)
	{
		printf("Error: fatal error during memory alocation, exiting.\n");
		return -1;
	}
	clone_path = NULL;
	num = 0;

	if (DEBUG)
	{
		//clear();
		set_user_color(WHITE);
		set_minimax_depth(2);

		State = GAME_STATE;
		//first_updating_MenKings_coordinate();
		turn = COMPUTER;

		get_moves(turn);
		path_t** newpaths = clone_path_arr();
	}
	original_board();
	print_board();
	back_up_players();

	best_score = -1;
	max_score = -1;
	get_moves(turn);
	clone_path = clone_path_arr();
	num = paths_number;
	board = copy_game_board();
	if (paths_number == 1)
	{
		best_path = paths_arr[0]->head_position;
	}
	else
	{
		for (i = 0; i < num; i++)
		{
			copy_board_to_gameboard(board);
			path_on_board(clone_path[i]);
			best_score = rec_minimax(game_board, 1, FALSE);
			if (best_score > max_score)
			{
				max_score = best_score;
			}
		}
	}
	
	return_player_to_original_satae();
	copy_board_to_gameboard(backup_board);
	print_board();

}

void print_tmp_board()
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			//printf("| %c ", game_board[i][j]);
			printf("| %c ", backup_board[j][i]);
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
type_e find_type(int row, int col)
{
	char char_on_board;
	type_e tool_type;

	char_on_board = EMPTY;
	tool_type = KING;

	if (use_tmp_board)
		char_on_board = tmp_board[row][col];
	if (!use_tmp_board)
		char_on_board = game_board[row][col];
	if ((char_on_board == WHITE_M) || (char_on_board == BLACK_M))
		tool_type = MAN;

	return tool_type;
}

color_e find_color(int row, int col)
{
	char char_on_board;
	color_e tool_color;

	char_on_board = EMPTY;
	tool_color = WHITE;

	if (use_tmp_board)
		char_on_board = tmp_board[row][col];
	if (!use_tmp_board)
		char_on_board = game_board[row][col];
	if ((char_on_board == WHITE_M) || (char_on_board == BLACK_M))
		tool_color = BLACK;

	return tool_color;
}

void change_turn(player_e turn)
{
	if (turn == COMPUTER)
	{
		turn = USER;
	}
	else
	{
		turn = COMPUTER;
	}
}