#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*Functions that are used during the game*/

/**
* move
* input : int row, int col, char* coo_stream
* output : int move
* functionality : move disc to other place on the board
* return : -1 if the was memory allocation problems, 1 seccesfull movem , the was a mistake (wrong end posiotion, the was no disc at the given coordinate) but the game should go on
* updates TURN's men/kings list
*/
int move(int row, int col, char* coo_stream){

	char ch, source_slot;
	int len , i , des_col , des_row;
	path_t* user_input_path;
	coordinate_t *head, *iter;
	color_e curr_color;

	user_input_path = calloc(1, sizeof(path_t));
	if (user_input_path == NULL)
	{
		perror_message("calloc");
		return -1;
	}

	i = 0;
	len = strlen(coo_stream);
	
	/*add source position to path.*/
	user_input_path->head_position = updating_linked_list(row, col, user_input_path->head_position);
	head = user_input_path->head_position;

	while(i < len)
	{
		ch = coo_stream[i];
		if (isalpha(ch))
		{
			des_col = alpha_to_num(ch);
			if ((coo_stream[i + 3] == '0') && (coo_stream[i + 4] == '>'))
			{
				des_row = 9;
				i += 3;
			}
			else if (coo_stream[i + 3] != '>')
			{
				print_message(WRONG_POSITION);
				free_path(&user_input_path);
				return 2;
			}
			else
			{
				des_row = (((int) (coo_stream[i + 2])) - 49);
				i += 2;
			}
			
			head = updating_linked_list(des_row, des_col, head);
		}
		else
		{
			i++;
		}
	}

	/*Validate that all positions in the path are legal.*/
	iter = head;
	while (iter != NULL)
	{
		if (!is_valid_position(iter->row, iter->col))
		{
			print_message(WRONG_POSITION);
			free_path(&user_input_path);
			return 2;
		}

		iter = iter->next_coordinate;
	}
	/*Validate source position.*/
	source_slot = game_board[row][col];
	curr_color = user.color;
	if (source_slot == EMPTY)
	{
		print_message(NO_DICS);
		free_path(&user_input_path);
		return 2;
	}
	else if ((source_slot == WHITE_M) || (source_slot == WHITE_K))
	{
		if (curr_color == BLACK)
		{
			print_message(NO_DICS);
			free_path(&user_input_path); 
			return 2;
		}
	}
	else
	{
		if (curr_color == WHITE)
		{
			print_message(NO_DICS);
			free_path(&user_input_path); 
			return 2;
		}
	}

	if (!is_legal_move(user_input_path))
	{
		print_message(ILLEGAL_MOVE);
		free_path(&user_input_path); 
		return 2;
	}
	else
	{
		perform_move(user_input_path->head_position, Turn);
	}
	
	free_path(&user_input_path); 
	return 1;
}

