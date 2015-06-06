#include "Draughts.h"
#include <math.h>

// Defauld values
Minimax_Depth = 1;
State = SETTINGS_STATE;
turn = COMPUTER;                // defalt: first player is the computer
char game_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
int Minimax_Depth = 1;
int player_a = 1;
int player_b = 0;
int capacity = 0;
int paths_number = 0;
int scoring_white = 0;
int scoring_black = 0;
state_e State = SETTINGS_STATE;
player_e turn = COMPUTER;
path_t* possible_user_paths = NULL;
path_t** paths_arr = NULL;
user_t user = { 0, BLACK, NULL, NULL, 0, 0 };
computer_t computer = { 0, WHITE, NULL, NULL, 0, 0 };
path_t* minmax_path[6] = { NULL };
tree_t minmax_tree = NULL;
//char tmp_board[BOARD_SIZE][BOARD_SIZE] = { 0 };

char** copy_board(char board[BOARD_SIZE][BOARD_SIZE])
{
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

int path_score(path_t path)
{
	const tmp_board = copy_board(game_board);

	while (path.head_position->next_coordinate != NULL)
	{

	}
	return 1;

}
tree_t build_min_max_tree(int row, int col)
{
	minmax_tree.root.val = scoring(); // the first level

	if (turn == COMPUTER)
	{
		depth = computer.minimax_depth;
	}
	else
	{
		depth = user.minimax_depth;
	}
	

}

int minMax(coordinate_t node, int depth, int a, int b, bool min_or_max)
{
	if (turn == COMPUTER)
	{
		depth = computer.minimax_depth;
	}
	else
	{
		depth = user.minimax_depth;
	}
	if (depth == 0 || node.next_coordinate == NULL)
	{
		return node.val;
	}
	if (min_or_max)
	{
		for (int i = 0; i < paths_number; i++)
		{

		}
	}
}

int main(){
	// Varibles
	int pars_succeed;
	string line;

	// Initialization
	line = NULL;
	pars_succeed = 1;
	init_board(game_board);
	
	printf(WELCOME_TO_DRAUGHTS);

	switch (State)
	{
	case GAME_STATE:
	{
		main_loop();
	}
		break;

	default:
	{
		while (TRUE)
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
		break;
	}
		print_board(game_board);
		return 1;
	}
}

// removes the solider from the board
int remove_disc(int row, int col, char** board){
	if ((!is_valid_position(row, col)) || game_board[col][row] == EMPTY )
	{
		printf(WRONG_POSITION);
		return -1;
	}
	board[col][row] = EMPTY;
	return 1;
}

int update_paths_list(){
	// allocating memory ,crating a node , joining to the linkedlist.
}

int free_linked_list(){
	//Frees the memory
}

int main_loop(){
	string line;
	switch (turn)
	{
		case(USER) :
		{
			line = readline();		
			turn = COMPUTER;
		}
		break;

		default:
		{
			//minMax();
			turn = USER;
		}
		break;
	}
}

// clearing the board
int clear(){
	
	int i, j;

	for (i = 0; i < BOARD_SIZE ; i++){
		for (j = BOARD_SIZE; j >= 0 ; j--){
			game_board[i][j] = EMPTY;
		}
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
	free_linked_list(path->head_position);
	free(path);
}
// reads what the user typed
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

	char* userinput[4] = { 0 }, *inputCopy, *token, *type;
	color_e color;

	// Initialize
	depth = 1;
	color = WHITE;

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

		if (strstr(userinput[1], "2") != NULL )
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
			color = BLACK;
		}

		set_user_color(color); // calculate depth
	}

	else if (strstr(userinput[0], "clear") != NULL)
	{
		clear();
	}

	else if (strstr(userinput[0], "set") != NULL)
	{
		col = alpha_to_num((int)userinput[1][1])-1; // <x>

		check_if_10 = (int)userinput[1][4] - 48;
		if (check_if_10 == 0)
		{
			row = 10-1;
		}
		if (check_if_10 != 0)
		{
			row = (int)userinput[1][3] - 49; // <y>
		}

		if (strstr(userinput[2],"black"))
		{
			if (strstr(userinput[3], "M"))
			{
				set_disc(BLACK_M, row, col, game_board);
			}
			else
			{
				set_disc(BLACK_K, row, col, game_board);
			}
		}

		else
		{
			if (strstr(userinput[3], "m"))
			{
				set_disc(WHITE_M, row, col, game_board);
			}
			else
			{
				set_disc(WHITE_K, row, col, game_board);
			}
		}
	}

	else if (strstr(userinput[0], "rm") != NULL)
	{
		col = alpha_to_num((int)userinput[1][1]) - 1; // <x>

		check_if_10 = (int)userinput[1][4] - 48;
		if (check_if_10 == 0)
		{
			row = 10 - 1;
		}
		if (check_if_10 != 0)
		{
			row = (int)userinput[1][3] - 49; // <y>
		}
		remove_disc(row, col, game_board);
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

void print_board(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", board[i][j]);
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

void init_board(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if ((i + j) % 2 == 0){
				if (j <= 3){
					board[i][j] = WHITE_M;
				}
				else if (j >= 6){
					board[i][j] = BLACK_M;
				}
				else{
					board[i][j] = EMPTY;
				}
			}
			else{
				board[i][j] = EMPTY;
			}
		}
	}
}
////////////////////////////////////////////////////////

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

