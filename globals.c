#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

char game_board[BOARD_SIZE][BOARD_SIZE];
char** move_arr = NULL;// rotem
int Minimax_Depth = 1;
int capacity = 0;
int paths_number = 0;
int maximal_path_weight = 0;
int move_number = 0; // rotem
int moves_capacity = 0;
state_e State = SETTINGS_STATE;
player_e Turn = USER;
path_t** paths_arr = NULL;
player_t user = { WHITE, NULL, NULL, 0, 0 };
player_t computer = { BLACK, NULL, NULL, 0, 0 };
node_t* minmax_tree = { NULL };
node_t* tree_next_level = { NULL };
coordinate_t* best_path = NULL;
coordinate_t* tmp_path = NULL;
bool use_tmp_board = FALSE;
path_t minmax_path = { 0, 0, { 0 }, NULL };
