#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(){
	// Varibles
	int pars_succeed;
	char *line;

	// Initialization
	line = NULL;
	pars_succeed = 1;
	init_board(game_board);

	print_message(WELCOME_TO_DRAUGHTS);

	if (State == SETTINGS_STATE)
	{
		print_message(ENTER_SETTINGS);
	}
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
				return -1;
			}
		}
	}
	free_all();

	return 1;
}

int main_loop(){                     //I changed here.
	char* line;
	int retVal;
	while (State == GAME_STATE)
	{
		if (Turn == USER)
		{
			print_message(ENTER_YOUR_MOVE);
			line = readline();
			if ((strcmp(line, "quit")) == 0) // zero for equal.
			{
				free(line);
				break;
			}
			retVal = parsing(line);
			if (retVal == -1)
			{
				return -1;
			}
			else if (retVal == 2)
			{
				continue;
			}
		}
		else // computer's turn
		{
			minimax();
		}
		print_board();
		// check if user/computer is the winner of the game after the move they made
		if (is_a_winner(Turn))
		{
			switch (Turn)
			{

			case USER:
				if (user.color == WHITE)
				{
					print_message(WHITE_WINS);
				}
				else
				{
					print_message(BLACK_WINS);
				}
				break;
			case COMPUTER:
				if (computer.color == WHITE)
				{
					print_message(WHITE_WINS);
				}
				else
				{
					print_message(BLACK_WINS);
				}
				break;
			default:
				break;
			}

			return 1;
		}
		Turn = Turn ^ 1;
	}
	
	return 1;
}

// converting user's input to something we can work with
char* readline(void) {

	// Varibles
	int currentlen, ch;
	char* line, *linep;
	size_t maxlen, len;    //size_t is an unsigned integer type of at least 16 bit 
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

// parsing user input - find out what the user wants to do!
int parsing(char* input){

	// Varibles
	int i, depth, row, col, check_if_10, return_val;
	color_e tool_color;
	type_e tool_type;
	char ch_on_board;
	char* userinput[4] = { 0 }, *inputCopy, *token;

	// Initialize
	depth = 1;
	tool_color = WHITE;

	inputCopy = (char*)malloc(sizeof(char)*(strlen(input) + 1));
	if (inputCopy == NULL)
	{
		perror_message("parsing");
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

	if (strcmp(userinput[0], "minimax_depth") == 0) // minimax depth
	{
		if (userinput[1] == NULL)
		{
			print_message(ILLEGAL_COMMAND);
			free(input);
			free(inputCopy);
			return 2;
		}
		depth = atoi(userinput[1]);
		set_minimax_depth(depth);
	}
	else if (strcmp(userinput[0], "user_color") == 0) // user's color
	{
		if ((strcmp(userinput[1], "black") == 0) || (strcmp(userinput[1], "Black") == 0) || (strcmp(userinput[1], "BLACK") == 0))
		{
			tool_color = BLACK;
		}
		set_user_color(tool_color); 
	}

	else if (strcmp(userinput[0], "clear") == 0) // clear board
	{
		return_val = clear();
		if (return_val == -1)
		{
			print_message(CLEAR_BOARD_FAILD);
		}
	}
	else if (strcmp(userinput[0], "set") == 0) // set disc on the board
	{
		col = alpha_to_num((int)userinput[1][1]); // <x>
		check_if_10 = (int)userinput[1][4] - 48;

		if ((check_if_10 == 0) && (userinput[1][5] == '>'))
		{
			row = 9;
		}
		else if (userinput[1][4] != '>')
		{
			print_message(WRONG_POSITION);
		}
		else
		{
			row = (int)userinput[1][3] - 49; // <y>
		}

		if (strcmp(userinput[2], "black") == 0)
		{
			tool_color = BLACK;
			if (strcmp(userinput[3], "m") == 0)
			{
				tool_type = MAN;
				ch_on_board = BLACK_M;
			}
			else if (strcmp(userinput[3], "k") == 0)
			{
				tool_type = KING;
				ch_on_board = BLACK_K;
			}
			else
			{
				print_message(ILLEGAL_COMMAND);
				free(input);
				free(inputCopy);
				return 2;
			}
		}
		else if (strcmp(userinput[2], "white") == 0)
		{
			tool_color = WHITE;
			if (strcmp(userinput[3], "m") == 0)
			{
				tool_type = MAN;
				ch_on_board = WHITE_M;
			}
			else if (strcmp(userinput[3], "k") == 0)
			{
				tool_type = KING;
				ch_on_board = WHITE_K;
			}
			else
			{
				print_message(ILLEGAL_COMMAND);
				free(input);
				free(inputCopy);
				return 2;
			}
		}
		else
		{
			print_message(ILLEGAL_COMMAND);
			free(input);
			free(inputCopy);
			return 2;
		}

		return_val = set_disc(ch_on_board, row, col, tool_color, tool_type);
		if (return_val == -1)
		{
			return -1;
		}
	}
	else if (strcmp(userinput[0], "rm") == 0) // remove disc from the board
	{
		col = alpha_to_num((int)userinput[1][1]); // <x>

		check_if_10 = (int)userinput[1][4] - 48;
		if ((check_if_10 == 0) && (userinput[1][5] == '>'))
		{
			row = 9;
		}
		else if (userinput[1][4] != '>')
		{
			print_message(WRONG_POSITION);
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
				print_message(REMOVE_DISC_FAILD);
			}
		}
	}
	else if (strcmp(userinput[0], "get_moves") == 0) // get user's moves
	{
		if (State == GAME_STATE)
		{
			return_val = get_moves(Turn);
			print_path_arr();
			free(input);
			free(inputCopy);
			return 2;
		}
	}
	else if (strcmp(userinput[0], "start") == 0) // start game
	{
		if (State == SETTINGS_STATE)
		{
			return_val = start();
		}
		else
		{
			print_message(ILLEGAL_COMMAND);
			free(input);
			free(inputCopy);
			return 2;
		}
	}
	else if (strcmp(userinput[0], "print") == 0) // print board
	{
		print_board();
	}
	else if (strcmp(userinput[0], "move") == 0) // move disc from the board to other spot on the board
	{
		col = alpha_to_num((int)userinput[1][1]); // <x>

		check_if_10 = (int)userinput[1][4] - 48;
		if ((check_if_10 == 0) && (userinput[1][5] == '>'))
		{
			row = 9;
		}
		else if (userinput[1][4] != '>')
		{
			print_message(WRONG_POSITION);
		}
		else
		{
			row = (int)userinput[1][3] - 49; // <y>
		}
		return_val = move(row, col, userinput[3]);
		
		if (return_val == 2)
		{
			free(input);
			free(inputCopy);
			return 2;
		}
	}
	else{
		print_message(ILLEGAL_COMMAND);
		free(input);
		free(inputCopy);
		return 2;
	}

	free(input);
	free(inputCopy);
	if (return_val == -1)
	{
		return -1;
	}
	return 1;
}

// initialize game board
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

// update men and kings list for eack user
void first_updating_MenKings_coordinate(){
	coordinate_t *user_soldier, *user_kings, *comp_soldiers, *copm_kings;
	int i, j;
	char slot;
	user_soldier = NULL;
	user_kings = NULL;
	comp_soldiers = NULL;
	copm_kings = NULL;

	user.num_of_kings = 0;
	user.num_of_men = 0;
	computer.num_of_kings = 0;
	computer.num_of_men = 0;
	
	free_linked_list(&user.kings_coordinate);
	free_linked_list(&user.men_coordinate);
	free_linked_list(&computer.kings_coordinate);
	free_linked_list(&computer.men_coordinate);

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
						comp_soldiers = updating_linked_list(i, j, comp_soldiers);
						computer.num_of_men++;
					}
					else
					{
						user_soldier = updating_linked_list(i, j, user_soldier);
						user.num_of_men++;
					}
				}
				else if (slot == WHITE_K)
				{
					if (computer.color == WHITE)
					{
						copm_kings = updating_linked_list(i, j, copm_kings);
						computer.num_of_kings++;
					}

					else
					{
						user_kings = updating_linked_list(i, j, user_kings);
						user.num_of_kings++;
					}
				}
				else if (slot == BLACK_K)
				{
					if (computer.color == BLACK)
					{
						copm_kings = updating_linked_list(i, j, copm_kings);
						computer.num_of_kings++;
					}
					else
					{
						user_kings = updating_linked_list(i, j, user_kings);
						user.num_of_kings++;
					}
				}
				else
				{
					if (computer.color == BLACK)
					{
						comp_soldiers = updating_linked_list(i, j, comp_soldiers);
						computer.num_of_men++;
					}
					else
					{
						user_soldier = updating_linked_list(i, j, user_soldier);
						user.num_of_men++;
					}
				}
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


