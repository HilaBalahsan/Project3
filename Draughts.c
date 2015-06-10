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
node_t* tmp_tree = { NULL };
coordinate_t* best_path = NULL;
coordinate_t* tmp_path = NULL;
bool use_tmp_board = FALSE;
path_t minmax_path = { 0, 0, { 0 }, NULL };
bool debug = TRUE;

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
		set_disc(BLACK_M, 3, 3, BLACK, MAN);
		first_updating_MenKings_coordinate();
		print_coordinate_list(user.men_coordinate);
		print_coordinate_list(computer.men_coordinate);
		print_board();
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
			//minMax();
			print_board();
			//	if (is_a_winnet)    //page 6 in the PDF

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

//void free_tree(tree_t* tree)
//{
//	free(tree->root->path);
//	free_node_list(tree->root);
//}

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

	if (debug)
	{
		if (strstr(userinput[0], "minmax") != NULL)
		{
		col = alpha_to_num((int)userinput[1][1]); // <x>
		tmp_path = (coordinate_t*)malloc(sizeof(coordinate_t));
		if (tmp_path == NULL)
		{
			printf("Error: fatal error during memory allocation, exiting.\n");
			return -1;
		}
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
				if (j <= 3){
					game_board[i][j] = WHITE_M;
				}
				else if (j >= 6)
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
	coordinate_t* head_linkedList;
	int i, j , return_val;
	char slot;

	head_linkedList = NULL;

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
						head_linkedList = creat_linkedList_pointer(MAN, COMPUTER);
					}
					else
					{
						head_linkedList = creat_linkedList_pointer(MAN, USER);
					}
				}
				else if (slot == WHITE_K)
				{
					if (computer.color == WHITE)
					{
						head_linkedList = creat_linkedList_pointer(KING, COMPUTER);
					}
					else
					{
						head_linkedList = creat_linkedList_pointer(KING, USER);
					}
				}
				else if (slot == BLACK_K)
				{
					if (computer.color == BLACK)
					{
						head_linkedList = creat_linkedList_pointer(KING, COMPUTER);
					}
					else
					{
						head_linkedList = creat_linkedList_pointer(KING, USER);
					}
				}
				else
				{
					if (computer.color == BLACK)
					{
						head_linkedList = creat_linkedList_pointer(MAN, COMPUTER);
						computer.men_coordinate = head_linkedList;
					}
					else
					{
						head_linkedList = creat_linkedList_pointer(MAN, USER);
						user.men_coordinate = head_linkedList;
					}
				}
				head_linkedList = updating_linked_list(i, j, head_linkedList);
				if (head_linkedList == NULL)
				{
					return -1;
				}
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

	if (TRUE)
	{
		printf("%d  , %d", user.men_coordinate, head_coordinate);
	}

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
	}
	else
	{
		temp_coordinate = head_coordinate;
		prev_coordinate = head_coordinate;
		while (temp_coordinate != NULL)
		{
			if (temp_coordinate == head_coordinate)
			{
				temp_coordinate = temp_coordinate->next_coordinate;
			}
			else
			{
				temp_coordinate = temp_coordinate->next_coordinate;
				prev_coordinate = prev_coordinate->next_coordinate;
			}
			break;
		}
		temp_coordinate = current_coordinate;
		temp_coordinate->previous_coordinate = prev_coordinate;
	}
	return head_coordinate;
}



