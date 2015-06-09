#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


char game_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char tmp_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char** move_arr = NULL;// rotem
int Minimax_Depth = 1;
int player_a = 1;
int player_b = 0;
int capacity = 0;
int paths_number = 0;
int scoring_white = 0;
int scoring_black = 0;
int maximal_path_weight = 0;
int move_number = 0; // rotem
int moves_capacity = 0;
state_e State = SETTINGS_STATE;
player_e turn = USER;
path_t** paths_arr = NULL;
player_t user = { 0, WHITE, NULL, NULL, 0, 0 };
player_t computer = { 0, BLACK, NULL, NULL, 0, 0 };
node_t* minmax_tree = { NULL };
node_t* tree_next_level = { NULL };
coordinate_t* best_path = NULL;
coordinate_t* tmp_path = NULL;
bool use_tmp_board = FALSE;
path_t minmax_path = { 0, 0, { 0 }, NULL };

int main(){
	// Varibles
	int pars_succeed;
	String line;

	// Initialization
	line = NULL;
	pars_succeed = 1;
	init_board(game_board);

	printf(WELCOME_TO_DRAUGHTS);
	while (TRUE)
	{
		if (State == GAME_STATE)
		{
			main_loop();
			break;
		}
		else
		{
			line = readline();
			if ((strcmp(line, "quit")) == 0) // zero for equal.
			{
				free(line);
				break;
			}
			pars_succeed = parsing(line);
			if (pars_succeed == -1)
			{
				//
			}
		}
	}
    //FUNCTION THAT FREES ALL	
	print_board(game_board);
	return 1;
}

int main_loop(){                     //I changed here.
	String line;

	if (DEBUG)
	{
		clear();
		set_disc(WHITE_M, 0, 0, WHITE, MAN);
		set_disc(BLACK_M, 1, 1, BLACK, MAN);
		first_updating_MenKings_coordinate();
		print_coordinate_list(user.men_coordinate);
		get_moves(turn);
	}
	first_updating_MenKings_coordinate();
	while (State == GAME_STATE)
	{
		if (turn == USER)
		{
			printf(ENTER_YOUR_MOVE);
			line = readline();
			if ((strcmp(line, "quit")) == 0) // zero for equal.
			{
				free(line);
				break;
			}
			print_board();
			//	if (is_a_winnet)    //page 6 in the PDF

			//else
			turn = COMPUTER;
		}
		else
		{
			get_moves(COMPUTER);
			for (int i = 0; i < paths_number; i++)
			{
//				tmp_path = minMax(paths_arr[i]->head_position);
//				if (tmp_path->val > best_path->val)
//					best_path = tmp_path;
			}
			print_board();

			turn = USER;
		}
	}
}

int update_moves_arr(char* string)
{
	if (move_number == capacity)
	{
		move_arr = (int**)realloc(paths_arr, sizeof(int) * 2);

		if (move_arr == NULL)
		{
			printf("Error: fatal error during memory alocation, exiting.\n");
			return -1;
		}
	}
	move_arr[move_number] = string;
	move_number++;
	return 1;
}

void free_node_list(node_t *linkedlist){
	node_t *iterator;
	iterator = linkedlist;
	while (iterator->next_node != NULL)
	{
		if (linkedlist->prev_node != NULL)
		{
			free(linkedlist->prev_node);
		}
		iterator = iterator->next_node;
	}
	free(iterator);
}

