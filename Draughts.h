#ifndef DRAUGHTS_
#define DRAUGHTS_
#include<stdio.h>

//Defines
#define WHITE_M 'm'
#define WHITE_K 'k'
#define BLACK_M 'M'
#define BLACK_K 'K'
#define EMPTY ' '

#define BOARD_NUM  (7)
#define ARR_NUM  (6)

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

//typedef struct tree{
//node_t* root;
//}tree_t;


// Saperate between board line (char*) and a regular string.
typedef char* String;


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
#define num_to_alpha(int) (int + 97);


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
extern int maximal_path_weight;
extern char** move_arr; // rotem
extern player_e turn;
extern path_t** paths_arr;
extern player_t user;
extern player_t computer;
extern coordinate_t* best_path;
extern path_t minmax_path;
extern state_e State;
extern bool use_tmp_board;


extern player_t back_up_user;
extern player_t back_up_computer;
extern char backup_board[BOARD_SIZE][BOARD_SIZE];
extern char level_1_board[BOARD_SIZE][BOARD_SIZE];
extern char level_2_board[BOARD_SIZE][BOARD_SIZE];
extern char level_3_board[BOARD_SIZE][BOARD_SIZE];
extern char level_4_board[BOARD_SIZE][BOARD_SIZE];
extern char level_5_board[BOARD_SIZE][BOARD_SIZE];
extern char level_6_board[BOARD_SIZE][BOARD_SIZE];
extern char level_7_board[BOARD_SIZE][BOARD_SIZE];
#define BOARD_NUM  (7)
#define ARR_NUM  (6)
extern char** boards[BOARD_NUM];


//Infrastructure Functions
void print_board();
void init_board();
void print_line();
char* readline(void);
int parsing(char* line);
int main_loop();
int main();
coordinate_t* updating_linked_list(int row, int col, coordinate_t *head_coordinate);
int delete_node(coordinate_t* tool_to_delete);
bool delete_link_from_linked_list(coordinate_t* node_to_delete);
bool check_win(color_e color);
bool is_valid_position(int row, int col);   //not a white square , ranges
bool is_valid_initialization();            //empty, disc of one color , more then 20 discs of the same color
void free_path(path_t* path);
void free_linked_list(coordinate_t *linkedlist);
bool is_at_the_edge(int row, int col);
bool compare_two_paths(coordinate_t* path_from_arr, coordinate_t* user_input_path);

//Settings Functions
int set_minimax_depth(int x);
int clear();
int remove_disc(int row, int col, player_e list_to_change);
int set_disc(char char_on_board, int row, int col, color_e tool_color, type_e tool_type);
int start();
void set_user_color(color_e color);


//Game Functions
int scoring();                  //Uses the global board
void moves(coordinate_t tool);
coordinate_t* minMax(coordinate_t node, int depth, int a, int b, bool min_or_max);
bool is_safe_slot; // checks if anamy diagonaly lcated around this slot
int get_moves(player_e player);
void get_move_helper(coordinate_t *itereting_node, type_e tool);
int get_men_moves(int curr_row, int curr_col);
int get_man_moves_helper(direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path);
int get_king_moves_helper(direction_e dir, int next_row, int next_col, step_t*step, path_t *new_path);
int get_king_moves(int curr_row, int curr_col);
int check_and_build(int row, int col, int des_row, int des_col, bool first_step);

void original_board();
void path_on_board(path_t* path_pointer);
void back_up_players();
void minimax();
void return_player_to_original_satae();
void copy_board_to_gameboard(char board[BOARD_SIZE][BOARD_SIZE]);
int rec_minimax(char ** board, int depth, bool min_or_max);


int* adjacent_slot_is_enemy(int row, int col, type_e player);
bool is_enemy_position(int row, int col);
bool is_become_king(int row, int col);
void print_path(path_t *path);
void initialize_step(step_t* step);
void print_path_arr();
void print_single_path(path_t* path);
void free_paths_arr(bool needToDeleteArr);
bool is_empty_position(int row, int col);
void first_updating_MenKings_coordinate();
void free_node_list(node_t *linkedlist);
coordinate_t * creat_linkedList_pointer(type_e type, player_e player);
coordinate_t* clone_linkedline(coordinate_t *to_clone);
path_t* clone_path(path_t* original_path);
bool is_a_winner();
coordinate_t* pointer_to_link(int row, int col, coordinate_t* list_to_change);
bool is_legal_move(path_t* user_input_path);
void make_user_path(coordinate_t* move);
void change_turn(player_e turn);



//int recursive_minMax(node_t* node, int depth, int a, int b, bool min_or_max);
coordinate_t* minMax(coordinate_t* node);
//void free_tree(tree_t tree);
int update_moves_arr(char* string);
int move(int row, int col, char* string);
void print_coordinate_list(coordinate_t* list_to_print);



#endif