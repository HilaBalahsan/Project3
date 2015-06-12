#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


char level_1_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char level_2_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char level_3_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char level_4_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char level_5_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char level_6_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char level_7_board[BOARD_SIZE][BOARD_SIZE] = { 0 };

char game_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char tmp_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
char** move_arr = NULL;// rotem
int Minimax_Depth = 1;
int player_a = 1;
int player_b = 0;
int capacity = 0;
int paths_number = 0;
int scoring_white = 0;
int scoring_black = 0;
int maximal_path_weight = 0;
int move_number = 0; // rotem
int moves_capacity = 0;
state_e State = SETTINGS_STATE;
player_e turn = USER;
path_t** paths_arr = NULL;
player_t user = { 0, WHITE, NULL, NULL, 0, 0 };
player_t computer = { 0, BLACK, NULL, NULL, 0, 0 };
node_t* minmax_tree = { NULL };
node_t* tree_next_level = { NULL };
coordinate_t* best_path = NULL;
coordinate_t* tmp_path = NULL;
bool use_tmp_board = FALSE;
path_t minmax_path = { 0, 0, { 0 }, NULL };
player_t back_up_user = { 0, WHITE, NULL, NULL, 0, 0 };
player_t back_up_computer = { 0, WHITE, NULL, NULL, 0, 0 };