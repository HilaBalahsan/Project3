#ifndef DRAUGHTS_
#define DRAUGHTS_

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

//type definition
typedef struct path {
	int path_weight;
	struct path* next_move;
}path_t;


typedef struct move {
	// current position
	int row;
	int col;
	struct path* path_start;
} move_t;

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
#define alpha_to_num(character) (character - 48)


//Externs
extern int Minimax_Depth;
extern state_e State;


//Infrastructure Functions
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
<<<<<<< HEAD
char* readline(void);
int parsing(char* line);
=======
>>>>>>> parent of 3ed05dc... Second Commit
int main_loop();
bool check_win(color_e color);
bool is_valid_position(int row, int col);   //not a white square , ranges
bool is_valid_initialization();            //empty, disc of one color , more then 20 discs of the same color


//Settings Functions
void set_minimax_depth(int x);
void set_user_color(color_e color);
void clear();
void remove_disc(int row, int col);
void set_disc(color_e color, type_e type, int row, int col);


//Game Functions
int scoring();                  //Uses the global board
void best_path();


#endif  