/**
* perform_move
* input : coordinate_t* move, player_e turn
* output :noting
* functionality : move disc to other place on the board and update turn's lists
* help function to move
*/
void perform_move(coordinate_t* move, player_e turn)
{
	int row, col, colAvg, rowAvg;
	char slot, tool;
	player_t *player, *enemy;
	coordinate_t *link2delete;
	bool isSrc = TRUE,isLastLink = FALSE;


	tool = game_board[move->row][move->col];
	colAvg = move->col;
	rowAvg = move->row;

	if (turn == USER)
	{
		player = &user;
		enemy  = &computer;
	}
	else
	{
		player = &computer;
		enemy  = &user;
	}

	while (move != NULL)
	{
		row = move->row;
		col = move->col;

		if (move->previous_coordinate != NULL) /*not first*/
		{
			colAvg = (move->previous_coordinate->col + col) / 2;
			rowAvg = (move->previous_coordinate->row + row) / 2;
		}

		if ((colAvg == col) || (rowAvg == row) 
			|| (colAvg == move->previous_coordinate->col) 
			|| (rowAvg == move->previous_coordinate->row))
		{
			if (isSrc) 
			{
				isSrc = FALSE;
				slot = game_board[rowAvg][colAvg];
				if ((slot == BLACK_M) || (slot == WHITE_M))
				{
					player->num_of_men--;
					link2delete = pointer_to_link(rowAvg, colAvg, player->men_coordinate);
				}
				else
				{
					player->num_of_kings--;
					link2delete = pointer_to_link(rowAvg, colAvg, player->kings_coordinate);
				}
				game_board[rowAvg][colAvg] = EMPTY;
				isLastLink = delete_link_from_linked_list(link2delete);
				if (isLastLink)
				{
					if (player->num_of_men == 0)
					{
						player->men_coordinate = NULL;
					}
					if (player->num_of_kings == 0)
					{
						player->kings_coordinate = NULL;
					}
				}
			}
		}
		else
		{
			slot = game_board[rowAvg][colAvg];
			if ((slot == BLACK_M) || (slot == WHITE_M))
			{
				enemy->num_of_men--;
				link2delete = pointer_to_link(rowAvg, colAvg, enemy->men_coordinate);
			}
			else
			{
				enemy->num_of_kings--;
				link2delete = pointer_to_link(rowAvg, colAvg, enemy->kings_coordinate);
			}
			game_board[rowAvg][colAvg] = EMPTY;
			isLastLink = delete_link_from_linked_list(link2delete);
			if (isLastLink)
			{
				if (enemy->num_of_men == 0)
				{
					enemy->men_coordinate = NULL;
				}
				if (enemy->num_of_kings == 0)
				{
					enemy->kings_coordinate = NULL;
				}
			}
		}

		if (move->next_coordinate == NULL) /*last cordinate.*/
		{
			/*Check if this is a King position.*/
			if (is_become_king(move->row, move->col, turn))
			{
				if (tool == BLACK_M)
				{
					tool = BLACK_K;
				}
				else
				{
					tool = WHITE_K;
				}
			}
			if ((tool == BLACK_M) || (tool == WHITE_M))
			{
				player->num_of_men++;
				player->men_coordinate = updating_linked_list(move->row, move->col, player->men_coordinate);
			}
			else
			{
				player->num_of_kings++;
				player->kings_coordinate = updating_linked_list(move->row, move->col, player->kings_coordinate);
			}

			game_board[move->row][move->col] = tool;
		}

		move = move->next_coordinate;
	}
}

/**
* get_moves
* input :player_e player
* output :noting
* functionality : returns all possible moves of given player (user or computer)
* returns : -1 if the was memory allocation problems, 1 if there were no problems in the function  
*/
int get_moves(player_e player){

	coordinate_t *iterator;
	/*releast the previouse arr.*/
	free_paths_arr(TRUE);
	maximal_path_weight = 0;

	paths_arr = calloc(BOARD_SIZE, sizeof(path_t*));
	if (paths_arr == NULL)
	{
		perror_message("calloc");
		return -1;
	}
	capacity = BOARD_SIZE;

	iterator = creat_linkedList_pointer(MAN, player);
	get_move_helper(iterator, MAN, player);

	iterator = creat_linkedList_pointer(KING, player);
	get_move_helper(iterator, KING, player);
	
	return 1;
}

/**
* get_move_helper
* input :coordinate_t *itereting_node, type_e tool,player_e turn
* output :int get_move_helper
* functionality : returns all possible moves of given player (user or computer)
* returns : -1 if the was memory allocation problems, 1 if there were no problems in the function
* helps to get_moves
*/
int get_move_helper(coordinate_t *itereting_node, type_e tool,player_e turn){

	int return_val;

	while (itereting_node != NULL)
	{
		if (tool == KING)
		{
			return_val = get_king_moves(itereting_node->row, itereting_node->col, turn);
		}
		else
		{
			return_val = get_men_moves(itereting_node->row, itereting_node->col, turn);
		}
		if (return_val == -1)
		{
			return -1;
		}

		itereting_node = itereting_node->next_coordinate;
	}
	
	return 1;
}

