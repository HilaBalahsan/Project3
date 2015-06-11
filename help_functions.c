#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


int check_and_build(int row,int col,int des_row, int des_col , path_t* user_path_input){
	char source_slot, des_slot;
	color_e curr_color;

	if (turn == USER)
	{
		curr_color = user.color;
	}
	else
	{
		curr_color = computer.color;
	}

	if ((des_row == -1) && (des_col == -1))
	{
		if ((source_slot == WHITE_M) || (source_slot == WHITE_K))
		{
			if (curr_color == BLACK)
			{
				printf(NO_DICS);
				return -1;
			}
		}
		else
		{
			if (curr_color == WHITE)
			{
				printf(NO_DICS);
				return -1;
			}
		}
		user_path_input->head_position = updating_linked_list(row, col, user_path_input);
		user_path_input->last_coordinate[0] = row;
		user_path_input->last_coordinate[1] = col;
	}

	source_slot = game_board[row][col];
	des_slot = game_board[des_row][des_col];

	if (!is_valid_position(des_row, des_col))
	{
		printf(WRONG_POSITION);
		return -1;
	}
	else
	{
		user_path_input->head_position = updating_linked_list(des_row, des_col, user_path_input);
		user_path_input->last_coordinate[0] = des_row;
		user_path_input->last_coordinate[1] = des_col;
	}

}

bool compare_two_paths(path_t* path_from_arr, path_t* user_input_path) {
	bool equal = TRUE;

	if ((path_from_arr->last_coordinate[0] != user_input_path->last_coordinate[0]) ||
		(path_from_arr->last_coordinate[1] != user_input_path->last_coordinate[1]))
	{
		equal = FALSE;
	}
	else
	{

	}


	while ((path_from_arr->head_position != NULL) && (user_input_path->head_position != NULL) && equal)
	{
		if ((path_from_arr->head_position->row == user_input_path->head_position->row) &&
			(path_from_arr->head_position->col == user_input_path->head_position->col))
			{
			path_from_arr->head_position = path_from_arr->head_position->next_coordinate;
			path_from_arr->head_position = path_from_arr->head_position->next_coordinate;
			}
			else
			{
				equal = FALSE;
			}
	}

	return equal;
}

void make_user_path(coordinate_t* move)
{
	int row, col;
	char slot;
	color_e enemy_color;
	coordinate_t* link_do_delete;

	game_board[move->row][move->col] = EMPTY;
	move = move->next_coordinate;
	enemy_color = computer.color;

	while (move != NULL)
	{
		row = move->row;
		col = move->col;

		if (is_empty_position(row, col))
		{
			move = move->next_coordinate;
		}
		else
		{
			slot = game_board[row][col];
			if ((slot == BLACK_M) || (slot == WHITE_M))
			{
				computer.num_of_men--;
				link_do_delete = pointer_to_link(row, col, computer.men_coordinate);
			}
			else
			{
				computer.num_of_kings--;
				link_do_delete = pointer_to_link(row, col, computer.kings_coordinate);
			}
			delete_link_from_linked_list(link_do_delete);
		}
	}

}

int update_paths_array(path_t* new_path)
{
	if (new_path == NULL)
	{
		return -1;
	}

	if (new_path->path_weight == maximal_path_weight)
	{
		paths_arr[paths_number] = new_path;
		paths_number++;
	}
	else if (new_path->path_weight > maximal_path_weight)
	{
		free_paths_arr(FALSE);
		paths_arr[paths_number] = new_path;
		paths_number++;
		maximal_path_weight = new_path->path_weight;
	}
	else
	{
		free_path(new_path);
	}

	if (paths_number == capacity)
	{
		paths_arr = (path_t**)realloc(paths_arr, sizeof(path_t) * 2);

		if (paths_arr == NULL)
		{
			printf("Error: fatal error during memory alocation, exiting.\n");
			return -1;
		}
	}
	return 1;
}

void print_path_arr(){
	int i;
	if (paths_arr != NULL)
	{
		for (i = 0; i < paths_number; i++)
		{
			print_single_path(paths_arr[i]);
		}
	}
}

