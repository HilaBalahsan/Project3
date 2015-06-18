all: Draughts 

clean:
	-rm *.o Draughts

Draughts: Draughts.o game_state.o globals.o help_functions.o minimax.o questioning.o setting.o
	gcc  -o Draughts Draughts.o game_state.o globals.o help_functions.o minimax.o questioning.o setting.o -std=c99 -pedantic-errors -Wall -ansi

Draughts.o: Draughts.c
	gcc -std=c99 -pedantic-errors -Wall -ansi Draughts.c

game_state.o: game_state.c
	gcc -std=c99 -pedantic-errors -Wall -ansi game_state.c

globals.o: globals.c
	gcc -std=c99 -pedantic-errors -Wall -ansi globals.c

help_functions.o: help_functions.c
	gcc -std=c99 -pedantic-errors -Wall -ansi help_functions.c

minimax.o: minimax.c
	gcc -std=c99 -pedantic-errors -Wall -ansi minimax.c

questioning.o: questioning.c
	gcc -std=c99 -pedantic-errors -Wall -ansi questioning.c

setting.o: setting.c
	gcc -std=c99 -pedantic-errors -Wall -ansi setting.c