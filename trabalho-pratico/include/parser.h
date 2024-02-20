#ifndef PARSER_H
#define PARSER_H

void* parser(FILE *type, void* (verifica)(char**, void*, FILE*, void*), void* (organiza)(void**), void* userDados, char* path, char* init, void* stats);

void parser_passageiros(FILE *type, void verifica(void*, void*, char**, FILE* resultado, void*), void* users, void* voos, void* passStats);

void parse_query(FILE* input, void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void* todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats);

#endif
