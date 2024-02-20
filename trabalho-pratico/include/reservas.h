#ifndef RESERVAS_H
#define RESERVAS_H

typedef struct reservation Reservation;

typedef struct save_reservations Save_reservations;

typedef struct hoteis Hoteis;

typedef struct save_hoteis Save_Hoteis;

double calculaCustoPerNoite(void* reservas);

void insereReservas_User(void *utilizadores, void *reservas);

void ordenaPorData (void* todosHoteis, char* hotel);

int comparaDataQ (const void* a, const void* b);

int comparaDataDec (const void* a, const void* b);

double calculaMediaHotel (void* todosHoteis, char* tag);

void sortBookingHoteis (void* todasReservas, void* todosHoteis, char* tag);

#endif