#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


int update_paths_array(path_t* new_path)
{
	if (new_path->path_weight == maximal_path_weight)
	{
		paths_arr[paths_number] = new_path;
		paths_number++;
	}
	else if (new_path->path_weight > maximal_path_weight)
	{
		free_paths_arr();
		paths_arr[paths_number] = new_path;
		paths_number++;
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

	for (i = 0; i < paths_number; i++)
	{
		print_single_path(paths_arr[i]);
	}
}

void print_single_path(path_t* path){
	coordinate_t *iterator;
	iterator = path->head_position;
	printf("move <%d,%d> to <%d,%d>[", path->head_position->row, path->head_position->col,
		path->last_coordinate[0], path->last_coordinate[1]);
	while (iterator->next_coordinate != NULL)
	{
		iterator = iterator->next_coordinate;
		printf("<%d,%d> ", iterator->row, iterator->col);
	}
	printf("\n");
}

void clone_linkedline(coordinate_t *iterator, coordinate_t *clone){
	int check_clone;

	while (iterator != NULL)
	{
		clone = (coordinate_t*)malloc(sizeof(coordinate_t));
		if (clone == NULL)
		{
			printf("Error: fatal error during memory allocation, exiting.\n");
			return -1;
		}

		check_clone = updating_linked_list(iterator->row, iterator->col, clone);
		if (check_clone == -1)
		{
			printf("Didn't clone eatten_kings_coordinate ");
		}
		iterator = iterator->next_coordinate;
	}
}

int clone_path(path_t* original_path)
{
	bool check_cloned_positions;
	path_t* cloned_path;
	coordinate_t  *positions_iterate, *cloned_positions;

	cloned_path = (path_t*)malloc(sizeof(path_t));
	if (cloned_path == NULL)
	{
		printf("clone_path function - Failed to allocated memory");
		return -1;
	}
	cloned_path = NULL;
	cloned_positions = NULL;

	cloned_path->path_weight = original_path->path_weight;
	positions_iterate = original_path->head_position;
	clone_linkedline(positions_iterate, cloned_positions);

	return 1;
}

void initialize_step(step_t step){
	step.is_first_step = 1;
	step.is_potntial_step = 0;
}

void free_linked_list(coordinate_t *linkedlist)
{
	coordinate_t *iterator;

	iterator = linkedlist;
	while (iterator->next_coordinate != NULL)
	{
		if (linkedlist->previous_coordinate != NULL)
		{
			free(linkedlist->previous_coordinate);
		}
		iterator = iterator->next_coordinate;
	}
	free(iterator);
}

void free_path(path_t* path)
{
	if (path->head_position != NULL)
	{
		free_linked_list(path->head_position);
	}
	free(path);
}

void free_paths_arr()
{
	int i;

	if (paths_number > 0)
	{
		for (i = 0; i < paths_number; i++)
		{
			free_path(paths_arr[i]);
		}
	}
	paths_number = 0;
	free(paths_arr);
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
