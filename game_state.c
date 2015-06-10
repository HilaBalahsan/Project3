#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int move(int row, int col, char* string)
{
	// Varibles
	int check_if_10, row_new, col_new;
	char tool, pre_tool;
	int i, len;
	color_e tool_color, pre_tool_col;
	type_e tool_type, pre_tool_type;
	bool enemy_pos, last_one;
	char *inputCopy, *token, *type;

	coordinate_t* node_to_delete, *list_to_change;

	node_to_delete = NULL;


	// Initialize
	len = strlen(string);
	tool_color = WHITE;

	node_to_delete = (coordinate_t*)malloc(sizeof(coordinate_t*));
	inputCopy = (char*)malloc(sizeof(char)*(strlen(string) + 1));
	if (inputCopy == NULL || node_to_delete == NULL)
	{
		printf("Error: standard function malloc failed, exiting.");
		return -1;
	}

	node_to_delete->next_coordinate = NULL;
	node_to_delete->previous_coordinate = NULL;
	strncpy(inputCopy, string, strlen(string));
	inputCopy[strlen(string)] = '\0';
	move_arr = NULL;
	move_number = 0;

	token = strtok(inputCopy, BIGGER);
	i = 0;
	while ((token != NULL)) {
		if (move_number < len) {
			update_moves_arr(token);
		}
		token = strtok(NULL, BIGGER);
		//i++;
	}

	/*input check*/
	if (move_number == 0)
	{
		free(string);
		free(inputCopy);
		return 1;
	}

	// find type
	pre_tool = game_board[row][col];
	if ((pre_tool == WHITE_K) || (pre_tool == BLACK_K))
	{
		pre_tool_type = KING;
	}
	else
	{
		pre_tool_type = MAN;
	}

	// find color
	if ((pre_tool == WHITE_K) || (pre_tool == WHITE_M))
	{
		pre_tool_col = WHITE;
	}
	else
	{
		pre_tool_col = BLACK;
	}

	while (i < move_number)
	{
		remove_disc(row, col, turn); // remove the disc from his current position

		col_new = alpha_to_num((int)move_arr[i][1]); // <x>

		check_if_10 = (int)move_arr[i][4] - 48;
		if (check_if_10 == 0)
		{
			row_new = 10;
		}
		else
		{
			row_new = (int)move_arr[i][3] - 49; // <y>
		}

		// find type
		tool = game_board[col_new][row_new];
		if (tool != EMPTY)
		{
			if ((tool == WHITE_K) || (tool == BLACK_K))
			{
				tool_type = KING;
			}
			else
			{
				tool_type = MAN;
			}
			// find color
			if ((tool == WHITE_K) || (tool == WHITE_M))
			{
				tool_color = WHITE;
			}
			else
			{
				tool_color = BLACK;
			}
		}



		enemy_pos = is_enemy_position(row_new, col_new);

		if (enemy_pos)
		{
			if (turn == COMPUTER)
			{
				remove_disc(row_new, col_new, USER);
			}
			else
			{
				remove_disc(row_new, col_new, COMPUTER);
			}
			node_to_delete->col = col_new;
			node_to_delete->row = row_new;
			list_to_change = creat_linkedList_pointer(pre_tool_type, turn);
			last_one = delete_link_from_linked_list(node_to_delete, list_to_change);
			if (last_one){
				if (turn = COMPUTER){
					if (computer.color == BLACK)
					{
						if ((tool == BLACK_M) || (tool == WHITE_M))
						{
							computer.men_coordinate = NULL;
						}
						else
						{
							computer.kings_coordinate = NULL;
						}
					}
					else
					{
						if ((tool == BLACK_M) || (tool == WHITE_M))
						{
							computer.men_coordinate = NULL;
						}
						else
						{
							computer.kings_coordinate = NULL;
						}
					}
				}
			}
		}

		else
		{
			set_disc(pre_tool, col_new, row_new, pre_tool_col, pre_tool_type);
			row = row_new;
			col = col_new;
		}
		i++;

	}

	free(inputCopy);
	//	free_linked_list(node_to_delete);
	return 1;
}