int scoring(){

	int i, j;
	//int score_arr[2] = { 0 }; // scoer array - computer, user
	int scoring_computer, scoring_user;
	int white_score, black_score;
	//int* result;
	char char_on_board;

	white_score = 0;
	black_score = 0;

	if (use_tmp_board)
	{
		for (i = 0; i < BOARD_SIZE; i++)
		{
			for (j = 0; j < BOARD_SIZE; j++)
			{
				char_on_board = tmp_board[i][j];

				if (char_on_board == WHITE_M)
					white_score += 1;
				if (char_on_board == WHITE_K)
					white_score += 3;
				if (char_on_board == BLACK_M)
					black_score += 1;
				if (char_on_board == BLACK_K)
					black_score += 3;
			}
		}
	}

	else
	{
		for (i = 0; i < BOARD_SIZE; i++)
		{
			for (j = 0; j < BOARD_SIZE; j++)
			{
				char_on_board = game_board[i][j];

				if (char_on_board == WHITE_M)
					white_score += 1;
				if (char_on_board == WHITE_K)
					white_score += 3;
				if (char_on_board == BLACK_M)
					black_score += 1;
				if (char_on_board == BLACK_K)
					black_score += 3;
			}
		}
	}

	if ((turn == COMPUTER && computer.color == WHITE) || (turn == USER && user.color == WHITE))
		return (white_score - black_score);
	else
		return (black_score - white_score);
}

int path_score(path_t* path_pointer)
{

	path_t path;
	char** tmp_board = copy_board(game_board);
	int score;
	char type;
	int result;
	coordinate_t* node, * tmp;
	bool b;

	result = 0;
	path.head_position = (coordinate_t*)malloc(sizeof(coordinate_t*));
	path.head_position = path_pointer->head_position;
	path.head_position->next_coordinate = path_pointer->head_position->next_coordinate;
	path.path_weight = path_pointer->path_weight;
	path.score_board_after_path = path_pointer->score_board_after_path;
	path.head_position->next_coordinate = path_pointer->head_position->next_coordinate;

	use_tmp_board = TRUE;	
	while (path.head_position != NULL)
	{
		if (is_enemy_position(path.head_position->row, path.head_position->col))
		{
			remove_disc(path.head_position->row, path.head_position->col, turn);
		}
		else
		{
			node = (coordinate_t*)malloc(sizeof(coordinate_t*));
			if (node == NULL)
			{
				printf("path_score function - Failed to allocated memory");
				return -1;
			}
			node = path.head_position->previous_coordinate;
			while (node != NULL){
				while (is_enemy_position(node->row, node->col)){
					type = tmp_board[node->row][node->col];
					if (strstr(type, "M") != NULL || strstr(type, "m") != NULL)
					{
						if (strstr(type, "M") != NULL)
						{
							set_disc(type, path.head_position->row, path.head_position->col, BLACK, MAN);
						}
						else
						{
							set_disc(type, path.head_position->row, path.head_position->col, WHITE, MAN);
						}
					}
					else
					{
						if (strstr(type, "K") != NULL)
						{
							set_disc(type, path.head_position->row, path.head_position->col, BLACK, KING);
						}
						else
						{
							set_disc(type, path.head_position->row, path.head_position->col, WHITE, KING);
						}
					}
					node = node->next_coordinate;
				}
			}
		}
	//	path.head_position = (coordinate_t*)malloc(sizeof(coordinate_t*));
		path.head_position = path.head_position->next_coordinate;
	}

	result = scoring();
	use_tmp_board = FALSE;
	free(score);
	free_linked_list(path.head_position);

	return result; // return user score
}

