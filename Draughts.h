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
#define WHITESPACE  (" ")

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
	struct coordinate * next_coordinate;
	struct coordinate * previous_coordinate;
} coordinate_t;

typedef struct path {
	float path_weight;
	coordinate_t * head_position;
} path_t;

typedef struct player {
	color_e color;
	coordinate_t* kings_coordinate;
	coordinate_t* men_coordinate;
	int num_of_men;
	int num_of_kings;
} player_t;

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
#define BLACK_WINS "Black player wins!\n"
#define WHITE_WINS "White player wins!\n"
#define CLEAR_BOARD_FAILD "Failed to clear board .\n"
#define REMOVE_DISC_FAILD "Faild to remove disc . \n"
#define COMPUTER_MOVE "Computer: move "

//Macros
#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name))
#define print_message(message) (printf("%s", message));
#define alpha_to_num(character) (character - 97)
#define num_to_alpha(int) (int + 97);

//Externs
extern int Minimax_Depth;
extern int capacity;
extern int paths_number;
extern char game_board[BOARD_SIZE][BOARD_SIZE];
extern float maximal_path_weight;
extern player_e Turn;
extern path_t** paths_arr;
extern player_t user;
extern player_t computer;
extern state_e State;

//Infrastructure Functions
void init_board();
char* readline(void);
int parsing(char* line);
int main_loop();
int main();
void free_all();

//Settings Functions
void set_minimax_depth(int x);
int clear();
int remove_disc(int row, int col, player_e list_to_change);
int set_disc(char char_on_board, int row, int col, color_e tool_color, type_e tool_type);
int start();
void set_user_color(color_e color);
void print_board();
void print_line();

//Game Functions
int scoring(player_e turn);                  //Uses the global board
int get_moves(player_e player);
int move(int row, int col, char* string);
void minimax();

// Help Game Functions
int get_move_helper(coordinate_t *itereting_node, type_e tool, player_e turn);
int get_men_moves(int curr_row, int curr_col, player_e turn);
int get_man_moves_helper(direction_e dir, int next_row, int next_col, step_t* step, path_t *new_path, player_e turn);
int get_king_moves_helper(direction_e dir, int next_row, int next_col, step_t*step, path_t *new_path, player_e turn);
int get_king_moves(int curr_row, int curr_col, player_e turn);
int rec_minimax(int depth, player_e player);
void moves(coordinate_t tool);
void first_updating_MenKings_coordinate();
void return_player_to_original_satae();
void initialize_step(step_t* step);
void perform_move(coordinate_t* move, player_e turn);

// Print Functions
void print_path_arr();
void print_single_path(path_t* path);
void print_path(path_t *path);

// Boolean Functions
bool is_empty_position(int row, int col);
bool is_a_winner(player_e turn);
bool is_legal_move(path_t* user_input_path);
bool is_enemy_position(int row, int col);
bool is_become_king(int row, int col, player_e turn);
bool is_safe_slot; // checks if anamy diagonaly lcated around this slot
bool check_win(color_e color);
bool is_valid_position(int row, int col);   //not a white square , ranges
bool is_valid_initialization();            //empty, disc of one color , more then 20 discs of the same color
bool is_at_the_edge(int row, int col);
bool compare_two_paths(path_t* path_from_arr, path_t* user_input_path);

// Free Functions
void free_path(path_t **path);
void free_linked_list(coordinate_t **linkedlist);
void free_paths_arr(bool needToDeleteArr);

// create & update linkes_ list/arrays/structs
coordinate_t* creat_linkedList_pointer(type_e type, player_e player);
coordinate_t* clone_linkedline(coordinate_t *to_clone);
coordinate_t* pointer_to_link(int row, int col, coordinate_t* list_to_change);
coordinate_t* updating_linked_list(int row, int col, coordinate_t *head_coordinate);
bool delete_link_from_linked_list(coordinate_t* node_to_delete);
path_t* clone_path(path_t* original_path);
path_t**  clone_path_arr();
int update_moves_arr(char* string);
int update_paths_array(path_t* new_path);
int delete_node(coordinate_t* tool_to_delete);
void print_coordinate_list(coordinate_t* list_to_print);

#endif