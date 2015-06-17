#include "Draughts.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*global varibels in our project*/

char game_board[BOARD_SIZE][BOARD_SIZE]; /*game board*/
int Minimax_Depth = 1; /*minimax depth - initializ to defolt minimax depth*/
int capacity = 0; /*capacity of paths_arr  - to calloc paths_arr*/
int paths_number = 0; /*number of possible pathe that the player can make*/
float maximal_path_weight = 0;
state_e State = SETTINGS_STATE; /*state of the game*/
player_e Turn = USER; /*current turn*/
path_t** paths_arr = NULL; /*possible pathe that the player can make*/
player_t user = { WHITE, NULL, NULL, 0, 0 }; /*USER*/
player_t computer = { BLACK, NULL, NULL, 0, 0 }; /*COMPUTER*/
