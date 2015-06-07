#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


char game_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
int Minimax_Depth = 1;
int player_a = 1;
int player_b = 0;
int capacity = 0;
int paths_number = 0;
int scoring_white = 0;
int scoring_black = 0;
int move_number = 0; // rotem
char** move_arr = { 0 };// rotem
int moves_capacity = 0;
state_e State = SETTINGS_STATE;
player_e turn = COMPUTER;
path_t** paths_arr = NULL;
player_t user = { 0, BLACK, NULL, NULL, 0, 0 };
player_t computer = { 0, WHITE, NULL, NULL, 0, 0 };
tree_t minmax_tree = { NULL };
char tmp_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
coordinate_t* best_path = NULL;
bool use_tmp_board = FALSE;

int main(){
	// Varibles
	int pars_succeed;
	string line;

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
	string line;
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
			//minMax();
			print_board();
			//	if (is_a_winnet)    //page 6 in the PDF

			turn = USER;
		}
	}
}
int move(int row, int col, char* string)
{
	// Varibles
	int check_if_10, row_new, col_new;
	char tool;
	int i, len;
	color_e tool_color;
	type_e tool_type;
	bool enemy_pos, last_one;
	char* *inputCopy, *token, *type;
	coordinate_t* node_to_delete;

	// Initialize
	len = strlen(string);
	tool_color = WHITE;

	inputCopy = (char*)malloc(sizeof(char)*(strlen(string) + 1));
	if (inputCopy == NULL)
	{
		printf("Error: standard function malloc failed, exiting.");
		return -1;
	}

	strncpy(inputCopy, string, strlen(string));
	inputCopy[strlen(string)] = '\0';

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

	// remove the disc from his current position
	remove_disc(row, col);

	while (i < move_number)
	{
		col_new = alpha_to_num((int)move_arr[i][1]) - 1; // <x>

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
		tool = game_board[row_new][col_new];
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


		enemy_pos = is_enemy_position(row_new, col_new);

		if (enemy_pos)
		{
			remove_disc(row_new, col_new);
			node_to_delete->col = col_new;
			node_to_delete->row = row_new;
			last_one = delete_link_from_linked_list(node_to_delete);
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
			set_disc(tool, col_new, row_new, tool_color, tool_type);
		}
		i++;
	}
	return 1;
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
void free_tree(tree_t tree)
{
	free(tree.root->path);
	free_node_list(tree.root);
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
	int i, depth, row, col, check_if_10;
	color_e tool_color;
	type_e tool_type;
	char* userinput[4] = { 0 }, *inputCopy, *token, *type;

	// Initialize
	depth = 1;
	tool_color = WHITE;

	inputCopy = (char*)malloc(sizeof(char)*(strlen(input) + 1));
	if (inputCopy == NULL)
	{
		printf("Error: standard function malloc failed, exiting.");
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

	if (strstr(userinput[0], "minmax_depth") != NULL)
	{

		if (strstr(userinput[1], "2") != NULL)
		{
			depth = 2;
		}
		else if (strstr(userinput[1], "3") != NULL)
		{
			depth = 3;
		}
		else if (strstr(userinput[1], "4") != NULL)
		{
			depth = 4;
		}
		else if (strstr(userinput[1], "5") != NULL)
		{
			depth = 5;
		}
		else if (strstr(userinput[1], "6") != NULL)
		{
			depth = 6;
		}

		set_minimax_depth(depth); // calculate depth
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
		clear();
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
			row = (int)userinput[1][3] - 48; // <y>
		}

		if (strstr(userinput[2], "black"))
		{
			tool_color = BLACK;
			if (strstr(userinput[3], "m"))
			{
				tool_type = MAN;
				set_disc(BLACK_M, row, col, tool_color, tool_type);
			}
			else
			{
				tool_type = KING;
				set_disc(BLACK_K, row, col, tool_color, tool_type);
			}
		}

		else
		{
			color_e tool_color = WHITE;
			if (strstr(userinput[3], "m"))
			{
				tool_type = MAN;
				set_disc(WHITE_M, row, col, tool_color, tool_type);
			}
			else
			{
				tool_type = KING;
				set_disc(WHITE_K, row, col, tool_color, tool_type);
			}
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
			row = (int)userinput[1][3] - 48; // <y>
		}
		remove_disc(row, col);
	}

	else if (strstr(userinput[0], "get_moves") != NULL)
	{
		//get_moves();
	}

	else if (strstr(userinput[0], "start") != NULL)
	{
		start();
	}

	else if (strstr(userinput[0], "print") != NULL)
	{
		print_board(game_board);
	}

	else if (strstr(userinput[0], "move") != NULL)
	{
		//move...
	}

	return 1;
}

void print_line(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

void print_board()
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", game_board[i][j]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
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
	int i, j;
	char slot;

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
					}
					else
					{
						head_linkedList = creat_linkedList_pointer(MAN, USER);
					}
				}
				updating_listList(i, j, head_linkedList);
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

