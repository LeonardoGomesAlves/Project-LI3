#ifndef INTERATIVO_H_
#define INTERATIVO_H_
#include <stdio.h>
#include <ncurses.h>
#include "parser_interativo.h"

int menu_inicial ();

char* menu_inserirpath();

int menu_principal();

void loading(WINDOW* janela, char* output);

int exemplos_queries();

void print_exemplos (int type);

void menu_queries(void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void* todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats);

#endif