char* readline(void) {

	// Varibles
	int currentlen;
	char* line, *linep;
	size_t maxlen, len;    //size_t is an unsigned integer type of at least 16 bit 
	int ch;
	bool spaceFlag;

	//Initialization
	currentlen = 0;
	spaceFlag = FALSE;
	maxlen = 10;
	len = maxlen;
	line = (char*)malloc(10);
	linep = line;


	if (line == NULL)
	{
		return NULL;
	}

	while (EOF != (ch = fgetc(stdin)) && (ch != '\n'))
	{
		if (--len == 0)   //Condition for resizing.
		{
			len = maxlen;

			char* linen = realloc(linep, maxlen *= 2);

			if (linen == NULL)
			{
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);

			linep = linen;
		}
		if (ch == 32)
		{
			if (!spaceFlag)
			{
				*line++ = ch;
				currentlen++;
			}
			spaceFlag = TRUE;
		}
		else
		{
			spaceFlag = FALSE;
		}

		if (!spaceFlag)
		{
			*line++ = ch;
			currentlen++;
		}
	}
	*line = '\0';

	linep = (char*)realloc(linep, sizeof(char)* (currentlen + 1));

	return linep;
}

int parsing(char* input){

	// Varibles
	int i, depth, row, col, check_if_10 , return_val;
	color_e tool_color;
	type_e tool_type;
	char ch_on_board;
	char* userinput[4] = { 0 }, *inputCopy, *token, *type;

	// Initialize
	depth = 1;
	tool_color = WHITE;

	inputCopy = (char*)malloc(sizeof(char)*(strlen(input) + 1));
	if (inputCopy == NULL)
	{
		printf("Error: standard function malloc failed, exiting .\n");
		return -1;
	}

	strncpy(inputCopy, input, strlen(input));
	inputCopy[strlen(input)] = '\0';

	token = strtok(inputCopy, WHITESPACE);
	i = 0;
	while ((token != NULL)) {
		if (i < 4) {
			userinput[i] = token;
		}
		token = strtok(NULL, WHITESPACE);
		i++;
	}

	/*input check*/
	if (i == 0)
	{
		free(input);
		free(inputCopy);
		return 1;
	}

	if (strstr(userinput[0], "minimax_depth") != NULL)
	{
		depth = atoi(userinput[1]);
		set_minimax_depth(depth);
	}

	else if (strstr(userinput[0], "user_color") != NULL)
	{
		if ((strstr(userinput[1], "black") != NULL) || (strstr(userinput[1], "Black") != NULL) || (strstr(userinput[1], "BLACK") != NULL))
		{
			tool_color = BLACK;
		}

		set_user_color(tool_color); // calculate depth
	}

	else if (strstr(userinput[0], "clear") != NULL)
	{
		return_val = clear();
		if (return_val == -1)
		{
			printf("Failed to clear board .\n");
		}
	}

	else if (strstr(userinput[0], "set") != NULL)
	{
		col = alpha_to_num((int)userinput[1][1]); // <x>
		check_if_10 = (int)userinput[1][4] - 48;
		if (check_if_10 == 0)
		{
			row = 10;
		}
		if (check_if_10 != 0)
		{
			row = (int)userinput[1][3] - 49; // <y>
		}

		if (strstr(userinput[2], "black"))
		{
			tool_color = BLACK;
			if (strstr(userinput[3], "m"))
			{
				tool_type = MAN;
				ch_on_board = BLACK_M;
			}
			else
			{
				tool_type = KING;
				ch_on_board = BLACK_K;
			}
		}
		else
		{
			tool_color = WHITE;
			if (strstr(userinput[3], "m"))
			{
				tool_type = MAN;
				ch_on_board = WHITE_M;
			}
			else
			{
				tool_type = KING;
				ch_on_board = WHITE_K;
			}
		}

		return_val = set_disc(ch_on_board, row, col, tool_color, tool_type);
		if (DEBUG)
		{
			printf("row number : %d  , col number : %d  \n", row, col);
			printf(" %c  \n", game_board[row][col]);
		}

		if (return_val == -1)
		{
			printf("Failed to set tool on board .\n");
		}
	}

	else if (strstr(userinput[0], "rm") != NULL)
	{
		col = alpha_to_num((int)userinput[1][1]); // <x>

		check_if_10 = (int)userinput[1][4] - 48;
		if (check_if_10 == 0)
		{
			row = 10;
		}
		else
		{
			row = (int)userinput[1][3] - 49; // <y>
		}

		if (State == SETTINGS_STATE)
		{
			game_board[row][col] = EMPTY;
		}
		else
		{
			return_val = remove_disc(row, col, USER);
			if (return_val == -1)
			{
				printf("Faild to remove disc . \n");
			}
		}
	}

	else if (strstr(userinput[0], "get_moves") != NULL)
	{
		if (State == GAME_STATE)
		{
			return_val = get_moves(turn);
		}
	}

	else if (strstr(userinput[0], "start") != NULL)
	{
		start();
	}

	else if (strstr(userinput[0], "print") != NULL)
	{
		print_board(game_board);
	}

	else if (strstr(userinput[0], "minmax") != NULL)
	{
		col = alpha_to_num((int)userinput[1][1]); // <x>
		tmp_path = (coordinate_t*)malloc(sizeof(coordinate_t));
		if (tmp_path == NULL)
		{
			printf("Error: fatal error during memory allocation, exiting.\n");
			return -1;
		}
		tmp_path->next_coordinate = NULL;

		check_if_10 = (int)userinput[1][4] - 48;
		if (check_if_10 == 0)
		{
			row = 10;
		}
		if (check_if_10 != 0)
		{
			row = (int)userinput[1][3] - 49; // <y>
		}

		tmp_path->col = col;
		tmp_path->row = row;

		minMax(tmp_path);
	}

	else if (strstr(userinput[0], "move") != NULL)
	{
		col = alpha_to_num((int)userinput[1][1]); // <x>

		check_if_10 = (int)userinput[1][4] - 48;
		if (check_if_10 == 0)
		{
			row = 10;
		}
		if (check_if_10 != 0)
		{
			row = (int)userinput[1][3] - 49; // <y>
		}

		move(row, col, userinput[3]);
	}
	return 1;
}

