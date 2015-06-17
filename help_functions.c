#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/**
* clone_path_arr
* input : nothing
* output : path_t** clone_path_arr()
* functionality : create's a copy of paths_arr ( clone's paths_arr) and returns the copy
*/
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

/**
* compare_two_paths
* input : path_t* path_from_arr, path_t* user_input_path
* output : TRUE/FALSE 
* functionality : compers between 2 paths, if equal return TRUE else returns FALSE
*/
bool compare_two_paths(path_t* path_from_arr, path_t* user_input_path) {
	bool equal = TRUE;
	coordinate_t *path, *userPath;

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
	
	return equal;
}

/**
* update_paths_array
* input : ppath_t* new_path
* output : int update_paths_array
* functionality : updates paths_arr
* return : -1 if the was memory allocation problems or the path that is given to update paths_arr is NULL, 1 - if the update was done 
*/
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
			perror_message("realloc");
			return -1;
		}
		for (index = paths_number; index < capacity; index++)
		{
			paths_arr[index] = NULL;
		}
	}
	return 1;
}

/**
* print_path_arr
* input : nothig
* output :nothig
* functionality : prints paths_arr
*/
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

/**
* print_coordinate_list
* input :coordinate_t* list_to_print
* output : nothig
* functionality : prints a given linked list (coordinat_t type)
*/
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

/**
* print_single_path
* input :path_t* path
* output : nothig
* functionality : prints a given linked list (path_t type)
*/
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

/**
* clone_linkedline
* input :coordinate_t *to_clone
* output : coordinate_t* clone_linkedline
* functionality : create's a copy of given coordinate_t linked list and returns the copy
*/
coordinate_t* clone_linkedline(coordinate_t *to_clone){
	coordinate_t* cloned_list;
	cloned_list = NULL;

	while (to_clone != NULL)
	{
		cloned_list = updating_linked_list(to_clone->row, to_clone->col, cloned_list);
		if (cloned_list == NULL)
		{
			perror_message("melloc");
		}
		to_clone = to_clone->next_coordinate;
	}
	return cloned_list;
}

/**
* clone_path
* input :path_t* original_path
* output : path_t* clone_path
* functionality : create's a copy of given path_t linked list and returns the copy
*/
path_t* clone_path(path_t* original_path)
{
	path_t* cloned_path;

	cloned_path = (path_t*)malloc(sizeof(path_t));
	if (cloned_path == NULL)
	{
		perror_message("malloc");
		return NULL;
	}
	cloned_path->head_position = NULL;
	cloned_path->path_weight = original_path->path_weight;
	cloned_path->head_position = clone_linkedline(original_path->head_position);

	return cloned_path;
}

/**
* initialize_step
* input :step_t* step
* output : nothing
* functionality : initailizing a given step_t*
*/
void initialize_step(step_t* step){
	step->is_first_step = 1;
	step->is_potntial_step = 0;
}

/**
* free_linked_list
* input :coordinate_t **linkedlist
* output : nothing
* functionality : free a given coordinat_t** from the memory
*/
void free_linked_list(coordinate_t **linkedlist)
{
	if ((*linkedlist) == NULL)
	{
		return;
	}
	/*iterator = linkedlist;*/
	while ((*linkedlist)->next_coordinate != NULL)
	{
		if ((*linkedlist)->previous_coordinate != NULL)
		{
			free((*linkedlist)->previous_coordinate);
		}
		(*linkedlist) = (*linkedlist)->next_coordinate;
	}
	if ((*linkedlist)->previous_coordinate != NULL)
	{
		free((*linkedlist)->previous_coordinate);
	}
	free((*linkedlist));
	*linkedlist = NULL;
}

/**
* free_path
* input :path_t** path
* output : nothing
* functionality : free a given path_t** from the memory
*/
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

/**
* free_paths_arr
* input :bool needToDeleteArr
* output : nothing
* functionality : free paths_arr of TRUE 
*/
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

/**
* free_all
* input :nothing
* output : nothing
* functionality : free paths_arr, user's and computer's men & kings linked lists
*/
void free_all(){
	free_paths_arr(TRUE);
	free_linked_list(&user.kings_coordinate);
	free_linked_list(&user.men_coordinate);
	free_linked_list(&computer.kings_coordinate);
	free_linked_list(&computer.men_coordinate);
}
