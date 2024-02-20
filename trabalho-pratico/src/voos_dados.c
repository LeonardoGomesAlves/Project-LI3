#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/datas.h"
#include "../include/utilizadores.h"
#include "../include/utilidades.h"

typedef struct voo {
    char* flight_id;
    char* airline;
    char* plane_model;
    int total_seats;
    char origin[5];
    char destination[5];
    char* schedule_departure_date;
    char* schedule_ano;
    char schedule_arrival_date[25];
    char real_departure_date[25];
    char real_arrival_date[25];
    int passageiros;
    int delay;
} Voo;


typedef struct save_voos { 
    GHashTable* voos_hashtable;
    void** voos_lista;
    int tamanho;
} Save_voos;


typedef struct aeroportos {
    char* name;
    int p2023;
    int p2022;
    int p2021;
    int total_voos;
    int* delays;
    int mediana_delays;
    Voo** arrayVoos;
} Aeroportos;

typedef struct save_aeroportos { 
    Aeroportos** arrayAeroportos;
    int tamanho;
} Save_Aeroportos;

// verifica se uma linha de um dado voo é valida
void *verificaVoo(char** info, void* null, FILE* resultado, void* stats)
{
    if(isEmpty(info[0]) == 1 || isEmpty(info[1]) == 1 || isEmpty(info[2]) == 1 || isDigitString(info[3]) == 0 || 
       isLocationValid(info[4]) == 0 || isLocationValid(info[5]) == 0 || verifica_DuasDatas(info[6], info[7], 1) == 0 || 
       verifica_DuasDatas(info[8], info[9], 1) == 0 || isEmpty(info[10]) == 1 || isEmpty(info[11]) == 1) 
       {
        // da print aos erros no ficheiro
        int t = 13;
        for(int k = 0; k < t; k++)
        {
            if(info[k] == NULL) info[k] = strdup("");
        }

        fprintf(resultado, "%s", info[0]);

        for (int k = 1; k < t; k++) {
            fprintf(resultado, ";%s", info[k]);
        }
        fprintf(resultado, "\n");


        return NULL;
    }
    else{
        struct voo *v = malloc(sizeof(struct voo));
        if(!v) return NULL;

        v->flight_id = strdup(info[0]);
        v->airline = strdup(info[1]);
        v->plane_model = strdup(info[2]);
        v->total_seats = atoi(info[3]);
        char* temp = strdup(info[4]);
        for(int k = 0; temp[k]; k++) temp[k] = toupper(temp[k]);
        strcpy(v->origin, temp);
        char* temp2 = strdup(info[5]);
        for(int k = 0; temp2[k]; k++) temp2[k] = toupper(temp2[k]);
        strcpy(v->destination, temp2);
        v->schedule_departure_date = strdup(info[6]);
        strcpy(v->schedule_arrival_date, info[7]);
        strcpy(v->real_departure_date, info[8]);
        strcpy(v->real_arrival_date, info[9]);
        v->passageiros = 0;
        v->delay = calculaDelays(v->schedule_departure_date, v->real_departure_date);
        v->schedule_ano = dataGetAno(strdup(info[6]));
        addStats(v->schedule_departure_date, stats, v->schedule_ano, NULL, NULL);
        
        free(temp);
        free(temp2);
        return v;
    }
}

//da free dos dados dos voos
void free_voos_data(void* data) 
{
    if (data == NULL) {
        return;
    }

    Save_voos* save_voos = (Save_voos*)data;

    for (int i = 0; save_voos->voos_lista[i]; i++) 
    {
        Voo* currentVoo = (Voo*)save_voos->voos_lista[i];
        free(currentVoo->flight_id);
        free(currentVoo->airline);
        free(currentVoo->plane_model);
        free(currentVoo->schedule_departure_date);
        free(currentVoo->schedule_ano);
        free(currentVoo);
    }

    GHashTable* tabela = (GHashTable*)save_voos->voos_hashtable;
    g_hash_table_destroy(tabela);
    free(save_voos->voos_lista);
    free(save_voos);
}

//funcoes de get e set

char* getFlightSchAno (void* voos_dados) {
    Voo* currentVoo = (Voo*)voos_dados;
    return currentVoo->schedule_ano;
}

char* getFlightId_V(void* voos_dados, char* flight) {
    Save_voos* todosVoos = (Save_voos*)voos_dados;
    Voo* currentVoo = g_hash_table_lookup(todosVoos->voos_hashtable, flight);
    return currentVoo->flight_id;
}

char* getFlightId(void* voos_dados) {
    Voo* currentVoo = (Voo*)voos_dados;
    return currentVoo->flight_id;
}

void addFlightPassenger (void* voos_dados, char* flight) {
    Save_voos* todosVoos = (Save_voos*)voos_dados;
    Voo* currentVoo = g_hash_table_lookup(todosVoos->voos_hashtable, flight);
    currentVoo->passageiros++;
}

void* getFlight(void* voos_dados, char* flight) {
    Save_voos* voos = (Save_voos*)voos_dados;
    Voo* currentVoo = g_hash_table_lookup(voos->voos_hashtable, flight);
    return currentVoo;
}