void init_board(){
	int i, j;

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){

			if ((i + j) % 2 == 0)
			{
				if (i <= 3){
					game_board[i][j] = WHITE_M;
				}
				else if (i >= 6)
				{
					game_board[i][j] = BLACK_M;
				}
				else
				{
					game_board[i][j] = EMPTY;
				}
			}
			else{
				game_board[i][j] = EMPTY;
			}
		}
	}
}

void first_updating_MenKings_coordinate(){
	coordinate_t *list_to_build, *user_soldier, *user_kings, *comp_soldiers, *copm_kings;
	int i, j , return_val;
	char slot;
	list_to_build = NULL;
	user_soldier = NULL;
	user_kings = NULL;
	comp_soldiers = NULL;
	copm_kings = NULL;

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++)
		{
			slot = game_board[i][j];
			if (!is_empty_position(i, j))
			{
				if (slot == WHITE_M)
				{
					if (computer.color == WHITE)
					{
						//list_to_build = comp_soldiers;
						comp_soldiers = updating_linked_list(i, j, comp_soldiers);
					}
					else
					{
						//list_to_build = user_soldier;
						user_soldier = updating_linked_list(i, j, user_soldier);
					}
				}
				else if (slot == WHITE_K)
				{
					if (computer.color == WHITE)
					{
						//list_to_build = copm_kings;
						copm_kings = updating_linked_list(i, j, copm_kings);
					}

					else
					{
						//list_to_build = user_kings;
						user_kings = updating_linked_list(i, j, user_kings);
					}
				}
				else if (slot == BLACK_K)
				{
					if (computer.color == BLACK)
					{
						//list_to_build = copm_kings;
						copm_kings = updating_linked_list(i, j, copm_kings);

					}
					else
					{
						//list_to_build = user_kings;
						user_kings = updating_linked_list(i, j, user_kings);
					}
				}
				else
				{
					if (computer.color == BLACK)
					{
						//list_to_build = comp_soldiers;
						comp_soldiers = updating_linked_list(i, j, comp_soldiers);

					}
					else
					{
						//list_to_build = user_soldier;
						user_soldier = updating_linked_list(i, j, user_soldier);
					}
				}
			//	list_to_build = updating_linked_list(i, j, list_to_build);
			//	if (list_to_build == NULL)
		//		{
		//			return -1;
			//	}
			}
			if (!is_empty_position(i, j))
			{
				user.kings_coordinate = user_kings;
				user.men_coordinate = user_soldier;
				computer.kings_coordinate = copm_kings;
				computer.men_coordinate = comp_soldiers;
			}
		}

	}
}