int remove_disc(int row, int col){                        //Problem        

	if (!is_valid_position(row, col))
	{
		printf(WRONG_POSITION);
		return -1;
	}
	else
	{
		game_board[row][col] = EMPTY;
	}
	return 1;
}

void free_linked_list(coordinate_t *linkedlist){
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

int clear(){

	int i, j;

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = BOARD_SIZE; j >= 0; j--){
			game_board[i][j] = EMPTY;
		}
	}
	free_linked_list(computer.kings_coordinate);
	free_linked_list(user.kings_coordinate);
	free_linked_list(computer.men_coordinate);
	free_linked_list(user.men_coordinate);
	return 1;
}

int set_minimax_depth(int depth){
	if ((depth > 6) || (depth < 1))
	{
		printf(WRONG_MINIMAX_DEPTH);
	}
	else
	{
		Minimax_Depth = depth;
		player_a = (depth / 2) + 1;
		player_b = (depth / 2);
	}
}

void set_user_color(color_e color)
{
	if (color == WHITE)
	{
		user.minimax_depth = player_a;
		user.color = color;
		computer.color = BLACK;
		computer.minimax_depth = player_b;
		turn = USER;
	}
	else
	{
		user.minimax_depth = player_b;
		user.color = color;
		computer.color = WHITE;
		computer.minimax_depth = player_a;
	}
}

int set_disc(char char_on_board, int col, int row, color_e tool_color, type_e tool_type)
{
	coordinate_t *temp_linkedlist;
	int return_val;

	if ((!is_valid_position(row, col)) || (!is_empty_position(row, col)))
	{
		printf(WRONG_POSITION);
		return -1;
	}
	game_board[row][col] = char_on_board;
	if (user.color == tool_color)
	{
		temp_linkedlist = creat_linkedList_pointer(tool_type, USER);
		if (tool_type == KING)
		{
			user.num_of_kings++;
		}
		else
		{
			user.num_of_men++;
		}
	}
	else
	{
		temp_linkedlist = creat_linkedList_pointer(tool_type, COMPUTER);
		if (tool_type == KING)
		{
			computer.num_of_kings++;
		}
		else
		{
			computer.num_of_men++;
		}
	}

	return_val = updating_linked_list(row, col, temp_linkedlist);
	if (return_val == -1)
	{
		return -1;
	}
	return 1;
}

//Craeting pointer to the list we're updating
coordinate_t * creat_linkedList_pointer(type_e type, player_e player){
	// player_e is for the setting stage

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

int updating_linked_list(int row, int col, coordinate_t *head_coordinate){

	// Variables
	path_t* new_coor_list;
	coordinate_t *new_coordinate, *current_coordinate, *prev_coordinate, *temp_coordinate;

	//Initializing
	new_coor_list = NULL;
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
	//free_coordinate(current_coordinate)
	return 1;
}

bool is_empty_position(int row, int col){
	bool empty = TRUE;
	if (game_board[row][col] != EMPTY)
	{
		empty = FALSE;
	}
	return empty;
}

bool is_a_winner(){
	bool winner = FALSE;
	coordinate_t* temp_king , *temp_man;

	if (turn == COMPUTER)
	{
		temp_king = creat_linkedList_pointer(KING, COMPUTER);
		temp_man = creat_linkedList_pointer(MAN, COMPUTER);
	}
	else
	{
		temp_king = creat_linkedList_pointer(KING, MAN);
		temp_man = creat_linkedList_pointer(MAN, MAN);
	}
	if ((temp_king == NULL) && (temp_man == NULL))
	{
		winner == TRUE;
	}
	else
	{
		get_moves(turn);
		if (paths_arr[0] == NULL)
		{
			winner == TRUE;
		}
	}
}



bool is_valid_position(int row, int col){

	// define
	bool b;

	// initalize
	b = TRUE;

	//if (((col % 2 == 1) && (row % 2 == 0)) || ((col % 2 == 0) && (row % 2 == 1)))

	if ((row + col) % 2 != 0)      //assamption: indexes starts from ziro
	{
		b = FALSE;
	}

	if ((row > 10) || (row < 0) || (col > 10) || (col < 0))
	{
		b = FALSE;
	}

	return b;
}

// checks if the borad is OK - not empty, 20 discs for each color
bool is_valid_initialization()
{
	// define
	bool b;
	int num_of_white, num_of_black, total_discs, num_of_kings, i, j;

	// initalize
	b = TRUE;
	num_of_white = 0;
	num_of_black = 0;
	total_discs = 0;
	num_of_kings = 0;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (game_board[i][j] == WHITE_M)
			{
				num_of_white++;
				total_discs++;
			}
			else if (game_board[i][j] == BLACK_M)
			{
				num_of_black++;
				total_discs++;
			}

			else if ((game_board[i][j] == BLACK_K) || (game_board[i][j] == WHITE_K))
			{
				num_of_kings++;
			}
		}
	}

	if ((num_of_white != 20) || (num_of_black != 20) || (num_of_kings != 0) || (total_discs != 40))
	{
		b = FALSE;

	}

	return b;
}

