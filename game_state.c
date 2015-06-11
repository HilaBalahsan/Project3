#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int move(int row, int col, String coo_stream){

	char ch;
	int len , i , des_col , des_row , return_val;
	bool first_step;
	path_t* user_input_path;

	i = 0;
	first_step = TRUE;
	len = strlen(coo_stream);
	
	while(i < len)
	{
		if (isalpha(ch))
		{
			des_col = alpha_to_num(ch);
			if (coo_stream[i + 3] == 0)
			{
				des_row = 9;
				i += 3;
			}
			else
			{
				des_row = (int)coo_stream[i + 2];
				i += 2;
			}
			if (first_step)
			{
				return_val = check_and_build(row, col, des_row, des_col, first_step);
				first_step = FALSE;
				if (return_val == -1)
				{
					return -1;
				}
			}
			return_val = check_and_build(row, col, des_row, des_col, first_step);
			if (return_val == -1)
			{
				return -1;
			}
			user_input_path = updating_linked_list(des_row, des_col, user_input_path);
		}
		 else
		 {
			 i++;
		 }
	}

	if (!is_legal_move)
	{
		printf(ILLEGAL_MOVE);
	}
	else
	{
		make_user_path(user_input_path->head_position);
	}
}

int get_moves(player_e player){

	coordinate_t *iterator;
	//releast the previouse arr.
	free_paths_arr(TRUE);
	maximal_path_weight = 0;

	paths_arr = calloc(BOARD_SIZE, sizeof(path_t*));
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
	step_t step = { 1, 0 };
	int return_val;

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

	return_val = get_man_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, &step, clone_path(new_path));
	initialize_step(&step);

	return_val = get_man_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, &step, clone_path(new_path));
	initialize_step(&step);

	return_val = get_man_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, &step, clone_path(new_path));
	initialize_step(&step);

	return_val = get_man_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, &step, clone_path(new_path));
	initialize_step(&step);

	if (return_val == -1)
	{
		return -1;
	}

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
				free_path(new_path);
				return -1;
			}
		}
		//else
	//	{
			//free_path(new_path);
	//	}
		free_path(new_path);
		return 1;
	}

	if ((is_become_king(next_row, next_col)) && ((step->is_first_step == TRUE) || (step->is_potntial_step == TRUE)))
	{
		new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
		if (new_path->head_position == NULL)
		{
			free_path(new_path);
			return -1;
		}
		returnval1 = update_paths_array(clone_path(new_path));
		if (returnval1 == -1)
		{
			free_path(new_path);
			return -1;
		}
		return 1;
	}

	if ((step->is_first_step == TRUE) && (step->is_potntial_step == FALSE))
	{
		// arrive here if it the first step.
		step->is_first_step = FALSE;
		if (is_enemy_position(next_row, next_col) && (!is_at_the_edge(next_row, next_col)))
		{
			step->is_potntial_step = TRUE;
			if (!is_at_the_edge(next_row, next_col))
			{
				new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
				new_path->last_coordinate[0] = next_row;
				new_path->last_coordinate[1] = next_col;
				if (new_path->head_position == NULL)
				{
					free_path(new_path);
					return -1;
				}
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path));
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path));
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path));
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path));
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				free_path(new_path);
				return -1;
			}

		}
		// not an enemy slot
		else if (is_empty_position(next_row, next_col))
		{
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				free_path(new_path);
				return -1;
			}

			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;

			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				free_path(new_path);
				return -1;
			}
		}
		// place is takken by my frient or the slot is out the board
		//else
		//{
		//	free_path(new_path);
		//	return 1;
		//}
	}
	else if ((step->is_first_step == FALSE) && (step->is_potntial_step == TRUE))
	{
		//arrive here if the previouse slot is belongs to enemy

		if (is_enemy_position(next_row, next_col))
		{
			// two enemy tools in a row can't go from here...
			//free_path(new_path);
			free_path(new_path);
			return 1;
		}
		else if (is_empty_position(next_row, next_col))
		{
			step->is_potntial_step = FALSE;
			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				free_path(new_path);
				return -1;
			}
			// Calculate path weight
			new_path->path_weight += 1;
			returnval1 = update_paths_array(clone_path(new_path));
			if (returnval1 == -1)
			{
				free_path(new_path);
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
				free_path(new_path);
				return -1;
			}
		}
	}
	else  //Arrive here when looking for another eat
	{
		if (is_enemy_position(next_row, next_col) && (!is_at_the_edge(next_row, next_col)))
		{
			step->is_potntial_step = TRUE;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				free_path(new_path);
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path));
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path));
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path));
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path));
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				free_path(new_path);
				return -1;
			}
		}
		//else
	//	{
		//	free_path(new_path);
	//		return 1;
	//	}

	}
	free_path(new_path);
	return 1;
}

