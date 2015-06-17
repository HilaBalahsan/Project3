#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int scoring(player_e turn){   ///change!! run over lists and not all board!!!!

	int score;
	player_t *enemy, *mine;

	if (is_a_winner(turn))
	{
		return 100;
	}
	if (is_a_winner(turn ^ 1))
	{
		return -100;
	}

	if (turn == USER)
	{
		mine = &user;
		enemy = &computer;
	}
	else
	{
		mine = &computer;
		enemy = &user;
	}
	score = (mine->num_of_men) 
		- (enemy->num_of_men) 
		+ (3)*(mine->num_of_kings) 
		- ((3)*(enemy->num_of_kings));

	return score;
}


int rec_minimax(int depth, player_e player)
{
	int score, paths_num, i, val, bestVal, bestIndex;
	path_t** local_paths;
	char board[BOARD_SIZE][BOARD_SIZE] = {{0}};
	
	score = scoring(player);
	
	if ((depth == 0) || (score == 100) || (score == -100))
	{
		//This is a terminal game state.
		return (score*-1);
	}

	// claculate all possible steps.
	get_moves(player);

	//save local copy of the global moves array.
	local_paths = clone_path_arr();
	paths_num = paths_number;

	if (player == COMPUTER) // maximazing.
	{
		bestVal = -100;
		bestIndex = 0;

		for (i = 0; i < paths_num; i++)
		{
			//save board
			memcpy(&board, &game_board, (BOARD_SIZE*BOARD_SIZE));

			//perform move
			perform_move(local_paths[i]->head_position, player);

			//recursive call
			val = rec_minimax((depth - 1), (player ^ 1)); //xor to toggle player. 

			//update maximum
			if (val > bestVal)
			{
				bestVal = val;
				bestIndex = i;
			}

			//recover board
			memcpy(&game_board, &board, (BOARD_SIZE*BOARD_SIZE));
			first_updating_MenKings_coordinate();
		}

		if (depth == Minimax_Depth) //upper call.
		{
			//perform best move

			perform_move(local_paths[bestIndex]->head_position, player);
			print_message(COMPUTER_MOVE);
			print_single_path(local_paths[bestIndex]);

			for (i = 0; i < paths_num; i++)
			{
				free_path(&local_paths[i]);
			}

			free(local_paths);
			return 1;
		}
		else
		{
			for (i = 0; i < paths_num; i++)
			{
				free_path(&local_paths[i]);
			}

			free(local_paths);
			return bestVal;
		}
	}
	else //minimazing.
	{
		bestVal = 100;

		for (i = 0; i < paths_num; i++)
		{
			//save board
			memcpy(&board, &game_board, (BOARD_SIZE*BOARD_SIZE));

			//perform move
			perform_move(local_paths[i]->head_position, player);
			free_path(&local_paths[i]);

			//recursive call
			val = rec_minimax((depth - 1), (player ^ 1)); //xor to toggle player. 

			//update minimum
			if(val < bestVal)
			{
				bestVal = val;
			}

			//recover board
			memcpy(&game_board, &board, (BOARD_SIZE*BOARD_SIZE));
			first_updating_MenKings_coordinate();
		}

		for (i = 0; i < paths_num; i++)
		{
			free_path(&local_paths[i]);
		}
		free(local_paths);

		return bestVal;
	}
}

void minimax()
{
	// Initiate recursive call to minimax.
	rec_minimax(Minimax_Depth, COMPUTER);
}