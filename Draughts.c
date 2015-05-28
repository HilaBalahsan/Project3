#include "Draughts.h"

int main()
{
	
	char board[BOARD_SIZE][BOARD_SIZE];
	init_board(board);
	print_board(board);
	print_message(WRONG_MINIMAX_DEPTH);
	perror_message("TEST");
	return 0;
}

char* readline(void) {

	int currentlen = 0;
	char* line = (char*)malloc(10), *linep = line;
	size_t maxlen = 10, len = maxlen;
	int ch;
	int whiteFlag = 0;


	if (line == NULL)
	{
		return NULL;
	}

	while (EOF != (ch = fgetc(stdin)) && (ch != '\n'))
	{
		if (--len == 0)
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

		*line++ = ch;
		currentlen++;
	}
	*line = '\0';

	linep = (char*)realloc(linep, sizeof(char)* (currentlen + 1));

	return linep;
}


int parsing(char* input){

	// Varibles

	int i, depth, row, col;
	const char * whitespace = " ";
	char* userinput[5] = { 0 }, *inputCopy, *token;
	color_e color;
	type_e type;

	// Initialize

	depth = 1;
	Minimax_Depth = 1;
	color = WHITE;

	inputCopy = (char*)malloc(sizeof(char)*(strlen(input) + 1));
	if (inputCopy == NULL)
	{
		printf("Error: standard function malloc failed, exiting.");
		return -1;
	}

	strncpy(inputCopy, input, strlen(input));
	inputCopy[strlen(input)] = '\0';

	token = strtok(inputCopy, whitespace);
	i = 0;
	while ((token != NULL)) {
		if (i < 5) {
			userinput[i] = token;
		}
		token = strtok(NULL, whitespace);
		i++;
	}

	/*input check*/
	if (i == 0)
	{
		free(input);
		free(inputCopy);
		return 1;
	}

	if (userinput[0] == "minmax_depth"){
		depth = alpha_to_num(userinput[1]);
	//	Minimax_Depth = set_minimax_depth(depth); // calculate deoth
	}

	else if (userinput[0] == "user_color"){
		color = (color_e)userinput[1];
		//set_user_color(color); // calculate depth
	}

	else if (userinput[0] == "clear"){
		//clear();
	}

	else if (userinput[0] == "set"){
		color = (color_e)userinput[2];
		type = (type_e) userinput[3];
		row = userinput[1][1];
		col = userinput[1][3];
		//set_disc(color, type, row, col); // add row, col
	}

	else if (userinput[0] == "quit"){
		//quit();
	}

	else if (userinput[0] == "rm"){
		row = userinput[1][1];
		col = userinput[1][3];
		//remove_disc(row, col);
	}

	else if (userinput[0] == "get_moves"){
		//get_moves();
	}

	else if (userinput[0] == "start"){
		//start();
	}

	return 0;

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