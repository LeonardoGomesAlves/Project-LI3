#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/datas.h"
#include "../include/reservas.h"
#include "../include/utilizadores.h"
#include "../include/utilidades.h"
#include "../include/utilizadores_dados.h"
#include "../include/reservas_dados.h"

typedef struct reservation {
    char id[15];  
    char* user_id; 
    char hotel_id[8];
    char hotel_name[35]; 
    int hotel_stars;
    int city_tax;
    char begin_date[12]; 
    char* begin_ano; 
    char end_date[12];
    int price_per_night;
    char includes_breakfast[6];
    char rating[2];
    int n_noites;
} Reservation;


typedef struct save_reservations { 
    GHashTable* reservations_hashtable;
    void** reservations_lista;
    int tamanho;
} Save_reservations;

typedef struct hoteis {
    char* hotel_id;
    char** ratings;
    char** datas;
    Reservation** arrayReservas;
    Reservation** arrayQ4;
    int tamanho;
} Hoteis;

typedef struct save_hoteis { 
    GHashTable* tabela;
    int tamanho;
    char** hoteis_lista;
    int tamanho_lista;
} Save_Hoteis;

//calcula o custo de uma dada reserva
double calculaCustoPerNoite(void* reservas)
{  
    Reservation* reserva = (Reservation*)reservas;
    double custo = reserva->price_per_night * reserva->n_noites + ((reserva->price_per_night * reserva->n_noites)/100.0) * reserva->city_tax;

    return custo;
}


//insere as reservas no respetivo utilizador
void insereReservas_User(void *utilizadores, void *reservas) { 
    void** arrayReservas = ((Save_reservations*)reservas)->reservations_lista;
    for (int i = 0; arrayReservas[i] != NULL; i++){
        Reservation* reservaToInsert = arrayReservas[i];
        setUserReservation(utilizadores, reservaToInsert->user_id, reservas, reservaToInsert->id);
        addUserReserva(utilizadores, reservaToInsert->user_id);
        precoGastoReservas(utilizadores, reservaToInsert->user_id, calculaCustoPerNoite(reservaToInsert));
    }    
}

//ordena as reservas de um dado hotel por ordem decrescente, relativo à begin date das reservas
void ordenaDataDescrescente (Save_Hoteis* todosHoteis, char* hotel) {
    Hoteis* currentHotel = g_hash_table_lookup(todosHoteis->tabela, hotel);    
    qsort(currentHotel->arrayQ4, currentHotel->tamanho, sizeof(Reservation*), comparaDataDec);
}

//funcao auxiliar para a funcao de ordenar as reservas de um hotel por data decrescente
int comparaDataDec (const void* a, const void* b) {
    const Reservation* reservaA = *(const Reservation**)a;
    const Reservation* reservaB = *(const Reservation**)b;

    int k = strcmp(reservaB->begin_date, reservaA->begin_date);
    if (k != 0) {
        return k;
    } else return strcmp(reservaA->id, reservaB->id);
    return 0;
}

//ordena as reservas de um dado hotel por ordem crescente, relativo à begin data das reservas
void ordenaPorData (void* hoteis, char* hotel) {
    Save_Hoteis* todosHoteis = (Save_Hoteis*)hoteis;
    Hoteis* currentHotel = g_hash_table_lookup(todosHoteis->tabela, hotel);    
    qsort(currentHotel->arrayReservas, currentHotel->tamanho, sizeof(Reservation*), comparaDataQ);
}

//funcao auxiliar para a funcao de ordenar as reservas de um hotel por data crescente
int comparaDataQ (const void* a, const void* b) {
    const Reservation* reservaA = *(const Reservation**)a;
    const Reservation* reservaB = *(const Reservation**)b;

    //printf("%s\n", reservaA->id);

    int k = strcmp(reservaA->begin_date, reservaB->begin_date);
    if (k != 0) {
        return k;
    } else return strcmp(reservaA->id, reservaB->id);
    return 0;

}


//calula a media de rating de um dado hotel
double calculaMediaHotel (void* hoteis, char* tag) {
    Save_Hoteis* todosHoteis = (Save_Hoteis*)hoteis;
    Hoteis* hotel = g_hash_table_lookup(todosHoteis->tabela, tag);

    double sum = 0;
    for (int i = 0; i < hotel->tamanho; i++) {
        sum += (atoi(hotel->ratings[i]));
    }
    double ret = sum/hotel->tamanho;
    return (ret);
}

//função que organiza os dados de um hotel por data
void sortBookingHoteis (void* reservas, void* hoteis, char* tag) {
    Save_Hoteis* todosHoteis = (Save_Hoteis*)hoteis;
    
    ordenaDataDescrescente(todosHoteis, tag);

}