bool is_enemy_position(int row, int col)
{
	char tool;
	bool enemy;
	color_e mine;

	enemy = FALSE;
	tool = game_board[row][col];

	if ((is_valid_position(row, col)) && (game_board[row][col] != EMPTY))
	{
		if (turn == USER)
		{
			mine = user.color;
		}
		else
		{
			mine = computer.color;
		}

		if (mine == WHITE)
		{
			if ((tool == BLACK_M) || (tool == BLACK_K))
			{
				enemy = TRUE;
			}
		}
		else
		{
			if ((tool == WHITE_M) || (tool == WHITE_K))
			{
				enemy = TRUE;
			}
		}
	}
	return enemy;
}

bool is_become_king(int row, int col){

	bool king;
	char slot;
	color_e mine;

	king = FALSE;

	if (turn == USER)
	{
		mine = user.color;
	}
	else
	{
		mine = computer.color;
	}

	if (mine == WHITE)
	{
		if (((row == BOARD_SIZE - 1) && (is_empty_position(row, col)) && (is_valid_position(row, col))))
		{
			king = TRUE;
		}
	}
	else
	{
		if (((row == 0) && (is_empty_position(row, col)) && (is_valid_position(row, col))))
		{
			king = TRUE;
		}
	}

	return king;
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
		four_diraction[0] == TRUE;
	}
	if (is_enemy_position(row - 1, col + 1))
	{
		four_diraction[1] == TRUE;
	}
	if (is_enemy_position(row - 1, col - 1))
	{
		four_diraction[2] == TRUE;
	}
	if (is_enemy_position(row + 1, col - 1))
	{
		four_diraction[3] == TRUE;
	}

	return four_diraction;
}

int start()
{
	//bool valid;

	if (is_valid_initialization() == FALSE)
	{
		printf(WROND_BOARD_INITIALIZATION);
		return -1;
	}

	State = GAME_STATE;
	return 1;
}

int update_paths_array(path_t* new_path)
{
	paths_arr[paths_number] = new_path;
	paths_number++;

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

int get_moves(player_e player){

	coordinate_t *iterator;

	//releast the previouse arr.
	free_paths_arr(paths_arr);

	paths_arr = (path_t**)malloc(sizeof(path_t*) * BOARD_SIZE);
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

	//printf(paths_list);
	free_linked_list(iterator);
}

void get_move_helper(coordinate_t *itereting_node, type_e tool){

	while (itereting_node != NULL)
	{
		if (tool == KING)
		{
			get_king_moves(itereting_node->row, itereting_node->col);
		}
		else
		{
			get_men_moves(itereting_node->row, itereting_node->col);
		}

		itereting_node = itereting_node->next_coordinate;
	}
}

void initialize_step(step_t step){
	step.is_first_step = 1;
	step.is_potntial_step = 0;
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
	new_path->last_coordinate[0] = curr_row;
	new_path->last_coordinate[1] = curr_col;

	return_val = updating_linked_list(curr_row, curr_col, new_path->head_position);
	if (return_val == -1)
	{
		return -1;
	}

	return_val = get_man_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, step, new_path);
	initialize_step(step);

	return_val = get_man_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, step, new_path);
	initialize_step(step);

	return_val = get_man_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, step, new_path);
	initialize_step(step);

	return_val = get_man_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, step, new_path);
	initialize_step(step);

	if (return_val == -1)
	{
		return -1;
	}
	return 1;
}