int get_moves(player_e player){

	coordinate_t *iterator;
	//releast the previouse arr.
	free_paths_arr(TRUE);
	maximal_path_weight = 0;

	paths_arr = (path_t**)calloc(BOARD_SIZE, sizeof(path_t*));
	if (paths_arr == NULL)
	{
		printf("get_move function - Failed to allocated memory");
		return -1;
	}
	capacity = BOARD_SIZE;

	iterator = creat_linkedList_pointer(MAN, player);
	get_move_helper(iterator, MAN);

	iterator = creat_linkedList_pointer(KING, player);
	get_move_helper(iterator, KING);

	if (iterator != NULL)
	{
		free_linked_list(iterator);
	}
	print_path_arr();
}

void get_move_helper(coordinate_t *itereting_node, type_e tool){

	int return_val;

	while (itereting_node != NULL)
	{
		if (tool == KING)
		{
			return_val = get_king_moves(itereting_node->row, itereting_node->col);
		}
		else
		{
			return_val = get_men_moves(itereting_node->row, itereting_node->col);
		}
		if (return_val == -1)
		{
			return -1;
		}

		itereting_node = itereting_node->next_coordinate;
	}

}

int get_men_moves(int curr_row, int curr_col){

	path_t *new_path;
	coordinate_t *moves_coor;
	direction_e prev_dir;
	step_t* step;
	int return_val;

	step = (step_t*)malloc(sizeof(step_t));
	if (step == NULL)
	{
		return -1;
	}
	step = initialize_step(step);
	new_path = (path_t*)malloc(sizeof(path_t));
	if (new_path == NULL)
	{
		printf("get_move function - Failed to allocated memory");
		return -1;
	}
	new_path->path_weight = 0;
	new_path->last_coordinate[0] = curr_row;
	new_path->last_coordinate[1] = curr_col;
	new_path->head_position = NULL;

	new_path->head_position = updating_linked_list(curr_row, curr_col, new_path->head_position);
	if (new_path->head_position == NULL)
	{
		return -1;
	}

	return_val = get_man_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, step, new_path);
	step = initialize_step(step);


	return_val = get_man_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, step, new_path);
	step = initialize_step(step);

	return_val = get_man_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, step, new_path);
	step = initialize_step(step);

	return_val = get_man_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, step, new_path);
	step = initialize_step(step);

	if (return_val == -1)
	{
		return -1;
	}

	free(step);
	return 1;
}

int get_man_moves_helper(direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path){

	path_t *right_up_clone, *right_down_clone, *left_up_clone, *left_down_clone;
	int returnval1, returnval2, returnval3;

	returnval1 = 0;
	returnval2 = 0;
	returnval3 = 0;

	//Halting conditions
	if (!is_valid_position(next_row, next_col))
	{
		if (new_path->head_position != NULL)
		{
			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				printf("Faild to update path array");
				return -1;
			}
		}
		return 1;
	}

	if ((is_become_king(next_row, next_col)) && ((step->is_first_step == TRUE) || (step->is_potntial_step == TRUE)))
	{
		new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
		if (new_path->head_position == NULL)
		{
			return -1;
		}
		return 1;
	}

	if ((step->is_first_step == TRUE) && (step->is_potntial_step == FALSE))
	{
		// arrive here if it the first step.
		step->is_first_step = FALSE;
		if (is_enemy_position(next_row, next_col))
		{
			step->is_potntial_step = TRUE;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;
			if (new_path->head_position == NULL)
			{
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, new_path);
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, new_path);
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, new_path);
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, new_path);
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				return -1;
			}

		}
		// not an enemy slot
		else if (is_valid_position(next_row, next_col))
		{
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				return -1;
			}

			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;

			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				return -1;
			}
		}
		// or the place is takken by my frient or the slot is out the board
		else
		{
			//user position can't go from here.
			free_path(new_path);
			return 1;
		}
	}
	else if ((step->is_first_step == FALSE) && (step->is_potntial_step == TRUE))
	{
		//arrive here if the previouse slot is belongs to enemy

		if (is_enemy_position(next_row, next_col))
		{
			// two enemy tools in a row can't go from here...
			free_path(new_path);
			return -1;
		}
		else if (is_valid_position(next_row, next_col))
		{
			step->is_potntial_step = FALSE;
			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				return -1;
			}
			// Calculate path weight
			new_path->path_weight += 1;
			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				return -1;
			}
			//continue looking for a eat in all direction
			//clone the path four times
			right_up_clone = clone_path(new_path);
			right_down_clone = clone_path(new_path);
			left_up_clone = clone_path(new_path);
			left_down_clone = clone_path(new_path);

			//Check if there are more enemies to kill.
			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, left_up_clone);
				returnval2 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, right_up_clone);
				returnval3 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, right_down_clone);

				//returnval3 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, left_down_clone);


				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, left_up_clone);
				returnval2 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, right_up_clone);
				//returnval2 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, right_down_clone);
				returnval3 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, left_down_clone);


				break;
			case DOWNRIGHT:
				//returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, left_up_clone);
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, right_up_clone);
				returnval3 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, left_down_clone);
				returnval2 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, right_down_clone);


				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, left_up_clone);
				//returnval2 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, right_up_clone);
				returnval2 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, right_down_clone);
				returnval3 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, left_down_clone);

				break;
			default:
				returnval1 = -1;
				break;
			}

			if ((returnval1 == -1) || (returnval2 == -1) || (returnval3 == -1))
			{
				return -1;
			}
		}
	}
	else  //Arrive here when looking for another eat
	{
		if (is_enemy_position(next_row, next_col))
		{
			step->is_potntial_step = TRUE;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, new_path);
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, new_path);
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, new_path);
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, new_path);
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				return -1;
			}
		}
		else
		{
			free_path(new_path);
			return 1;
		}

	}

	return 1;
}

