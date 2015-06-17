#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

char game_board[BOARD_SIZE][BOARD_SIZE];
int Minimax_Depth = 1;
int capacity = 0;
int paths_number = 0;
float maximal_path_weight = 0;
int moves_capacity = 0;
state_e State = SETTINGS_STATE;
player_e Turn = USER;
path_t** paths_arr = NULL;
player_t user = { WHITE, NULL, NULL, 0, 0 };
player_t computer = { BLACK, NULL, NULL, 0, 0 };
