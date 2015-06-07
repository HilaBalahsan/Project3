#ifndef DRAUGHTS_
#define DRAUGHTS_
//////////////////
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
#define BIGGER  (">")


//type definition
typedef char** board_t;
typedef unsigned char bool;


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

typedef enum turn {
	USER,
	COMPUTER
} player_e;

typedef enum direction {
	UPRIGHT, UPLEFT,
	DOWNRIGHT, DOWNLEFT
} direction_e;

typedef struct step {
	int is_first_step;
	int is_potntial_step;    //comes after eatting
} step_t;


typedef struct coordinate {
	// current position
	int row;
	int col;
	int val;
	struct coordinate * next_coordinate;
	struct coordinate * previous_coordinate;
} coordinate_t;


typedef struct path {
	int path_weight;
	int score_board_after_path;
	int last_coordinate[2];
	coordinate_t * head_position;
} path_t;


typedef struct player {
	int minimax_depth;
	color_e color;
	coordinate_t* kings_coordinate;
	coordinate_t* men_coordinate;
	int num_of_men;
	int num_of_kings;
} player_t;

typedef struct node
{
	int node_path_score;
	coordinate_t* path;
	struct node* next_node;
	struct node* prev_node;
}node_t;

typedef struct tree{
	node_t* root;
}tree_t;


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
#define alpha_to_num(character) (character - 96)


//Externs
extern int Minimax_Depth;
extern int player_a;
extern int player_b;
extern int scoring_white;
extern int scoring_black;
extern int capacity;
extern int move_number; // rotem
extern int paths_number;
extern char game_board[BOARD_SIZE][BOARD_SIZE];
extern char tmp_board[BOARD_SIZE][BOARD_SIZE];
extern char** move_arr; // rotem
extern player_e turn;
extern path_t* possible_user_paths;
extern path_t** paths_arr;
extern player_t user;
extern player_t computer;
extern coordinate_t* best_path;
extern path_t mimmax_path;
extern state_e State;
extern bool use_tmp_board;

//Infrastructure Functions
void print_board();
void init_board();
char* readline(void);
int parsing(char* line);
int main_loop();
int main();
int updating_linked_list(player_e player, int row, int col);
int delete_node(coordinate_t* tool_to_delete);
coordinate_t * creat_linkedList_pointer(type_e type);
bool check_win(color_e color);
bool is_valid_position(int row, int col);   //not a white square , ranges
bool is_valid_initialization();            //empty, disc of one color , more then 20 discs of the same color
void free_path(path_t path);
void free_linked_list(coordinate_t *linkedlist);



//Settings Functions
int set_minimax_depth(int x);
int clear();
int remove_disc(int row, int col);
int set_disc(char char_on_board, int row, int col, color_e tool_color, type_e tool_type);
int start();
void set_user_color(color_e color);


//Game Functions
int* scoring();                  //Uses the global board
void moves(coordinate_t tool);
coordinate_t* minMax(coordinate_t node, int depth, int a, int b, bool min_or_max);
bool is_safe_slot; // checks if anamy diagonaly lcated around this slot
int get_moves(player_e player);
void get_move_helper(coordinate_t *itereting_node, type_e tool);
int get_men_moves(int curr_row, int curr_col);
int get_men_moves_helper(direction_e dir, int next_row, int next_col, step_t step, coordinate_t *moves_coor);


int* adjacent_slot_is_enemy(int row, int col, type_e player);
bool is_become_king(int row, int col);
void print_path(path_t *path);
void initialize_step(step_t* step);
void print_path_arr();
void print_single_path(path_t* path);
void free_paths_arr();
bool is_empty_position(int row, int col);
void first_updating_MenKings_coordinate();


#endif