// returns the pointer to specific linked-list

coordinate_t* pointer_to_link(int row, int col, coordinate_t* list_to_change){

	coordinate_t* specific_link;

	specific_link = list_to_change;

	while (specific_link != NULL)
	{
		if ((specific_link->col == col) && (specific_link->row == row))
		{
			return specific_link;
		}

		specific_link = specific_link->next_coordinate;
	}

	return specific_link;
}

// create linked list pointer
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

// delete node from linked list
bool delete_link_from_linked_list(coordinate_t* node_to_delete){
	coordinate_t* temp;
	if (node_to_delete->previous_coordinate == NULL)
	{
		if (node_to_delete->next_coordinate == NULL) //Only element in list
		{
			free(node_to_delete);
			return TRUE;
		}
		else //first but not last.
		{
			node_to_delete->col = node_to_delete->next_coordinate->col;
			node_to_delete->row = node_to_delete->next_coordinate->row;
			temp = node_to_delete->next_coordinate;
			node_to_delete->next_coordinate = node_to_delete->next_coordinate->next_coordinate;
			if (node_to_delete->next_coordinate != NULL)
			{
				node_to_delete->next_coordinate->previous_coordinate = node_to_delete;
			}

			free(temp);
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

// add node to linked list
coordinate_t * updating_linked_list(int row, int col, coordinate_t *head_coordinate){

	coordinate_t *current_coordinate, *temp_coordinate;

	//Initializing
	current_coordinate = NULL;
	temp_coordinate = NULL;

	//Buildind the coordinate list
	current_coordinate = (coordinate_t*)malloc(sizeof(coordinate_t));
	if (current_coordinate == NULL)
	{
		perror_message("updating_linked_list");
		return NULL;
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
		while (temp_coordinate->next_coordinate != NULL)
		{
			if (temp_coordinate == head_coordinate)
			{
				temp_coordinate = temp_coordinate->next_coordinate;
			}
			else
			{
				temp_coordinate = temp_coordinate->next_coordinate;
			}
		}
		temp_coordinate->next_coordinate = current_coordinate;
		current_coordinate->previous_coordinate = temp_coordinate;
	}
	return head_coordinate;
}


