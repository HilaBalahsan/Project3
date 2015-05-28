#ifndef DRAUGHTS_
#define DRAUGHTS_
//
#include<stdio.h>

//Defines
#define WHITE_M 'm'
#define WHITE_K 'k'
#define BLACK_M 'M'
#define BLACK_K 'K'
#define EMPTY ' '

#define BOARD_SIZE  (10)
#define TRUE		(1)
#define FALSE		(0)
#define DEBUG		(TRUE)
#define WHITESPACE  (" ")

//type definition

typedef enum color {
	WHITE,
	BLACK
} color_e;

typedef enum type {
	MAN,
	KING
} type_e;

typedef enum state {
	SETTINGS_STATE,
	GAME_STATE
} state_e;

typedef enum player {
	USER,
	COMPUTER
} player_e;

typedef struct user{
	int minimax_depth;
	color_e color;
}user_t;

typedef struct computer{
	int minimax_depth;
	color_e color;
}computer_t;

typedef struct path {
	int path_weight;
	int lenght;
	struct move* head_position;
}path_t;


typedef struct moves {
	// current position
	int row;
	int col;
	struct move* next_move;
} moves_t;

typedef char** board_t;
typedef unsigned char bool;

// Saperate between board line (char*) and a regular string.
typedef char* string;


// Massages
#define WELCOME_TO_DRAUGHTS "Welcome to Draughts!\n"
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 6\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_DICS "The specified position does not contain your piece\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"
#define ENTER_YOUR_MOVE "Enter your move:\n" 

//Macros
#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name))
#define print_message(message) (printf("%s", message));
#define alpha_to_num(character) (character - 97)


//Externs
extern int Minimax_Depth;
extern state_e State;
//extern board_t game_board;
extern char game_board[BOARD_SIZE][BOARD_SIZE];
extern int player_a;
extern int player_b;


//Infrastructure Functions
void print_board(board_t game_board);
void init_board(board_t game_board);
char* readline(void);
int parsing(char* line);
int main_loop();
int main();
bool check_win(color_e color);
bool is_valid_position(int row, int col);   //not a white square , ranges
bool is_valid_initialization();            //empty, disc of one color , more then 20 discs of the same color


//Settings Functions
void set_minimax_depth(int x);
void set_user_color(color_e color);
int clear();
int remove_disc(int row, int col);
void set_disc(char char_on_board, int row, int col);


//Game Functions
int scoring();                  //Uses the global board
void best_path();


#endif