int get_king_moves(int curr_row, int curr_col){

	path_t *new_path;
	coordinate_t *moves_coor;
	direction_e prev_dir;
	step_t* step;
	int return_val;

	step = (step_t*)malloc(sizeof(step_t));
	if (step == NULL)
	{
		return -1;
	}
	step = initialize_step(step);

	new_path = (path_t*)malloc(sizeof(path_t));
	if (new_path == NULL)
	{
		printf("get_move function - Failed to allocated memory");
		return -1;
	}
	new_path->head_position = NULL;
	new_path->path_weight = 0;

	//First coordinate is the starting slot.
	new_path->head_position = updating_linked_list(curr_row, curr_col, new_path->head_position);
	if (new_path->head_position == NULL)
	{
		return -1;
	}

	return_val = get_king_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, step, new_path);
	step = initialize_step(step);

	return_val = get_king_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, step, new_path);
	step = initialize_step(step);

	return_val = get_king_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, step, new_path);
	step = initialize_step(step);

	return_val = get_king_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, step, new_path);
	step = initialize_step(step);

	free(step);

	if (return_val == -1)
	{
		return -1;
	}
	return 1;
}

int get_king_moves_helper(direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path){

	path_t *right_up_clone, *right_down_clone, *left_up_clone, *left_down_clone;
	int returnval1, returnval2, returnval3;

	right_up_clone = NULL;
	right_down_clone = NULL;
	left_up_clone = NULL;
	left_down_clone = NULL;

	//Halting conditions
	if (!is_valid_position(next_row, next_col))
	{
		if (new_path->head_position != NULL)
		{
			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;
			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				printf("Faild to update path array");
			}
			return -1;

		}
		return 1;
	}
	if ((step->is_first_step == TRUE) && (step->is_potntial_step == FALSE))
	{
		//arrive here if it first step or the previous slot is free

		step->is_first_step = FALSE;
		if (is_enemy_position(next_row, next_col))
		{
			step->is_potntial_step = TRUE;
			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, new_path);
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, new_path);
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, new_path);
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, new_path);
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				return -1;
			}
		}
		// The slot is not belongs to enemy
		else if (is_valid_position(next_row, next_col))
		{
			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_king_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, new_path);
				break;
			case UPLEFT:
				returnval1 = get_king_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, new_path);
				break;
			case DOWNRIGHT:
				returnval1 = get_king_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, new_path);
				break;
			case DOWNLEFT:
				returnval1 = get_king_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, new_path);
				break;
			default:
				returnval1 = -1;
				break;
			}
		}
		else       // it is not valid slot
		{
			if (new_path->head_position == NULL)
			{
				free_path(new_path);
			}
			return 1;
		}
	}
	else if ((step->is_first_step == FALSE) && (step->is_potntial_step == TRUE))
	{
		//arrive here if the previous slot is belong to enemy

		returnval1 = get_man_moves_helper(dir, next_row, next_col, step, new_path);
		if (returnval1 == -1)
		{
			return -1;
		}
	}

	return 1;
}
