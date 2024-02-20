#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/datas.h"
#include "../include/utilizadores.h"
#include "../include/voos.h"
#include "../include/utilidades.h"
#include "../include/utilizadores_dados.h"
#include "../include/voos_dados.h"

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

// verifica se uma localização é valida 
int isLocationValid(char* loc){
    if(strlen(loc) != 3) return 0;

    for(int i = 0; loc[i] != '\0'; i++){
        if(!isalpha(loc[i])) return 0;
    }

    return 1;
}


//--------------------------------- FUNÇÕES AUXILIARES PARA Q7 ----------------------------

// procura um aeroporto no array 
void* encontrarAeroportoArray(void* save_Aeroportos, char* origin) 
{
    Save_Aeroportos* saveAeroportos = (Save_Aeroportos*)save_Aeroportos;
    for (int i = 0; i < saveAeroportos->tamanho; i++) 
    {
        if (strcmp(saveAeroportos->arrayAeroportos[i]->name, origin) == 0) 
        {
            return saveAeroportos->arrayAeroportos[i];
        }
    }
    return NULL;
}

//funcao que cria a estrutura de aeroportos                  
void* criaSaveAeroportos(void* allFlights) 
{
    Save_voos* saveVoos = (Save_voos*)allFlights;

    Save_Aeroportos* saveAeroportos = malloc(sizeof(Save_Aeroportos));
    saveAeroportos->tamanho = 0;
    saveAeroportos->arrayAeroportos = malloc(sizeof(Aeroportos*) * 30);
    
    
    for (int i = 0; i < saveVoos->tamanho; i++) 
    {
        Voo* voo = (Voo*)saveVoos->voos_lista[i];

        char* partida = strdup(voo->origin);
        Aeroportos* aeroporto = encontrarAeroportoArray(saveAeroportos, partida);

        int data[6];
        dataToArray(voo->schedule_departure_date, data);

        if (aeroporto == NULL)   // o elemento ainda não existia
        {
            Aeroportos* aeroporto = malloc(sizeof(Aeroportos));
            aeroporto->name = strdup(voo->origin);
            aeroporto->p2023 = 0;
            aeroporto->p2022 = 0;
            aeroporto->p2021 = 0;
            aeroporto->total_voos = 1;
            aeroporto->delays = malloc(sizeof(int) * 50000);
            aeroporto->delays[0] = voo->delay;
            aeroporto->arrayVoos = malloc(sizeof(Voo) * 100000);
            aeroporto->arrayVoos[0] = voo;
            saveAeroportos->arrayAeroportos[saveAeroportos->tamanho] = aeroporto;

            if (data[0] == 2023) saveAeroportos->arrayAeroportos[saveAeroportos->tamanho]->p2023 += voo->passageiros;
            else if (data[0] == 2022) saveAeroportos->arrayAeroportos[saveAeroportos->tamanho]->p2022 += voo->passageiros;
            else if (data[0] == 2021) saveAeroportos->arrayAeroportos[saveAeroportos->tamanho]->p2021 += voo->passageiros;

            saveAeroportos->tamanho++;
        } 
        else   // o elemento já existe, então só atualiza as informações
        {

            aeroporto->total_voos++;
            aeroporto->delays[aeroporto->total_voos - 1] = voo->delay;
            aeroporto->arrayVoos[aeroporto->total_voos -1] = voo;

            if(data[0] == 2023) aeroporto->p2023 += voo->passageiros;
            else if(data[0] == 2022) aeroporto->p2022 += voo->passageiros;
            else if(data[0] == 2021) aeroporto->p2021 += voo->passageiros; 
        } 

        char* chegada = strdup(voo->destination);
        Aeroportos* aeroportoChegada = encontrarAeroportoArray(saveAeroportos, chegada);
        
        if(aeroportoChegada == NULL)
        {   
            Aeroportos* aeroporto = malloc(sizeof(Aeroportos));
            aeroporto->name = strdup(voo->destination);
            aeroporto->delays = malloc(sizeof(int) * 50000);
            aeroporto->total_voos = 0;
            aeroporto->p2023 = 0;
            aeroporto->p2022 = 0;
            aeroporto->p2021 = 0;
            aeroporto->arrayVoos = malloc(sizeof(Voo) * 100000);
            aeroporto->arrayVoos[0] = voo;
            saveAeroportos->arrayAeroportos[saveAeroportos->tamanho] = aeroporto;

            if (data[0] == 2023) saveAeroportos->arrayAeroportos[saveAeroportos->tamanho]->p2023 += voo->passageiros;
            else if (data[0] == 2022) saveAeroportos->arrayAeroportos[saveAeroportos->tamanho]->p2022 += voo->passageiros;
            else if (data[0] == 2021) saveAeroportos->arrayAeroportos[saveAeroportos->tamanho]->p2021 += voo->passageiros;

            saveAeroportos->tamanho++;
        }
        
        else
        {
            if(data[0] == 2023) aeroportoChegada->p2023 += voo->passageiros;
            else if(data[0] == 2022) aeroportoChegada->p2022 += voo->passageiros;
            else if(data[0] == 2021) aeroportoChegada->p2021 += voo->passageiros;
        }

        free(partida);
        free(chegada);
    }

    return saveAeroportos;
}


// auxiliar pra qsort
int comparaDelays(const void* a, const void* b) 
{
    return (*(int*)a - *(int*)b);
}

