#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


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

bool is_at_the_edge(int row, int col){
	bool edge;
	edge = FALSE;
	if ((row == 0) && (col == 0))
	{
		edge = TRUE;
	}
	else if ((row == 0) && (col == BOARD_SIZE-1))
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