int get_man_moves_helper(direction_e dir, int next_row, int next_col, step_t step, path_t *new_path){

	path_t *right_up_clone, *right_down_clone, *left_up_clone, *left_down_clone;
	int returnval1, returnval2, returnval3;

	//Halting conditions
	if ((!is_valid_position(next_row, next_col) && new_path->head_position != NULL))
	{
		new_path->last_coordinate[0] = next_row;
		new_path->last_coordinate[1] = next_col;
		returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
		if (returnval1 == -1)
		{
			return -1;
		}
		return 1;
	}

	if (is_become_king(next_row, next_col))
	{
		returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
		if (returnval1 == -1)
		{
			return -1;
		}
		return 1;
	}

	if ((step.is_first_step == TRUE) && (step.is_potntial_step == FALSE))
	{
		// arrive here if it the first step.
		step.is_first_step = FALSE;
		if (is_enemy_position(next_row, next_col))
		{
			step.is_potntial_step == TRUE;
			returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
			if (returnval1 == -1)
			{
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_men_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, new_path);
				break;
			case UPLEFT:
				returnval1 = get_men_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, new_path);
				break;
			case DOWNRIGHT:
				returnval1 = get_men_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, new_path);
				break;
			case DOWNLEFT:
				returnval1 = get_men_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, new_path);
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
			returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
			if (returnval1 == -1)
			{
				return -1;
			}

			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;

			returnval1 = update_paths_array(new_path);
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
	else if ((step.is_first_step == FALSE) && (step.is_potntial_step == TRUE))
	{
		//arrive here if the previouse slot is belongs to enemy

		if (is_enemy_position(next_row, next_col))
		{
			// two enemy tools in a row can't go from here...
			free_path(new_path);
			return -1;
		}
		else if (is_valid_posotion(next_row, next_col))
		{
			step.is_potntial_step == FALSE;
			returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
			if (returnval1 == -1)
			{
				return -1;
			}
			// Calculate path weight
			new_path->path_weight += 1;

			returnval1 = update_paths_array(new_path);
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
				returnval1 = get_men_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, left_up_clone);
				returnval2 = get_men_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, right_down_clone);
				returnval3 = get_men_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, left_down_clone);


				break;
			case UPLEFT:
				returnval1 = get_men_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, right_up_clone);
				returnval2 = get_men_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, right_down_clone);
				returnval3 = get_men_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, left_down_clone);


				break;
			case DOWNRIGHT:
				returnval1 = get_men_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, left_up_clone);
				returnval2 = get_men_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, right_up_clone);
				returnval3 = get_men_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, left_down_clone);

				break;
			case DOWNLEFT:
				returnval1 = get_men_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, left_up_clone);
				returnval2 = get_men_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, right_up_clone);
				returnval3 = get_men_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, right_down_clone);
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
		else  //Arrive here when looking for another eat
		{
			if (is_enemy_position(next_row, next_col))
			{
				step.is_potntial_step == TRUE;
				returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
				if (returnval1 == -1)
				{
					return -1;
				}

				switch (dir)
				{
				case UPRIGHT:
					returnval1 = get_men_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, new_path);
					break;
				case UPLEFT:
					returnval1 = get_men_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, new_path);
					break;
				case DOWNRIGHT:
					returnval1 = get_men_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, new_path);
					break;
				case DOWNLEFT:
					returnval1 = get_men_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, new_path);
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
			//user position can't go from here.
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
	step_t step = { 1, 0 };
	int return_val;

	new_path = (path_t*)malloc(sizeof(path_t));
	if (new_path == NULL)
	{
		printf("get_move function - Failed to allocated memory");
		return -1;
	}
	//First coordinate is the starting slot.
	return_val = updating_linked_list(curr_row, curr_col, new_path->head_position);
	if (return_val == -1)
	{
		return -1;
	}

	return_val = get_king_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, step, new_path);
	initialize_step(step);

	return_val = get_king_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, step, new_path);
	initialize_step(step);

	return_val = get_king_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, step, new_path);
	initialize_step(step);

	return_val = get_king_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, step, new_path);
	initialize_step(step);

	if (return_val == -1)
	{
		return -1;
	}
	return 1;
}