/**
* get_men_moves
* input :int curr_row, int curr_col, player_e turn
* output :int get_men_moves
* functionality : returns all possible moves for given player (user or computer) from given man coordinate on game_board
* returns : -1 if the was memory allocation problems, 1 if there were no problems in the function
* helps to get_moves_helper
*/
int get_men_moves(int curr_row, int curr_col, player_e turn) {

	path_t *new_path;
	step_t step = { 1, 0 };
	color_e current_color;
	int return_val1, return_val2, return_val3, return_val4;

	new_path = (path_t*)malloc(sizeof(path_t));
	if (new_path == NULL)
	{
		perror_message("malloc");
		return -1;
	}
	new_path->path_weight = 0;
	new_path->head_position = NULL;

	new_path->head_position = updating_linked_list(curr_row, curr_col, new_path->head_position);
	if (new_path->head_position == NULL)
	{
		free_path(&new_path); 
		return -1;
	}

	if ((game_board[curr_row][curr_col] == WHITE_K) || (game_board[curr_row][curr_col] == WHITE_M))
	{
		current_color = WHITE;
	}
	else
	{
		current_color = BLACK;
	}

	if (current_color == WHITE)
	{
		return_val1 = get_man_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, &step, clone_path(new_path), turn);
		initialize_step(&step);

		return_val2 = get_man_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, &step, clone_path(new_path), turn);

		step.is_first_step = FALSE;
		step.is_potntial_step = FALSE;
		return_val3 = get_man_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, &step, clone_path(new_path), turn);

		step.is_first_step = FALSE;
		step.is_potntial_step = FALSE;
		return_val4 = get_man_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, &step, clone_path(new_path), turn);
		initialize_step(&step);

	}
	else
	{
		return_val1 = get_man_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, &step, clone_path(new_path), turn);
		initialize_step(&step);

		return_val2 = get_man_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, &step, clone_path(new_path), turn);

		step.is_first_step = FALSE;
		step.is_potntial_step = FALSE;
		return_val3 = get_man_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, &step, clone_path(new_path), turn);

		step.is_first_step = FALSE;
		step.is_potntial_step = FALSE;
		return_val4 = get_man_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, &step, clone_path(new_path), turn);
	}
	free_path(&new_path); 

	if ((return_val1 == -1) || (return_val2 == -1) || (return_val3 == -1) || (return_val4 == -1))
	{
		return -1;
	}
	return 1;
}

/**
* get_man_moves_helper
* input :direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path, player_e turn
* output :int get_man_moves_helper
* functionality : helps get_men_moves to return all possible moves to turn
* returns : -1 if the was memory allocation problems, 1 if there were no problems in the function
* helps to get_men_moves
*/
int get_man_moves_helper(direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path, player_e turn) {

	int returnval1, returnval2, returnval3;
	
	returnval1 = 0;
	returnval2 = 0;
	returnval3 = 0;

	/*Halting conditions*/
	if (!is_valid_position(next_row, next_col))
	{
		free_path(&new_path);
		return 1;
	}

	if ((is_become_king(next_row, next_col, turn)) && ((step->is_first_step == TRUE) || (step->is_potntial_step == TRUE)))
	{
		if (step->is_potntial_step)
		{
			new_path->path_weight += 1;
		}
		new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
		if (new_path->head_position == NULL)
		{
			free_path(&new_path);
			return -1;
		}
		returnval1 = update_paths_array(clone_path(new_path));
		if (returnval1 == -1)
		{
			free_path(&new_path);
			return -1;
		}

		free_path(&new_path); 
		return 1;
	}
	
	if ((step->is_first_step == TRUE) && (step->is_potntial_step == FALSE))
	{
		/*arrive here if it the first step.*/
		step->is_first_step = FALSE;

		if (is_enemy_position(next_row, next_col) && (!is_at_the_edge(next_row, next_col)))
		{
			step->is_potntial_step = TRUE;

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path), turn);
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path), turn);
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				free_path(&new_path);
				return -1;
			}

		}
		/*not an enemy slot*/
		else if (is_empty_position(next_row, next_col))
		{
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				free_path(&new_path);
				return -1;
			}

			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				free_path(&new_path);
				return -1;
			}
		}
	}
	else if ((step->is_first_step == FALSE) && (step->is_potntial_step == TRUE))
	{
		/*arrive here if the previouse slot is belongs to enemy*/

		if (is_enemy_position(next_row, next_col))
		{
			/*two enemy tools in a row can't go from here...*/
			free_path(&new_path);
			return 1;
		}
		else if (is_empty_position(next_row, next_col))
		{
			step->is_potntial_step = FALSE;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				free_path(&new_path);
				return -1;
			}
			/*Calculate path weight*/
			new_path->path_weight += 1;
			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				free_path(&new_path);
				return -1;
			}
			/*continue looking for a eat in all direction*/
			/*clone the path four times*/


			/*Check if there are more enemies to kill.*/
			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path), turn);
				step->is_first_step = FALSE; step->is_potntial_step = FALSE;
				returnval2 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path), turn);
				step->is_first_step = FALSE; step->is_potntial_step = FALSE;
				returnval3 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path), turn);
				step->is_first_step = FALSE; step->is_potntial_step = FALSE;
				returnval2 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path), turn);
				step->is_first_step = FALSE; step->is_potntial_step = FALSE;
				returnval3 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path), turn);
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path), turn);
				step->is_first_step = FALSE; step->is_potntial_step = FALSE;
				returnval3 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path), turn);
				step->is_first_step = FALSE; step->is_potntial_step = FALSE;
				returnval2 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path), turn);
				step->is_first_step = FALSE; step->is_potntial_step = FALSE;
				returnval2 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path), turn);
				step->is_first_step = FALSE; step->is_potntial_step = FALSE;
				returnval3 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path), turn);

				break;
			default:
				returnval1 = -1;
				break;
			}

			if ((returnval1 == -1) || (returnval2 == -1) || (returnval3 == -1))
			{
				free_path(&new_path);
				return -1;
			}
		}
	}
	else  /*Arrive here when looking for another eat*/
	{
		if (is_enemy_position(next_row, next_col) && (!is_at_the_edge(next_row, next_col)))
		{
			step->is_potntial_step = TRUE;

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path), turn);
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path), turn);
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				free_path(&new_path);
				return -1;
			}
		}
	}
	free_path(&new_path);
	return 1;
}

