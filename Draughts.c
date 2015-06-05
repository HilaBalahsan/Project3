#include "Draughts.h"

// Defauld values
Minimax_Depth = 1;
/////////////////
//Initialization
char game_board[BOARD_SIZE][BOARD_SIZE] = { NULL };
player_a = 1;
player_b = 0;
State = SETTINGS_STATE;
computer_t computer = NULL;
user_t user = NULL;

int main()
{
	// Varibles
	int pars_succeed;
	char* line;

	// Initialization
	line = NULL;
	pars_succeed = 1;
	init_board(game_board);

	printf(WELCOME_TO_DRAUGHTS);
	print_board(game_board);
	//printf(game_board[2][1]);
	while (TRUE)
	{
		line = readline();
		if ((strcmp(line, "quit")) == 0) // zero for equal.
		{
			free(line);
			break;
		}
		pars_succeed = parsing(line);
		print_board(game_board);
		if (pars_succeed == -1)
		{
			//
		}
	}

	print_board(game_board);

	return 1;
}


// removes the solider from the board
int remove_disc(int row, int col){

	if (is_valid_position(row,col) == FALSE)
	{
		printf(WRONG_POSITION);
		return -1;
	}

//	printf(game_board[row][col]);
	if (game_board[col - 1][row - 1] != EMPTY){
		game_board[col - 1][row - 1] = EMPTY;
	}
	else{
		printf(WRONG_POSITION);
	}

	return 1;
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

		if (strstr(userinput[2],"black"))
		{
			if (strstr(userinput[3], "M"))
			{
				set_disc(BLACK_M, row, col);
			}
			else
			{
				set_disc(BLACK_K, row, col);
			}
		}

		else
		{
			if (strstr(userinput[3], "m"))
			{
				set_disc(WHITE_M, row, col);
			}
			else
			{
				set_disc(WHITE_K, row, col);
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
	switch (color)
	{
	case BLACK:
		//user.minimax_depth = player_b;
		//user.color = BLACK;
		break;

	default:
		;
		//user.minimax_depth = player_a;
		//user.color = WHITE;
	}
}

int set_disc(char char_on_board, int col, int row)
{
	if (is_valid_position(row, col) == FALSE)
	{
		printf(WRONG_POSITION);
		return -1;
	}

	game_board[(row-1)][col-1] = char_on_board;

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