int get_king_moves_helper(direction_e dir, int next_row, int next_col, step_t step, path_t *new_path){

	path_t *right_up_clone, *right_down_clone, *left_up_clone, *left_down_clone;
	int returnval1, returnval2, returnval3;

	//Halting conditions
	if ((!is_valid_position(next_row, next_col) && new_path->head_position != NULL))
	{
		new_path->last_coordinate[0] = next_row;
		new_path->last_coordinate[1] = next_col;
		returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
		if (returnval1 == -1)
		{
			return -1;
		}
		return 1;
	}

	if ((step.is_first_step == TRUE) && (step.is_potntial_step == FALSE))
	{
		//arrive here if it first step or the previous slot is free

		step.is_first_step = FALSE;
		if (is_enemy_position(next_row, next_col))
		{
			step.is_potntial_step == TRUE;
			new_path->last_coordinate[0] = next_row;
			new_path->last_coordinate[1] = next_col;
			returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
			if (returnval1 == -1)
			{
				return -1;
			}

			switch (dir)
			{
			case UPRIGHT:
				returnval1 = get_men_moves_helper(UPRIGHT, next_row + 1, next_col + 1, step, new_path);
				break;
			case UPLEFT:
				returnval1 = get_men_moves_helper(UPLEFT, next_row + 1, next_col - 1, step, new_path);
				break;
			case DOWNRIGHT:
				returnval1 = get_men_moves_helper(DOWNRIGHT, next_row - 1, next_col + 1, step, new_path);
				break;
			case DOWNLEFT:
				returnval1 = get_men_moves_helper(DOWNLEFT, next_row - 1, next_col - 1, step, new_path);
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
			returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
			if (returnval1 == -1)
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
	else if ((step.is_first_step == FALSE) && (step.is_potntial_step == TRUE))
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

int* scoring(){

	int score_arr[2] = { 0 }; // scoer array - computer, user
	int scoring_computer, scoring_user;

	scoring_computer = 0;
	scoring_user = 0;

	scoring_computer = ((computer.num_of_kings) * 3) + (computer.num_of_men);
	scoring_user = ((user.num_of_kings) * 3) + (user.num_of_men);

	score_arr[0] = (scoring_computer - scoring_user);
	score_arr[1] = (scoring_user - scoring_computer);

	return score_arr;

}

int path_score(path_t* path_pointer)
{
	path_t path;
	path.head_position = path_pointer->head_position;
	path.path_weight = path_pointer->path_weight;
	path.score_board_after_path = path_pointer->score_board_after_path;

	char** tmp_board = copy_board(game_board);
	int* score;
	char type;

	while (path.head_position->next_coordinate != NULL)
	{
		// remove
		if (is_enemy_position(path.head_position->next_coordinate->row, path.head_position->next_coordinate->col))
		{
			remove_disc(path.head_position->next_coordinate->row, path.head_position->next_coordinate->col, tmp_board);
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
		return score[0];
	}
	return score[1]; // return user score
}

int build_min_max_tree(int row, int col)
{
	int root_score, i, j, num_path_score;
	node_t* tree_next_level;
	tree_next_level = (node_t*)malloc(sizeof(node_t*));
	if (tree_next_level == NULL)
	{
		printf("build_min_max_tree function - Failed to allocated memory");
		return -1;
	}
	if (turn == COMPUTER)
	{
		root_score = scoring()[0];
	}
	else
	{
		root_score = scoring()[1];
	}

	// root of the tree
	minmax_tree.root->node_path_score = root_score;
	tree_next_level = minmax_tree.root->next_node;
	tree_next_level->prev_node->node_path_score = path_score(paths_arr[i]);
	tree_next_level->prev_node->path->col = col;
	tree_next_level->prev_node->path->row = row;

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
	tree_next_level = minmax_tree.root->next_node;

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
			if (tree_next_level->prev_node != minmax_tree.root){
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
			if (tree_next_level->prev_node != minmax_tree.root){
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

coordinate_t* minMax(coordinate_t node, int depth, int a, int b, bool min_or_max)
{
	build_min_max_tree(node.row, node.col);
	recursive_minMax(minmax_tree.root, Minimax_Depth, -100, 100, TRUE);

	return best_path;
}