/**
* get_king_moves
* input :int curr_row, int curr_col, player_e turn
* output :int get_king_moves
* functionality : returns all possible moves for given player (user or computer) from given king coordinate on game_board
* returns : -1 if the was memory allocation problems, 1 if there were no problems in the function
* helps to get_moves_helper
*/
int get_king_moves(int curr_row, int curr_col, player_e turn) {

	path_t *new_path;
	step_t step = { 1, 0 };
	int return_val;

	new_path = (path_t*)malloc(sizeof(path_t));
	if (new_path == NULL)
	{
		perror_message("malloc");
		return -1;
	}
	new_path->head_position = NULL;
	new_path->path_weight = 0;

	/*First coordinate is the starting slot.*/
	new_path->head_position = updating_linked_list(curr_row, curr_col, new_path->head_position);
	if (new_path->head_position == NULL)
	{
		free_path(&new_path); 
		return -1;
	}

	return_val = get_king_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, &step, clone_path(new_path), turn);
	initialize_step(&step);

	return_val = get_king_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, &step, clone_path(new_path), turn);
	initialize_step(&step);

	return_val = get_king_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, &step, clone_path(new_path), turn);
	initialize_step(&step);

	return_val = get_king_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, &step, clone_path(new_path), turn);
	initialize_step(&step);

	if (return_val == -1)
	{
		free_path(&new_path); 
		return -1;
	}
	free_path(&new_path); 
	return 1;
}

/**
* get_king_moves_helper
* input :direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path, player_e turn
* output :int get_king_moves_helper
* functionality : helps get_men_moves to return all possible moves to turn
* returns : -1 if the was memory allocation problems, 1 if there were no problems in the function
* helps to get_king_moves
*/
int get_king_moves_helper(direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path, player_e turn) {

	int returnval1;

	returnval1 = 0;


	/*Halting conditions*/
	if (!is_valid_position(next_row, next_col))
	{
		free_path(&new_path);
		return 1;
	}
	if ((step->is_first_step == TRUE) && (step->is_potntial_step == FALSE))
	{

		/*arrive here if it first step or the previous slot is free*/

		if (is_enemy_position(next_row, next_col))
		{
			step->is_first_step = FALSE;
			step->is_potntial_step = TRUE;

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path), turn);
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path), turn);
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				free_path(&new_path);
				return -1;
			}
		}
		/*The slot is not belongs to enemy*/
		else if (is_empty_position(next_row, next_col))
		{
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				free_path(&new_path); 
				return -1;
			}

			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				free_path(&new_path);
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_king_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case UPLEFT:
				returnval1 = get_king_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path), turn);
				break;
			case DOWNRIGHT:
				returnval1 = get_king_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path), turn);
				break;
			case DOWNLEFT:
				returnval1 = get_king_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path), turn);
				break;

			default:
				returnval1 = -1;
				break;
			}
		}
	}
	else if ((step->is_first_step == FALSE) && (step->is_potntial_step == TRUE))
	{
		/*arrive here if the previous slot is belong to enemy*/

		returnval1 = get_man_moves_helper(dir, next_row, next_col, step, new_path, turn);
		if (returnval1 == -1)
		{
			free_path(&new_path);
			return -1;
		}
	}

	free_path(&new_path);
	return 1;
}
