#ifndef UTILIZADORES_H
#define UTILIZADORES_H
#include "voos.h"

void precoGastoReservas (void *user_dados, char* name, double preco);

void organizaReservasUser (void* utilizadores, void* reservas, char* tag);

void organizaVoosUser (void* utilizadores, void* flights, char* tag);

void organiza_VoosEReservasUser (void* utilizadores, void* flights, void* reservas, char* tag);

int accountStatValido (char* asv);

void free_users_data(void* data);

int isInactive(char* str);

int isVoo (char* data);

void inicializarLocale();

int compararUsers(const void* a, const void* b);

int comecaComPrefix(const char* str, const char* prefix);

void ordenaNomes (void* allUsers);

#endif