#include "Draughts.h"

// Defauld values
Minimax_Depth = 1;

//Initialization
board_t game_board[BOARD_SIZE][BOARD_SIZE] = { NULL };
player_a = 1;
player_b = 0;

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

	print_board(game_board);
}


int remove_disc(int row, int col){

	if (row > 10 || row < 0 || col < 0 || col > 10)
	{
		printf("Invalid position on the board\n");
		return -1;
	}

	game_board[row][col] = EMPTY;

	return 1;
}


int clear(){
	
	int i, j;

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			game_board[i][j] = EMPTY;
		}
	}

	return 1;

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
	int i, depth, row, col;

	char* userinput[4] = { 0 }, *inputCopy, *token, *color, *type;

	char* userinput[4] = { 0 }, *inputCopy, *token;
	color_e color;
	type_e type;

	char* userinput[4] = { 0 }, *inputCopy, *token, *color;


	// Initialize
	depth = 1;

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


	if (strstr(userinput[0], "minmax_depth") != NULL){

	if (strstr (userinput[0],"minmax_depth") != NULL){

		depth = (int)userinput[1];

	if (userinput[0] == "minmax_depth"){
		depth = alpha_to_num(userinput[1]);
		//	Minimax_Depth = set_minimax_depth(depth); // calculate depth
	}

	else if (userinput[0] == "user_color"){
		color = (color_e)userinput[1];
		//set_user_color(color); // calculate depth
	}

	else if (strstr(userinput[0], "clear") != NULL){
		clear();
	}

	else if (strstr(userinput[0], "set") != NULL){

		row = alpha_to_num((int)userinput[1][1]);
		col = (int)userinput[1][3];

		if (strstr(userinput[2],"black")){
			if (strstr(userinput[3], "M")){
				set_disc(BLACK_M, row, col);
			}
			else{
				set_disc(BLACK_K, row, col);
			}
		}

		else{
			if (strstr(userinput[3], "m")){
				set_disc(WHITE_M, row, col);
			}
			else{
				set_disc(WHITE_K, row, col);
			}
		}

	}

	else if (strstr(userinput[0], "rm") != NULL)
	{
		row = alpha_to_num((int)userinput[1][1]);
		col = (int)userinput[1][3];
	}

	else if (userinput[0] == "clear"){
		//clear();
	}

	else if (userinput[0] == "set"){
		color = (color_e)userinput[2];
		type = (type_e)userinput[3];
		row = userinput[1][1];
		col = userinput[1][3];
		//set_disc(color, type, row, col); // add row, col
	}

	else if (userinput[0] == "rm"){
		row = userinput[1][1];
		col = userinput[1][3];

		//remove_disc(row, col);

		remove_disc(row, col);
	}

	else if (userinput[0] == "get_moves"){
		//get_moves();
	}

	else if (userinput[0] == "start"){
		//start();
	}


	else if (strstr(userinput[0], "print") != NULL){
		print_board(game_board);
	}

	else if (strstr(userinput[0], "move") != NULL){
		//start();
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

void set_minimax_depth(int depth){
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

void set_disc(char char_on_board, int col, int row)
{
	game_board[(row)][col] = char_on_board;
}