void calcularMedianaDelays(void* todosAeroportos) 
{
    Save_Aeroportos* saveAeroportos = (Save_Aeroportos*)todosAeroportos;
    for (int i = 0; i < saveAeroportos->tamanho; i++) 
    {
        Aeroportos* aeroporto = saveAeroportos->arrayAeroportos[i];

        if (aeroporto->total_voos > 0) 
        {
            qsort(aeroporto->delays, aeroporto->total_voos, sizeof(int), comparaDelays);  // ordena os delays usando qsort

            if (aeroporto->total_voos % 2 == 0)  // se for um número par de voos, calcula a média dos dois valores do meio
            {
                int meio1 = aeroporto->delays[(aeroporto->total_voos / 2) - 1];
                int meio2 = aeroporto->delays[aeroporto->total_voos / 2];
                aeroporto->mediana_delays = (meio1 + meio2) / 2;
            } 
            else //se for um número ímpar de voos, usa o valor do meio
            {
                aeroporto->mediana_delays = aeroporto->delays[aeroporto->total_voos / 2];
            }
        } 
        else aeroporto->mediana_delays = -1;   // se não houver voos, a mediana é 0
    }
}


// auxiliar pra qsort
int comparaMedianas(const void* a, const void* b) 
{
    const Aeroportos* aeroportoA = *(const Aeroportos**)a;
    const Aeroportos* aeroportoB = *(const Aeroportos**)b;

    // primeiro critério: mediana em ordem decrescente
    if (aeroportoA->mediana_delays != aeroportoB->mediana_delays) 
    {
        return aeroportoB->mediana_delays - aeroportoA->mediana_delays;
    }

    // segundo critério: ordem alfabética do origin
    return strcmp(aeroportoA->name, aeroportoB->name);
}

void ordenarPorMedianaDecrescente(void* aeroportos) 
{   
    Save_Aeroportos* saveAeroportos = (Save_Aeroportos*)aeroportos;
    qsort(saveAeroportos->arrayAeroportos, saveAeroportos->tamanho, sizeof(Aeroportos*), comparaMedianas);
}


// -------------------------------- AUXILIAR PRA Q6 ------------------------------

//funcoes responsaveis por ordenar os aeroportos que têm mais passageiros tendo em conta o ano que é pedido na query

int comparaPassageiros2023(const void* a, const void* b) 
{
    const Aeroportos* aeroportoA = *(const Aeroportos**)a;
    const Aeroportos* aeroportoB = *(const Aeroportos**)b;

    // primeiro critério: mediana em ordem decrescente
    if (aeroportoA->p2023 != aeroportoB->p2023) 
    {
        return aeroportoB->p2023 - aeroportoA->p2023;
    }
    
    // segundo critério: ordem alfabética do origin
    return strcmp(aeroportoA->name, aeroportoB->name);
}

int comparaPassageiros2022(const void* a, const void* b) 
{
    const Aeroportos* aeroportoA = *(const Aeroportos**)a;
    const Aeroportos* aeroportoB = *(const Aeroportos**)b;

    // primeiro critério: mediana em ordem decrescente
    if (aeroportoA->p2022 != aeroportoB->p2022) 
    {
        return aeroportoB->p2022 - aeroportoA->p2022;
    }

    // segundo critério: ordem alfabética do origin
    return strcmp(aeroportoA->name, aeroportoB->name);
}

int comparaPassageiros2021(const void* a, const void* b) 
{
    const Aeroportos* aeroportoA = *(const Aeroportos**)a;
    const Aeroportos* aeroportoB = *(const Aeroportos**)b;

    // primeiro critério: mediana em ordem decrescente
    if (aeroportoA->p2021 != aeroportoB->p2021) 
    {
        return aeroportoB->p2021 - aeroportoA->p2021;
    }

    // segundo critério: ordem alfabética do aeroporto
    return strcmp(aeroportoA->name, aeroportoB->name);
}

void ordenarPassageiros2023(void* aeroportos) 
{
    Save_Aeroportos* saveAeroportos = (Save_Aeroportos*)aeroportos;
    //for (int i = 0; i < saveAeroportos->tamanho; i++) printf("%s ", saveAeroportos->arrayAeroportos[i]->name);
    qsort(saveAeroportos->arrayAeroportos, saveAeroportos->tamanho, sizeof(Aeroportos*), comparaPassageiros2023);
}

void ordenarPassageiros2022(void* aeroportos) 
{
    Save_Aeroportos* saveAeroportos = (Save_Aeroportos*)aeroportos;
    qsort(saveAeroportos->arrayAeroportos, saveAeroportos->tamanho, sizeof(Aeroportos*), comparaPassageiros2022);
}

void ordenarPassageiros2021(void* aeroportos) 
{
    Save_Aeroportos* saveAeroportos = (Save_Aeroportos*)aeroportos;
    qsort(saveAeroportos->arrayAeroportos, saveAeroportos->tamanho, sizeof(Aeroportos*), comparaPassageiros2021);
}

//--------------------------------- FUNÇÕES AUXILIARES PARA Q5 ----------------------------

//funcao auxiliar para a funcao de ordenar os voos de um aeroporto por ordem decrescente
int comparaVoo(const void* a, const void* b)
{
    const Voo* aeroportoA = *(const Voo**)a;
    const Voo* aeroportoB = *(const Voo**)b;

    return strcmp(aeroportoB->schedule_departure_date, aeroportoA->schedule_departure_date);
}

//funcao q ordena os voos de um aeroporto por ordem decrescente da sua schedule departure rate
void ordenaVooDecrescente(void* aeroporto_t)
{
    Aeroportos* aeroporto = (Aeroportos*)aeroporto_t;
    qsort(aeroporto->arrayVoos, aeroporto->total_voos, sizeof(Voo*), comparaVoo);
}

