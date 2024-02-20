#ifndef UTILIZADORES_DADOS_H
#define UTILIZADORES_DADOS_H

typedef struct save_users Save_users;

typedef struct user User;

void *verificaUser(char** info, void* null, FILE* resultado, void* stats);

int getUserTotalFlights (void *user_dados, char* name);

void setUserFlights (void *user_dados, char* name, void* flight_dados, char* voo);

int getUserTotalReservations (void *user_dados, char* name);

void addUserFlight (void *user_dados, char* name);

void setUserReservation (void *user_dados, char* name, void *reservas_dados, char* reserva);

void addUserReserva (void *user_dados, char* name);

void* getUser (void *user_dados, char* name);

char* getUserName (void *user_dados);

char* getUserSex (void *user_dados);

int getUserSeen (void* user, int ano);

void setUserSeen (void* user, int ano);

char* getUserCountryC (void *user_dados);

char* getUserPassport (void *user_dados);

int getUserNFlights (void *user_dados);

int getUserNReser (void *user_dados);

double getUserGastoReserv (void *user_dados);

char* getUserStatus (void *user_dados);

char* getUserBirthdate (void *user_dados);

int getUserVoosSorted (void* user_dados);

int getUserReservasSorted (void* user_dados);

int getUserVoosReservasSorted (void *user_dados);

char* getUserFlightArr (void* user_dados, int ind);

char* getUserReservArr (void* user_dados, int ind);

char* getUserFnRArr (void *user_dados, int ind);

int getUserTotalFnR(void *user_dados);

void* getUserArray (void *user_dados, int ind);

int getSaveUserTamanho (void *user_dados);

char* getUserId (void *user_dados);

void *organizaUser (void** array);

void *organizaUser (void** array);

int procuraUser(void *tabela, char* user);

void free_users_data(void* data);

#endif