void print_coordinate_list(coordinate_t* list_to_print)
{
	char col;
	int row;

	while (list_to_print != NULL)
	{
		col = alpha_to_num(list_to_print->col);
		row = list_to_print->row;

		if (list_to_print->next_coordinate == NULL)
		{
			printf(" <%c,%d> ", col, row);
		}
		else
		{
			printf(" < %d,%d> --> ", col, row);
		}
		list_to_print = list_to_print->next_coordinate;
	}
	printf("\n");
}

void print_single_path(path_t* path){
	coordinate_t* iterator;
	iterator = path->head_position;
	char col, last_coo_col;
	int row, last_coo_row;

	last_coo_col = num_to_alpha(path->last_coordinate[1]);
	last_coo_row = (path->last_coordinate[0]) + 1;

	if (iterator != NULL)
	{
		col = num_to_alpha(iterator->col);
		row = (iterator->row) + 1;

		printf("move <%c,%d> to <%c,%d>[", col, row, last_coo_col, last_coo_row);
		//iterator = iterator->next_coordinate;

		while (iterator->next_coordinate != NULL)
		{
			printf("<%c,%d> ", col, row);
			iterator = iterator->next_coordinate;
			col = num_to_alpha(iterator->col);
			row = (iterator->row) + 1;
		}
		printf("<%c,%d>]", last_coo_col, last_coo_row);
	}
	printf("\n");
}

coordinate_t* clone_linkedline(coordinate_t *to_clone){
	coordinate_t* cloned_list;
	cloned_list = NULL;

	while (to_clone != NULL)
	{
		cloned_list = updating_linked_list(to_clone->row, to_clone->col, cloned_list);
		if (cloned_list == NULL)
		{
			printf("Didn't clone eatten_kings_coordinate ");
		}
		to_clone = to_clone->next_coordinate;
	}
	return cloned_list;
}

path_t* clone_path(path_t* original_path)
{
	bool check_cloned_positions;
	path_t* cloned_path;

	cloned_path = (path_t*)malloc(sizeof(path_t));
	if (cloned_path == NULL)
	{
		printf("clone_path function - Failed to allocated memory");
		return NULL;
	}
	cloned_path->head_position = NULL;
	cloned_path->last_coordinate[0] = original_path->last_coordinate[0];
	cloned_path->last_coordinate[1] = original_path->last_coordinate[1];

	cloned_path->path_weight = original_path->path_weight;
	cloned_path->head_position = clone_linkedline(original_path->head_position);

	return cloned_path;
}

void initialize_step(step_t* step){
	step->is_first_step = 1;
	step->is_potntial_step = 0;
}

void free_linked_list(coordinate_t *linkedlist)
{
	//coordinate_t *iterator;

	//iterator = linkedlist;
	while (linkedlist->next_coordinate != NULL)
	{
		if (linkedlist->previous_coordinate != NULL)
		{
			free(linkedlist->previous_coordinate);
		}
		linkedlist = linkedlist->next_coordinate;
	}
	free(linkedlist);
	linkedlist = NULL;
}

void free_path(path_t* path)
{
	if (path->head_position != NULL)
	{
		free_linked_list(path->head_position);
	}
	free(path);
	path = NULL;
}

void free_paths_arr(bool needToDeleteArr)
{
	int i;

	if (paths_number > 0)
	{
		for (i = 0; i < paths_number; i++)
		{
			free_path(paths_arr[i]);
			paths_arr[i] = NULL;
		}
	}
	paths_number = 0;
	if (needToDeleteArr && (paths_arr != NULL))
	{
	//	free(paths_arr);
	}
}

int* adjacent_slot_is_enemy(int row, int col){

	int four_diraction[4] = { FALSE };   //up-right , down-right , down-left , up-left -ClockWise
	color_e enemy_color;

	if (turn == USER)
	{
		enemy_color = computer.color;
	}
	else
	{
		enemy_color = user.color;
	}
	//Not good need to check one more slot
	if (is_enemy_position(row + 1, col + 1))
	{
		four_diraction[0] = TRUE;
	}
	if (is_enemy_position(row - 1, col + 1))
	{
		four_diraction[1] = TRUE;
	}
	if (is_enemy_position(row - 1, col - 1))
	{
		four_diraction[2] = TRUE;
	}
	if (is_enemy_position(row + 1, col - 1))
	{
		four_diraction[3] = TRUE;
	}

	return four_diraction;
}

