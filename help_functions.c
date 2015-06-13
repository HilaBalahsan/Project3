#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


path_t** clone_path_arr()	{

	path_t** cloned_path_arr;
	int i;

	cloned_path_arr = (path_t**)calloc(capacity, sizeof(path_t*));
	for (i = 0; i < paths_number; i++)
	{
		cloned_path_arr[i] = clone_path(paths_arr[i]);
	}
	
	return cloned_path_arr;
}

bool compare_two_paths(path_t* path_from_arr, path_t* user_input_path) {
	bool equal = TRUE;
	coordinate_t *path, *userPath;
	if ((path_from_arr->last_coordinate[0] != user_input_path->last_coordinate[0]) ||
		(path_from_arr->last_coordinate[1] != user_input_path->last_coordinate[1]))
	{
		equal = FALSE;
	}
	else
	{
		path = path_from_arr->head_position;
		userPath = user_input_path->head_position;

		while ((path != NULL) && (userPath != NULL) && equal)
		{
			if ((path->row == userPath->row) &&
				(path->col == userPath->col))
			{
				path = path->next_coordinate;
				userPath = userPath->next_coordinate;
			}
			else
			{
				equal = FALSE;
			}
		}
	}
	return equal;
}

int update_paths_array(path_t* new_path)
{
	int index;

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
		free_path(&new_path);
	}

	if (paths_number == capacity)
	{
		capacity = capacity * 2;

		paths_arr = (path_t**)realloc(paths_arr, sizeof(path_t*) * capacity);
		if (paths_arr == NULL)
		{
			printf("Error: fatal error during memory alocation, exiting.\n");
			return -1;
		}
		for (index = paths_number; index < capacity; index++)
		{
			paths_arr[index] = NULL;
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
	char col;
	int row, colAvg, rowAvg;
	bool isSrc = TRUE,isSec = TRUE;

	while (iterator != NULL)
	{
		col = num_to_alpha(iterator->col);
		row = (iterator->row) + 1;
		if (isSrc)
		{
			isSrc = FALSE;
			printf("<%c,%d> to ", col, row);
		}
		else if (iterator->next_coordinate == NULL)
		{
			printf("<%c,%d>", col, row);
		}
		else
		{
			colAvg = (iterator->previous_coordinate->col + iterator->col) / 2;
			rowAvg = (iterator->previous_coordinate->row + iterator->row) / 2;

			if (!((colAvg == iterator->col) || (rowAvg == iterator->row)
				|| (colAvg == iterator->previous_coordinate->col)
				|| (rowAvg == iterator->previous_coordinate->row)))
			{
				printf("<%c,%d>", col, row);
			}
		}
		iterator = iterator->next_coordinate;
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

void free_linked_list(coordinate_t **linkedlist)
{
	if ((*linkedlist) == NULL)
	{
		return;
	}
	//iterator = linkedlist;
	while ((*linkedlist)->next_coordinate != NULL)
	{
		if ((*linkedlist)->previous_coordinate != NULL)
		{
			free((*linkedlist)->previous_coordinate);
		}
		(*linkedlist) = (*linkedlist)->next_coordinate;
	}
	free((*linkedlist));
	*linkedlist = NULL;
}

void free_path(path_t** path)
{
	if ((*path) == NULL)
	{
		return;
	}

	if ((*path)->head_position != NULL)
	{
		free_linked_list(&((*path)->head_position));
	}
	free((*path));
	*path = NULL;
}

void free_paths_arr(bool needToDeleteArr)
{
	int i;

	if (paths_number > 0)
	{
		for (i = 0; i < paths_number; i++)
		{
			free_path(&paths_arr[i]);
			paths_arr[i] = NULL;
		}
	}
	paths_number = 0;
	if (needToDeleteArr && (paths_arr != NULL))
	{
		free(paths_arr);
	}
}



