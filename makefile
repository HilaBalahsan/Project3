all: Draughts 

clean:
	-rm *.o Draughts

Draughts: Draughts.o game_state.o globals.o help_functions.o minimax.o questioning.o setting.o
	gcc  -o Draughts Draughts.o game_state.o globals.o help_functions.o minimax.o questioning.o setting.o -lm -std=c99 -pedantic-errors -g

Draughts.o: Draughts.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Draughts.c

game_state.o: game_state.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm game_state.c

globals.o: globals.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm globals.c

help_functions.o: help_functions.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm help_functions.c

minimax.o: minimax.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm minimax.c

questioning.o: questioning.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm questioning.c

setting.o: setting.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm setting.c