int build_min_max_tree(int row, int col)
{
	int root_score, i, j;
	coordinate_t* path;
	path_t* new_path;

	if (debug)
	{
		//State = GAME_STATE;
		//paths_number = 1;
		clear();
		set_disc(WHITE_M, 0, 0, BLACK, MAN);
		set_disc(BLACK_M, 1, 1, WHITE, MAN);
		set_user_color(BLACK);
		print_board();
		new_path = (path_t*)malloc(sizeof(path_t*));
		new_path->head_position = (coordinate_t*)malloc(sizeof(coordinate_t*));
		paths_arr = (path_t**)malloc(sizeof(path_t**));
		new_path->head_position->col = 1;
		new_path->head_position->row = 1;
		new_path->path_weight = 32;
		update_paths_array(new_path);
		
	}
	path = (coordinate_t*)malloc(sizeof(coordinate_t*));
	tmp_tree = (node_t*)malloc(sizeof(node_t*));
	tmp_tree->next_node = (node_t*)malloc(sizeof(node_t*));
	tmp_tree->prev_node = (node_t*)malloc(sizeof(node_t*));
	tmp_tree->path = (coordinate_t*)malloc(sizeof(coordinate_t*));
	minmax_tree = (node_t*)malloc(sizeof(node_t*));
	
	if (tmp_tree == NULL || minmax_tree == NULL || path == NULL || tmp_tree->next_node == NULL || tmp_tree->prev_node == NULL || tmp_tree->path == NULL)
	{
		printf("build_min_max_tree function - Failed to allocated memory");
		return -1;
	}

	root_score = scoring();

	// root of the tree
	path->col = col;
	path->row = row;
	path->val = root_score;

	minmax_tree->node_path_score = root_score;
	minmax_tree->path = path;

	tmp_tree->prev_node = minmax_tree;

	// creat the tree
	for (i = 0; i < Minimax_Depth; i++)
	{
		tmp_tree = (node_t*)malloc(sizeof(node_t*));
		tmp_tree->next_node = (node_t*)malloc(sizeof(node_t*));
		tmp_tree->prev_node = (node_t*)malloc(sizeof(node_t*));
		tmp_tree->path = (coordinate_t*)malloc(sizeof(coordinate_t*));

		if (tmp_tree == NULL || tmp_tree->next_node == NULL || tmp_tree->prev_node == NULL || tmp_tree->path == NULL)
		{
			printf("build_min_max_tree function - Failed to allocated memory");
			return -1;
		}

		if (turn == COMPUTER)
		{
			turn = USER;
		}
		else
		{
			turn = COMPUTER;
		}

//		get_moves(turn);

		for (j = 0; j < paths_number; j++)
		{
			tmp_tree->path = paths_arr[i]->head_position;
			tmp_tree->node_path_score = path_score(paths_arr[i]);

			tmp_tree->next_node->prev_node->node_path_score = path_score(paths_arr[i]);
			tmp_tree->next_node->prev_node->path = paths_arr[i]->head_position;

			tmp_tree = tmp_tree->next_node;
		}
	}

	minmax_tree->next_node = tmp_tree;

	return 1;
}

int recursive_minMax(node_t* node, int depth, int a, int b, bool min_or_max)
{
	int build_tree, v, rec;

	// build minmax array - the optimal move is in
	if (depth == 0 || minmax_tree->next_node == NULL)
	{
		return minmax_tree->next_node->node_path_score;
	}

	if (min_or_max)
	{
		v = -1000;
		while (minmax_tree->next_node != NULL)
		{
			rec = (minmax_tree->next_node->next_node, depth - 1, a, b, FALSE);
			if (minmax_tree->next_node->prev_node != minmax_tree){
				best_path = minmax_tree->next_node->next_node->path;
			}
			v = max(v, rec);
			a = max(v, a);
			if (b <= a)
			{
				break;
			}

			minmax_tree->next_node = minmax_tree->next_node->next_node;
		}
		return v;
	}
	else
	{
		v = 1000;
		while (minmax_tree->next_node != NULL)
		{
			rec = (minmax_tree->next_node->next_node, depth - 1, a, b, TRUE);
			if (minmax_tree->next_node->prev_node != minmax_tree){
				best_path = minmax_tree->next_node->next_node->path;
			}
			v = min(v, rec);
			b = min(v, b);
			if (b <= a)
			{
				break;
			}

			minmax_tree->next_node = minmax_tree->next_node->next_node;
		}
		return v;
	}

}

coordinate_t* minMax(coordinate_t* node)
{
	build_min_max_tree(node->row, node->col);
	recursive_minMax(minmax_tree, Minimax_Depth, -100, 100, TRUE);

	free_node_list(tmp_tree);
	free_node_list(minmax_tree);

	return best_path;
}