char* getFlightAirline (void* voo) {
    Voo* currentVoo = (Voo*)voo;
    return currentVoo->airline;
}

char* getFlightPlaneModel (void* voo) {
    Voo* currentVoo = (Voo*)voo;
    return currentVoo->plane_model;
}

char* getFlightOrigin (void* voo) {
    Voo* currentVoo = (Voo*)voo;
    return currentVoo->origin;
}

char* getFlightDestination (void* voo) {
    Voo* currentVoo = (Voo*)voo;
    return currentVoo->destination;
}

char* getFlightSchDepa (void* voo) {
    Voo* currentVoo = (Voo*)voo;
    return currentVoo->schedule_departure_date;
}

char* getFlightSchArr (void* voo) {
    Voo* currentVoo = (Voo*)voo;
    return currentVoo->schedule_arrival_date;
}

int getFlightPassengers (void* voo) {
    Voo* currentVoo = (Voo*)voo;
    return currentVoo->passageiros;
}

int getFlightDelay (void* voo) {
    Voo* currentVoo = (Voo*)voo;
    return currentVoo->delay;
}

int getAeroportosTamanho (void* aeroportos_dados) {
    Save_Aeroportos* aeroportos = (Save_Aeroportos*)aeroportos_dados;
    return aeroportos->tamanho;
}

void* getAeroporto (void* aeroportos_dados, int ind) {
    Save_Aeroportos* aeroportos = (Save_Aeroportos*)aeroportos_dados;
    return aeroportos->arrayAeroportos[ind];
}

char* getAeroportoName (void* aeroportos_dados) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->name;
}

int getAeroportoP23 (void* aeroportos_dados) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->p2023;
}

int getAeroportoP22 (void* aeroportos_dados) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->p2022;
}

int getAeroportoP21 (void* aeroportos_dados) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->p2021;
}

int getAeroportoTotal (void* aeroportos_dados) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->total_voos;
}

char* getAeroportoArraySchDep (void* aeroportos_dados, int ind) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->arrayVoos[ind]->schedule_departure_date;
}

char* getAeroportoArrayFlID (void* aeroportos_dados, int ind) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->arrayVoos[ind]->flight_id;
}

char* getAeroportoArrayDest (void* aeroportos_dados, int ind) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->arrayVoos[ind]->destination;
}

char* getAeroportoArrayAirline (void* aeroportos_dados, int ind) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->arrayVoos[ind]->airline;
}

char* getAeroportoArrayModel (void* aeroportos_dados, int ind) {
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;
    return aeroporto->arrayVoos[ind]->plane_model;
}

int getAeroportosMediana_dir (void* aeroportos_dados, int ind) {
    Save_Aeroportos* aeroportos = (Save_Aeroportos*)aeroportos_dados;
    return aeroportos->arrayAeroportos[ind]->mediana_delays;
}


int getIndiceInicio(void* aeroportos_dados, char* end)
{
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;

    for(int i = 0; i < aeroporto->total_voos; i++)
        if(strcmp(getAeroportoArraySchDep(aeroporto, i), end) <= 0) return i;

    return -1;
}

int getIndiceFim(void* aeroportos_dados, char* begin)
{
    Aeroportos* aeroporto = (Aeroportos*)aeroportos_dados;

    for(int i = 0; i < aeroporto->total_voos; i++)
        if(strcmp(getAeroportoArraySchDep(aeroporto, i), begin) <= 0) return i;

    return aeroporto->total_voos;
}

//função que insere as linhas válidas do dataset dos voos na sua estrutura
void *organizaVoo(void** array)
{
    GHashTable* tabela = g_hash_table_new(g_str_hash, g_str_equal);  // cria a tabela

    int i;
    for(i = 0; array[i]; i++){
        Voo* voos = array[i];     // percorre o array de structs
        g_hash_table_insert(tabela, (voos->flight_id), voos);   // insere-os na hashtable
    }

    Save_voos* save_voos = malloc(sizeof(Save_voos));
    save_voos->voos_lista = array;          // guarda os dados em forma de array de structs
    save_voos->voos_hashtable = tabela;     // guarda os dados em forma de hashtable
    save_voos->tamanho = i;
    
    return save_voos;
}

//verifica se um dado voo existe na base de dados
int procuraVoo(void *tabela2, char* voo) 
{
    Save_voos* tabela = (Save_voos*)tabela2;
    Voo* valorEncontrado = g_hash_table_lookup(tabela->voos_hashtable, voo);

    if (valorEncontrado != NULL) {
        return 1;
    } else {
        return 0;

    }
}

//dá free dos dados dos aeroportos
void free_aeroportos_data(void* todosAeroportos) 
{
    Save_Aeroportos* save_aeroportos = (Save_Aeroportos*)todosAeroportos;
    if (save_aeroportos == NULL) {
        return;
    }

    for (int i = 0; i < save_aeroportos->tamanho; i++) 
    {
        Aeroportos* currentAeroporto = (save_aeroportos->arrayAeroportos[i]);
        free(currentAeroporto->name);      
        free(currentAeroporto->delays);
        free(currentAeroporto->arrayVoos);
        free(currentAeroporto);        
    }

    free(save_aeroportos->arrayAeroportos);
    free(save_aeroportos);
}
