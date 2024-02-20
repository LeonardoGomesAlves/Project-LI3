#ifndef VOOS_DADOS_H
#define VOOS_DADOS_H

typedef struct voo Voo;

typedef struct save_voos Save_voos;

typedef struct aeroportos Aeroportos;

typedef struct save_aeroportos Save_Aeroportos;

void *verificaVoo(char** info, void* null, FILE* resultado, void* stats);

void free_voos_data(void* data);

char* getFlightSchAno (void* voos_dados);

char* getFlightId(void* voos_dados);

void addFlightPassenger (void* voos_dados, char* flight);

void* getFlight(void* voos_dados, char* flight);

char* getFlightAirline (void* voo);

char* getFlightPlaneModel (void* voo);

char* getFlightOrigin (void* voo);

char* getFlightDestination (void* voo);

char* getFlightSchDepa (void* voo);

char* getFlightSchArr (void* voo);

int getFlightPassengers (void* voo);

int getFlightDelay (void* voo);

int getAeroportosTamanho (void* aeroportos_dados);

void* getAeroporto (void* aeroportos_dados, int ind);

char* getAeroportoName (void* aeroportos_dados);

int getAeroportoP23 (void* aeroportos_dados);

int getAeroportoP22 (void* aeroportos_dados);

int getAeroportoP21 (void* aeroportos_dados);

int getAeroportoTotal (void* aeroportos_dados);

char* getAeroportoArraySchDep (void* aeroportos_dados, int ind);

char* getAeroportoArrayFlID (void* aeroportos_dados, int ind);

char* getAeroportoArrayDest (void* aeroportos_dados, int ind);

char* getAeroportoArrayAirline (void* aeroportos_dados, int ind);

char* getAeroportoArrayModel (void* aeroportos_dados, int ind);

int getAeroportosMediana_dir (void* aeroportos_dados, int ind);

int getIndiceInicio(void* aeroportos_dados, char* end);

int getIndiceFim(void* aeroportos_dados, char* begin);

char* getFlightId_V(void* voos_dados, char* flight);

void *organizaVoo(void** results);

int procuraVoo(void *tabela, char* voo);

void free_aeroportos_data(void *saveAeroportos);

#endif