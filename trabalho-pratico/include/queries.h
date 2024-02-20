#ifndef QUERIES_H
#define QUERIES_H

void escolhe_querie(int is_Interativo, char* querie, char** flag, int num, void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void *todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats);

void query1(int is_Interativo, char *id, void *allFlights, void *allUsers, void* allReservations, int i, int type);

void query2(int is_Interativo, void* allReservations, void* allUsers, void* allFlights, int i, char* tag, char* info, int type); //info corresponde a ser flights, reservations e se for null é ambos

void query3 (int is_Interativo, void* todosHoteis, char* tag, int i, int type);

void query4(int is_Interativo, void* allReservations, void* todosHoteis, char* tag, int i, int type) ;

void query5(int isInterativo, void* todosAeroportos, char* origin, char* begin_d, char* begin_h, char* end_d, char* end_h, int i, int type);

void query6(int is_Interativo, void* todosAeroportos, char* top, char* info, int i, int type);

void query7(int is_Interativo, void* todosAeroportos, char* top, int i, int type);

void query8(int is_Interativo, void* todosHoteis, char* hotel, char* begin_date1, char* end_date1, int i, int type);

void query9(int is_Interativo, char* prefix, void* allUsers, int i, int type);

void query10 (int is_Interativo, void* U_stats, void* R_stats, void* P_stats, void* V_stats, char* ano_c, char* mes_c, int i, int type);

#endif