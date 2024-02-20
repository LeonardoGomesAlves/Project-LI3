#ifndef PRINTS_H
#define PRINTS_H

void print_q1(int is_Interativo, char *id, int identificador, int encontrou, void* info, double custo, int idade, int i, int type);

void print_q2(int is_Interativo, int encontrou, void* user, void* allReservations, void* allFlights, int N ,int i, char* tag, int identificador, int type);

void print_q3(int is_Interativo, int encontrou, char* tag, double rating, int i, int type);

void print_q4(int is_Interativo, void* todasReservas, int encontrou, void* hotel, int tamanho, char* tag, int i, int type);

void print_q5(int is_Interarivo, int encontrou, void* aeroporto, char* tag, int total_voos, char* begin, char* end, int num, int type);

void print_q6(int is_Interativo, void* todosAeroportos, int tamanho, int ano, int top, int num, int type);

void print_q7(int is_Interativo, void* todosAeroportos, int tamanho, int top, int num, int type);

void print_q8(int is_Interarivo, int encontrou, char* hotel, int revenue, char* begin_date1, char* end_date1, int i, int type);

void print_q9(int is_Interativo, char* prefix, void* allUsers, int tamanho, int i, int type);

void print_q10(int is_Interativo, void* U_stats, void* R_stats, void* P_stats, void* V_stats, char* ano_c, char* mes_c, int i, int type);

#endif