int get_king_moves(int curr_row, int curr_col){

	path_t *new_path;
	coordinate_t *moves_coor;
	direction_e prev_dir;
	step_t step = { 1, 0 };
	int return_val;

	new_path = (path_t*)malloc(sizeof(path_t));
	if (new_path == NULL)
	{
		printf("get_move function - Failed to allocated memory");
		return -1;
	}
	new_path->head_position = NULL;
	new_path->last_coordinate[0] = curr_row;
	new_path->last_coordinate[1] = curr_col;
	new_path->path_weight = 0;

	//First coordinate is the starting slot.
	new_path->head_position = updating_linked_list(curr_row, curr_col, new_path->head_position);
	if (new_path->head_position == NULL)
	{
		return -1;
	}

	return_val = get_king_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, &step, clone_path(new_path));
	initialize_step(&step);

	return_val = get_king_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, &step, clone_path(new_path));
	initialize_step(&step);

	return_val = get_king_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, &step, clone_path(new_path));
	initialize_step(&step);

	return_val = get_king_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, &step, clone_path(new_path));
	initialize_step(&step);

	if (return_val == -1)
	{
		return -1;
	}
	return 1;
}

int get_king_moves_helper(direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path){

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
				free_path(new_path);
				return -1;
			}
		}
		//else
		//{
			//free_path(new_path);
		//}
		free_path(new_path);
		return 1;
	}
	if ((step->is_first_step == TRUE) && (step->is_potntial_step == FALSE))
	{

		//arrive here if it first step or the previous slot is free

		//step->is_first_step = FALSE;

		if (is_enemy_position(next_row, next_col) && (!is_at_the_edge(next_row, next_col)))
		{
			step->is_first_step = FALSE;
			step->is_potntial_step = TRUE;
			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;
			new_path->head_position = updating_linked_list(next_row, next_col, new_path->head_position);
			if (new_path->head_position == NULL)
			{
				free_path(new_path);
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_man_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path));
				break;
			case UPLEFT:
				returnval1 = get_man_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path));
				break;
			case DOWNRIGHT:
				returnval1 = get_man_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path));
				break;
			case DOWNLEFT:
				returnval1 = get_man_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path));
				break;
			default:
				returnval1 = -1;
				break;
			}

			if (returnval1 == -1)
			{
				free_path(new_path);

				return -1;
			}
		}
		// The slot is not belongs to enemy
		else if (is_empty_position(next_row, next_col))
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
				returnval1 = get_king_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, clone_path(new_path));
				break;
			case UPLEFT:
				returnval1 = get_king_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, clone_path(new_path));
				break;
			case DOWNRIGHT:
				returnval1 = get_king_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, clone_path(new_path));
				break;
			case DOWNLEFT:
				returnval1 = get_king_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, clone_path(new_path));
				break;

			default:
				returnval1 = -1;
				break;
			}
		}
	}
		// place is takken by my frient or the slot is out the board

	//	else
	//	{
	//		free_path(new_path);
	//	}
	else if ((step->is_first_step == FALSE) && (step->is_potntial_step == TRUE))
	{
		//arrive here if the previous slot is belong to enemy

		returnval1 = get_man_moves_helper(dir, next_row, next_col, step, new_path);
		if (returnval1 == -1)
		{
			free_path(new_path);
			return -1;
		}
	}

	free_path(new_path);
	return 1;
}