char** copy_board(){
	char tmp_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
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

coordinate_t* pointer_to_link(int row, int col, coordinate_t* list_to_change){
	coordinate_t* specific_link;
	specific_link = (coordinate_t*)malloc(sizeof(coordinate_t));
	if (specific_link == NULL)
	{
		printf("Error: fatal error during memory allocation, exiting.\n");
		return;
	}
	specific_link->next_coordinate = NULL;
	specific_link->previous_coordinate = NULL;
			
	while (list_to_change != NULL)
	{
		if ((list_to_change->col == col) && (list_to_change->row == row))
		{
			specific_link = list_to_change;
			break;
		}
	}
	if (list_to_change == NULL)
	{
		printf("Error:Tool is not exist.\n");
		return;
	}
	return specific_link;
}

coordinate_t * creat_linkedList_pointer(type_e type, player_e player){

	coordinate_t *head_coordinate;

	if (type == MAN)
	{
		if (player == COMPUTER)
		{
			head_coordinate = computer.men_coordinate;
		}
		else
		{
			head_coordinate = user.men_coordinate;
		}
	}
	else
	{
		if (player == COMPUTER)
		{
			head_coordinate = computer.kings_coordinate;
		}
		else
		{
			head_coordinate = user.kings_coordinate;
		}
	}
	return head_coordinate;
}

bool delete_link_from_linked_list(coordinate_t* node_to_delete){
	if (node_to_delete->previous_coordinate == NULL)
	{
		if (node_to_delete->next_coordinate == NULL)
			//Only element in list
		{
			free(node_to_delete);
			return TRUE;
		}
		else
		{
			node_to_delete->col = node_to_delete->next_coordinate->col;
			node_to_delete->previous_coordinate = node_to_delete->next_coordinate->previous_coordinate;
			node_to_delete->next_coordinate = node_to_delete->next_coordinate->next_coordinate;
			node_to_delete->row = node_to_delete->next_coordinate->row;
			node_to_delete->val = node_to_delete->next_coordinate->val;
			delete_link_from_linked_list(node_to_delete->next_coordinate);
		}
	}
	else if (node_to_delete->next_coordinate == NULL)
	{
		node_to_delete->previous_coordinate->next_coordinate = NULL;
		free(node_to_delete);
	}
	else
	{
		node_to_delete->previous_coordinate->next_coordinate = node_to_delete->next_coordinate;
		node_to_delete->next_coordinate->previous_coordinate = node_to_delete->previous_coordinate;
		free(node_to_delete);
	}
	return FALSE;
}

coordinate_t * updating_linked_list(int row, int col, coordinate_t *head_coordinate){

	coordinate_t *new_coordinate, *current_coordinate, *prev_coordinate, *temp_coordinate;

	//Initializing
	new_coordinate = NULL;
	current_coordinate = NULL;
	prev_coordinate = NULL;
	temp_coordinate = NULL;

	//Buildind the coordinate list
	current_coordinate = (coordinate_t*)malloc(sizeof(coordinate_t));
	if (current_coordinate == NULL)
	{
		printf("Error: fatal error during memory allocation, exiting.\n");
		return -1;
	}

	//initializing node
	current_coordinate->col = col;
	current_coordinate->row = row;
	current_coordinate->next_coordinate = NULL;
	current_coordinate->previous_coordinate = NULL;


	if (head_coordinate == NULL)
	{
		head_coordinate = current_coordinate;
		//temp_coordinate = current_coordinate;
	}
	else
	{
		temp_coordinate = head_coordinate;
		//prev_coordinate = head_coordinate;
		while (temp_coordinate->next_coordinate != NULL)
		{
			if (temp_coordinate == head_coordinate)
			{
				temp_coordinate = temp_coordinate->next_coordinate;
			}
			else
			{
				temp_coordinate = temp_coordinate->next_coordinate;
				//prev_coordinate = prev_coordinate->next_coordinate;
			}
		}
		temp_coordinate->next_coordinate = current_coordinate;
		current_coordinate->previous_coordinate = temp_coordinate;
		
		//head_coordinate->next_coordinate = temp_coordinate;
	}
	return head_coordinate;
}



int* scoring(){

	int score_arr[2] = { 0 }; // scoer array - computer, user
	int scoring_computer, scoring_user;
	int* result;

	result = (int*)malloc(sizeof(int));
	if (result == NULL)
	{
		printf("scoring() function - Failed to allocated memory");
		return -1;
	}

	scoring_computer = 20;
	scoring_user = 20;

	scoring_computer = ((computer.num_of_kings) * 3) + (computer.num_of_men);
	scoring_user = ((user.num_of_kings) * 3) + (user.num_of_men);

	score_arr[0] = (scoring_computer - scoring_user);
	score_arr[1] = (scoring_user - scoring_computer);

	result = score_arr;

	return result;

}

int path_score(path_t* path_pointer)
{

	path_t path;

	path.head_position = path_pointer->head_position;
	path.path_weight = path_pointer->path_weight;
	path.score_board_after_path = path_pointer->score_board_after_path;

	char** tmp_board = copy_board(game_board);
	int* score = NULL;
	char type;
	int result;

	result = 0;

	while (path.head_position->next_coordinate != NULL)
	{
		// remove
		if (is_enemy_position(path.head_position->next_coordinate->row, path.head_position->next_coordinate->col))
		{
			remove_disc(path.head_position->next_coordinate->row, path.head_position->next_coordinate->col, turn);
		}
		// set

		else
		{
			coordinate_t* node = path.head_position->previous_coordinate;
			node = (coordinate_t*)malloc(sizeof(coordinate_t*));
			if (node == NULL)
			{
				printf("path_score function - Failed to allocated memory");
				return -1;
			}

			node->next_coordinate = NULL;
			node->previous_coordinate = NULL;

			while (is_enemy_position(node->row, node->col)){
			node = node->previous_coordinate;
			}
			type = tmp_board[node->row][node->col];
			if (strstr(type, "M") != NULL || strstr(type, "m") != NULL)
			{
				use_tmp_board = TRUE;
				if (strstr(type, "M") != NULL)
				{
					set_disc(type, path.head_position->next_coordinate->row, path.head_position->next_coordinate->col, BLACK, MAN);
				}
				else
				{
					set_disc(type, path.head_position->next_coordinate->row, path.head_position->next_coordinate->col, WHITE, MAN);
				}


			}
			else
			{
				use_tmp_board = TRUE;
				if (strstr(type, "K") != NULL)
				{
					set_disc(type, path.head_position->next_coordinate->row, path.head_position->next_coordinate->col, BLACK, KING);
				}
				else
				{
					set_disc(type, path.head_position->next_coordinate->row, path.head_position->next_coordinate->col, WHITE, KING);
				}
			}


		}
		use_tmp_board = FALSE;
	}
	if (turn == COMPUTER)
	{
		score = scoring();
		result = score[0];
		return result;
	}

	result = score[1];

	free(score);
	free_linked_list(path.head_position);

	return result; // return user score
}

int build_min_max_tree(int row, int col)
{
	int root_score, i, j, num_path_score;
	node_t* tree_next_level;
	coordinate_t* path;
	int* pointer_score;

	i = 0;
	path = (coordinate_t*)malloc(sizeof(coordinate_t*));
	tree_next_level = (node_t*)malloc(sizeof(node_t*));
	tree_next_level->next_node = (node_t*)malloc(sizeof(node_t*));
	tree_next_level->prev_node = (node_t*)malloc(sizeof(node_t*));
	tree_next_level->path = (coordinate_t*)malloc(sizeof(coordinate_t*));
	minmax_tree = (node_t*)malloc(sizeof(node_t*));
	//minmax_tree = malloc(sizeof(tree_t));
	if (tree_next_level == NULL || minmax_tree == NULL || path == NULL || tree_next_level->next_node == NULL)
	{
		printf("build_min_max_tree function - Failed to allocated memory");
		return -1;
	}

	if (turn == COMPUTER)
	{
		root_score = scoring()[0];
		pointer_score = root_score;
	}
	else
	{
		root_score = scoring()[1];
		pointer_score = root_score;
	}

	path->col = col;
	path->row = row;
	path->val = root_score;

	//minmax_tree = 
	// root of the tree
	//tree_next_level->prev_node = NULL;
	minmax_tree->node_path_score = pointer_score;
	minmax_tree->path = path;
	//tree_next_level = minmax_tree->next_node;
	tree_next_level->prev_node = minmax_tree;
	tree_next_level->prev_node->node_path_score = root_score;
	//tree_next_level->prev_node->path->col = col;
	//tree_next_level->prev_node->path->row = row;

	// creat the tree
	for (i = 0; i < Minimax_Depth; i++)
	{

		if (turn == COMPUTER)
		{
			turn = USER;
		}
		else
		{
			turn = COMPUTER;
		}

		get_moves(turn);
		for (j = 0; j < paths_number; j++)
		{
			tree_next_level->path = paths_arr[i]->head_position;
			tree_next_level->node_path_score = path_score(paths_arr[i]);
			tree_next_level = tree_next_level->next_node;
			tree_next_level->prev_node->node_path_score = path_score(paths_arr[i]);
			tree_next_level->prev_node->path = paths_arr[i]->head_position;
		}
	}
	return 1;
}

int recursive_minMax(node_t* node, int depth, int a, int b, bool min_or_max)
{
	int build_tree, v, rec;

	node_t* tree_next_level;
	tree_next_level = (node_t*)malloc(sizeof(node_t*));
	if (tree_next_level == NULL)
	{
		printf("build_min_max_tree function - Failed to allocated memory");
		return -1;
	}
	tree_next_level = minmax_tree->next_node;

	// build minmax array - the optimal move is in
	if (depth == 0 || tree_next_level->next_node == NULL)
	{
		return tree_next_level->node_path_score;
	}

	if (min_or_max)
	{
		v = -1000;
		while (tree_next_level->next_node != NULL)
		{
			rec = (tree_next_level->next_node, depth - 1, a, b, FALSE);
			if (tree_next_level->prev_node != minmax_tree){
				best_path = tree_next_level->next_node->path;
			}
			v = max(v, rec);
			a = max(v, a);
			if (b <= a)
			{
				break;
			}
		}
		return v;
	}
	else
	{
		v = 1000;
		while (tree_next_level->next_node != NULL)
		{
			rec = (tree_next_level->next_node, depth - 1, a, b, FALSE);
			if (tree_next_level->prev_node != minmax_tree){
				best_path = tree_next_level->next_node->path;
			}
			v = min(v, rec);
			b = min(v, b);
			if (b <= a)
			{
				break;
			}
		}
		return v;
	}

}

coordinate_t* minMax(coordinate_t* node)
{
	build_min_max_tree(node->row, node->col);
	recursive_minMax(minmax_tree, Minimax_Depth, -100, 100, TRUE);

	free_node_list(minmax_tree);

	return best_path;
}