int set_disc(char char_on_board, int col, int row, char** board)
{
	if (is_valid_position(row, col) == FALSE)
	{
		printf(WRONG_POSITION);
		return -1;
	}
	board[row][col] = char_on_board;
	updating_tool_list(USER, row, col, char_on_board);
	return 1;
}
//Craeting pointer to the list we're updating
coordinate_t * creat_linkedList_pointer(type_e type, player_e player, int row, int col){
	coordinate_t *head_coordinate;


	if (type == MAN)
	{
		if (player == COMPUTER)
		{
			head_coordinate = computer.men_coordinate;
			computer.num_of_men++;
		}
		else
		{
			head_coordinate = user.men_coordinate;
			user.num_of_men++;
		}
	}
	else
	{
		if (player == COMPUTER)
		{
			head_coordinate = computer.kings_coordinate;
			computer.kings_coordinate++;
		}
		else
		{
			head_coordinate = user.kings_coordinate;
			user.num_of_kings++;
		}
	}

	return head_coordinate;
}

int updating_linked_list(int row, int col, coordinate_t *head_coordinate){

	// Variables
	path_t* new_coor_list;
	coordinate_t *new_coordinate, *current_coordinate, *prev_coordinate, *temp_coordinate;
	int row, col;

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

bool is_free_coordinate(int row, int col) {
	bool free;
	free = FALSE;
	if (game_board[row][col] == EMPTY)
	{
		free = TRUE;
	}
	return free;
}

bool is_enemy_position(int row, int col)
{
	char tool;
	bool enemy;

	enemy = FALSE;
	tool = game_board[row][col];
	if (user.color == WHITE)
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
	return enemy;
}

bool is_king(int row, int col){
	bool king = FALSE;
	if ((game_board[row][col] == WHITE_K) || (game_board[row][col] == BLACK_K))
	{
		king = TRUE;
	}
	return king;
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
	int chech_eatten_king, check_eatten_men, check_cloned_positions;
	path_t* cloned_path;
	coordinate_t *eatten_kings_iterate, *eatten_men_iterate, *positions_iterate,
		*cloned_eatten_king, *cloaned_eatten_men, *cloned_positions;

	cloned_path->path_weight = original_path->path_weight;
	positions_iterate = original_path->head_position;

	clone_linkedline(eatten_kings_iterate, cloned_eatten_king);
	clone_linkedline(eatten_men_iterate, cloaned_eatten_men);
	clone_linkedline(positions_iterate, cloned_positions);

}

int get_moves(){

	paths_arr = (path_t**)malloc(sizeof(path_t*) * BOARD_SIZE);
	if (paths_arr == NULL)
	{
		printf("get_move function - Failed to allocated memory");
		return -1;
	}
	capacity = BOARD_SIZE;

	get_move_helper(user.kings_coordinate, KING);
	get_move_helper(user.men_coordinate, MAN);

	//printf(paths_list);
	// free
}

void get_move_helper(coordinate_t *itereting_node, type_e tool){

	while (itereting_node != NULL)
	{
		if (tool == KING)
		{
			get_king_move(itereting_node->row, itereting_node->col);
		}
		else
		{
			get_men_moves(itereting_node->row, itereting_node->col);
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

	return_val = updating_linked_list(curr_row, curr_col, new_path->head_position);
	if (return_val == -1)
	{
		return -1;
	}

	return_val = get_men_moves_helper(DOWNLEFT, curr_row - 1, curr_col - 1, step, new_path);

	return_val = get_men_moves_helper(DOWNRIGHT, curr_row - 1, curr_col + 1, step, new_path);

	return_val = get_men_moves_helper(UPLEFT, curr_row + 1, curr_col - 1, step, new_path);

	return_val = get_men_moves_helper(UPRIGHT, curr_row + 1, curr_col + 1, step, new_path);
}

int get_men_moves_helper(direction_e dir, int next_row,
	int next_col, step_t step, path_t *new_path){

	path_t *right_up_clone, *right_down_clone, *left_up_clone, *left_down_clone;
	int returnval1, returnval2, returnval3;

	//Halting condition

	if (!is_valid_position(next_row, next_col))
	{
		return 1;
	}

	if ((step.is_first_step == TRUE) && (step.is_potntial_step == FALSE))
	{
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
		else if (is_free_coordinate(next_row, next_col))
		{
			returnval1 = updating_linked_list(next_row, next_col, new_path->head_position);
			if (returnval1 == -1)
			{
				return -1;
			}
			returnval1 = update_paths_array(new_path);
			if (returnval1 == -1)
			{
				return -1;
			}
		}
		else
		{
			//user position can't go from here.
			free_path(new_path);
			return 1;
		}
	}
	else if ((step.is_first_step == FALSE) && (step.is_potntial_step == TRUE))
	{
		if (is_enemy_position(next_row, next_col))
		{
			// two enemy tools in a row can't go from here...
			free_path(new_path);
			return -1;
		}
		else if (is_free_coordinate(next_row, next_col))
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
		else
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
// checks if the position is legal ( not empty )
bool is_valid_position(int row, int col){

	// define
	bool b;

	// initalize
	b = TRUE;

	if (((col % 2 == 1) && (row % 2 == 0)) || ((col % 2 == 0) && (row % 2 == 1)))
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

	if ((num_of_white != 20) || (num_of_black != 20) || (num_of_kings != 0) || (total_discs != 40 ))
	{
		b = FALSE;	
	}

	return b;
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

int* scoring()
{
	int* score_arr[2] = { 0 } ; // scoer array - computer, user
	int scoring_computer, scoring_user;

	scoring_computer = 0;
	scoring_user = 0;

	scoring_computer = ((computer.num_of_kings) * 3) + (computer.num_of_men);
	scoring_user = ((user.num_of_kings) * 3) + (user.num_of_men);

	score_arr[0] = (scoring_computer - scoring_user);
	score_arr[1] = (scoring_user - scoring_computer);

	return score_arr;
	
}





