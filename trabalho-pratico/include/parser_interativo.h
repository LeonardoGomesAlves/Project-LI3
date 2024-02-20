#ifndef PARSER_INTERATIVO_H_
#define PARSER_INTERATIVO_H_
#include <stdio.h>
#include <ncurses.h>

int parse_query_Interativo(char* input, void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void* todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats);

void print_erros(WINDOW* input, int x);

#endif