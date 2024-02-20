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
    int cont;
    int tamanho_arr;
    int tamanho;
} Hoteis;

typedef struct save_hoteis { 
    GHashTable* tabela;
    int tamanho;
    char** hoteis_lista;
    int tamanho_lista;
} Save_Hoteis;

// hotel_stars valor inteiro entre 1 e 5 (inclusive)
int isHotelStarsValid(int hotel_stars) {
    if (hotel_stars >= 1 && hotel_stars <= 5) return 1;
    return 0;
}

// city_tax valor inteiro maior ou igual a 0
int isCityTaxValid(int city_tax) {
    if (city_tax >= 0) return 1;
    return 0;
}

// price_per_night valor inteiro  maior que 0
int isPricePerNightValid(char* price_per_night) {
    for (int i = 0; i < strlen(price_per_night); i++) {
        if (!isdigit(price_per_night[i])) return 0;
    }
    if (atoi(price_per_night) > 0) return 1;
    return 0;
}

// includes_breakfast: falso -> "f", "false", "0" ou "" string vazia; verdadeiro -> "t", "true", "1"
int isIncludesBreakfastValid(char* includes_breakfast) {
    if (strcmp(includes_breakfast, "") == 0) return 1;

    if (strcasecmp(includes_breakfast, "f") == 0 || strcasecmp(includes_breakfast, "false") == 0 ||
            strcasecmp(includes_breakfast, "0") == 0 ||
            strcasecmp(includes_breakfast, "t") == 0 || strcasecmp(includes_breakfast, "true") == 0 ||
            strcasecmp(includes_breakfast, "1") == 0) {
                return 1;
    }

    return 0;
}

// ratinf valor inteiro entre 1 e 5 (inclusive) ou opcionalmente vazio
int isRatingValid(char* rating) {
    if(strcmp(rating, "\n") == 0) return 1;
    int ratingValue = atoi(rating);
    if (ratingValue < 1 || ratingValue > 5) return 0;
    else return 1;
}

//função que verifica se uma dada reserva é válida
void *verificaReserva(char** info, void* userdados, FILE* resultado, void* stats)
{
    if(isEmpty(info[0]) == 1 || isEmpty(info[1]) == 1 || isEmpty(info[2]) == 1 || isEmpty(info[3]) == 1 || strlen(info[4]) != 1 ||
       isHotelStarsValid(atoi(info[4])) == 0 || strlen(info[5]) != 1 || isCityTaxValid(atoi(info[5])) == 0 || isEmpty(info[6]) == 1 || 
       verifica_DuasDatas(info[7], info[8], 0) == 0 || isPricePerNightValid((info[9])) == 0 || 
       isIncludesBreakfastValid(info[10]) == 0 || isRatingValid(info[12]) == 0 || procuraUser(userdados, info[1]) == 0)
       {
        // da print aos erros no ficheiro
        int t = 14;
        for(int k = 0; k < t; k++)
        {
            if(info[k] == NULL) info[k] = g_strdup("");
        }


        fprintf(resultado, "%s", info[0]);

        for (int k = 1; k < t; k++) {
            fprintf(resultado, ";%s", info[k]);
        }

        fprintf(resultado, "\n");

        return NULL;
    }
    else{
        struct reservation *r = malloc(sizeof(struct reservation));
        if(!r) return NULL;

        User* currentUser = getUser(userdados, info[1]);

        strcpy(r->id,info[0]);
        r->user_id = getUserId(currentUser);
        strcpy(r->hotel_id, info[2]);
        strcpy(r->hotel_name, info[3]);
        r->hotel_stars = atoi(info[4]);
        r->city_tax = atoi(info[5]);
        strcpy(r->begin_date, info[7]);
        strcpy(r->end_date, info[8]);
        r->price_per_night = atoi(info[9]);
        strcpy(r->includes_breakfast, info[10]);
        strcpy(r->rating, info[12]);
        r->n_noites = calculaNoites(r->begin_date, r->end_date);
        r->begin_ano = dataGetAno(strdup(info[7]));
        addStats(r->begin_date, stats, r->begin_ano, NULL, NULL);

        //printf("%s\n", r->id); // para testar apenas

        return r;
    }
}

//função que insere as linhas das reservas que são válidas, numa estrutura de hashtable e array
void *organizaReserva(void** array) {
    GHashTable* tabela = g_hash_table_new(g_str_hash, g_str_equal);  // cria a tabela

    int i;
    for(i = 0; array[i]; i++){
        Reservation* reservations = array[i];     // percorre o array de structs
        g_hash_table_insert(tabela, (reservations->id), reservations);   // insere-os na hashtable
    }

    Save_reservations* save_reservations = malloc(sizeof(Save_reservations));   
    save_reservations->reservations_lista = array;          // guarda os dados em forma de array de structs
    save_reservations->reservations_hashtable = tabela;     // guarda os dados em forma de hashtable
    save_reservations->tamanho = i;

    return save_reservations;
}
//verifica se um dado hotel existe no save de hoteis, caso existir retorna 1, senao 0
int procuraHotel (void *tabela, char* tag) {
    Save_Hoteis* hoteis = (Save_Hoteis*)tabela;
    Hoteis* valorEncontrado = g_hash_table_lookup(hoteis->tabela, tag);

    if (valorEncontrado != NULL) {
        return 1;
    } else {
        return 0;
    }
}

//função que procura se uma tal reserva existe nas reservas válidas
int procuraReservas(void *tabela, char* reserva) 
{   
    Save_reservations* reservas = (Save_reservations*)tabela;
    Reservation* valorEncontrado = g_hash_table_lookup(reservas->reservations_hashtable, reserva);

    if (valorEncontrado != NULL) {
        return 1;
    } else {
        return 0;
    }
}

//dá free das reservas
void free_reservations_data(void* data) 
{
    if (data == NULL) {
        return;
    }

    Save_reservations* save_reservations = (Save_reservations*)data;

    for (int i = 0; save_reservations->reservations_lista[i]; i++) 
    {
        Reservation* currentReservation = (Reservation*)save_reservations->reservations_lista[i];
        free(currentReservation->begin_ano);
        free(currentReservation);
    }

    GHashTable* tabela = (GHashTable*)save_reservations->reservations_hashtable;
    g_hash_table_destroy(tabela);
    free(save_reservations->reservations_lista);
    free(save_reservations);
}

//free da data dos hoteis
void free_hoteis_data(void* hoteis) 
{
    Save_Hoteis* save_hoteis = (Save_Hoteis*)hoteis;
    if (save_hoteis == NULL) {
        return;
    }
    for (int i = 0; i < save_hoteis->tamanho_lista; i++) {
        Hoteis* currentHotel = g_hash_table_lookup(save_hoteis->tabela, save_hoteis->hoteis_lista[i]);
        if (!currentHotel) return;
        free(currentHotel->arrayReservas);   
        free(currentHotel->arrayQ4);     
        free(currentHotel->ratings);    
        free(currentHotel);
    }

    GHashTable* tabela = (GHashTable*)save_hoteis->tabela;
    g_hash_table_destroy(tabela);
    free(save_hoteis->hoteis_lista);
    free(save_hoteis);

} 

//FUNCOES DE GET E SET

char* getReservationHotelId_Reserv (void* reservas_dados) {
    Reservation* reservas = (Reservation*)reservas_dados;
    return reservas->hotel_id;
}

char* getReservationId (void* reservas_dados, char* reservation) {
    Save_reservations* reservas = (Save_reservations*)reservas_dados;
    Reservation* reserva = g_hash_table_lookup(reservas->reservations_hashtable, reservation);
    return reserva->id;
}

char* getReservationUserId (void* reservas_dados, char* reservation) {
    Save_reservations* reservas = (Save_reservations*)reservas_dados;
    Reservation* reserva = g_hash_table_lookup(reservas->reservations_hashtable, reservation);
    return reserva->user_id;
}

void* getReservation(void* reservas_dados, char* chave) {
    Save_reservations* reservas = (Save_reservations*)reservas_dados;
    Reservation* reserva = g_hash_table_lookup(reservas->reservations_hashtable, chave);
    return reserva;
}

char* getBreakfast_Reserv (void* reservas_dados) {
    Reservation* reserva = (Reservation*)reservas_dados;
    return reserva->includes_breakfast;
}

char* getReservationHotelName_Reserv (void* reservas_dados) {
    Reservation* reservas = (Reservation*)reservas_dados;
    return reservas->hotel_name;
}

int getReservationHotelStars_Reserv (void* reservas_dados) {
    Reservation* reservas = (Reservation*)reservas_dados;
    return reservas->hotel_stars;
}

char* getReservationBeginDate_Reserv (void* reservas_dados) {
    Reservation* reservas = (Reservation*)reservas_dados;
    return reservas->begin_date;
}

char* getReservationEndDate_Reserv (void* reservas_dados) {
    Reservation* reservas = (Reservation*)reservas_dados;
    return reservas->end_date;
}

int getNnoites_Reserv (void* reservas_dados) {
    Reservation* reservas = (Reservation*)reservas_dados;
    return reservas->n_noites;
}

void* getHotel (void* hotel_dados, char* tag) {
    Save_Hoteis* hoteis = (Save_Hoteis*)hotel_dados;
    Hoteis* currentHotel = g_hash_table_lookup(hoteis->tabela, tag);
    return currentHotel;
}

int getHotelTamanho (void* hotel_dados) {
    Hoteis* hotel = (Hoteis*)hotel_dados;
    return hotel->tamanho; 
}

void* getHotelReserv (void* hotel_dados, int ind) {
    Hoteis* hotel = (Hoteis*)hotel_dados;
    return hotel->arrayQ4[ind];
}

char* getReservationId_Reserv (void* reservas_dados) {
    Reservation* reserva = (Reservation*)reservas_dados;
    return reserva->id;
}

char* getReservationUserId_Reserv (void* reservas_dados) {
    Reservation* reserva = (Reservation*)reservas_dados;
    return reserva->user_id;
}

char* getReservationRating (void* reservas_dados) {
    Reservation* reserva = (Reservation*)reservas_dados;
    return reserva->rating;
}

char* getHotelEndDate (void* hotel_dados, int ind) {
    Hoteis* hotel = (Hoteis*)hotel_dados;
    return hotel->arrayReservas[ind]->end_date;
}

char* getHotelBeginDate (void* hotel_dados, int ind) {
    Hoteis* hotel = (Hoteis*)hotel_dados;
    return hotel->arrayReservas[ind]->begin_date;
}

int getHotelPricePerNight (void* hotel_dados, int ind) {
    Hoteis* hotel = (Hoteis*)hotel_dados;
    return hotel->arrayReservas[ind]->price_per_night;
}

 //função que cria a estrutura dos hoteis e insere quais as reservas e rating de cada hotel
void *insereMediaHoteis (void* todas_reservas) {
    Save_reservations* reservas = (Save_reservations*)todas_reservas;
    int tam = 10000;
    void** array = reservas->reservations_lista;
    char** save_array = malloc(sizeof(char*) * 1000); //nº de hoteis
    int cont = 0;
    GHashTable* media_hoteis = g_hash_table_new(g_str_hash, g_str_equal);
    int j;
    for (j = 0; array[j] != NULL; j++) {
        Reservation* reservations = array[j];
        gpointer hotelEncontrado = g_hash_table_lookup(media_hoteis, reservations->hotel_id);
        if (hotelEncontrado != NULL) {
            Hoteis *todosHoteis = hotelEncontrado;
            if ((reservations->rating) == NULL) {} 
            else {
                todosHoteis->cont++;
                if (todosHoteis->cont * 2>= todosHoteis->tamanho_arr) {
                    todosHoteis->ratings = realloc(todosHoteis->ratings, sizeof(char*) * todosHoteis->tamanho_arr);
                    todosHoteis->arrayReservas = realloc(todosHoteis->arrayReservas, sizeof(Reservation) * 2 * todosHoteis->tamanho_arr);
                    todosHoteis->arrayQ4 = realloc(todosHoteis->arrayQ4, sizeof(Reservation) * 2 * todosHoteis->tamanho_arr);
                    todosHoteis->tamanho_arr *= 2;
                }

                todosHoteis->ratings[todosHoteis->tamanho] = (reservations->rating);
                todosHoteis->arrayReservas[todosHoteis->tamanho] = reservations;
                todosHoteis->arrayQ4[todosHoteis->tamanho] = reservations;
                todosHoteis->tamanho++;
            }

        }
        else {
            if ((reservations->rating) != NULL) {
                Hoteis *todosHoteis = malloc(sizeof(Hoteis));
                if (!todosHoteis) return NULL;
                todosHoteis->ratings = malloc(sizeof(char*)*100); //da para melhorar
                todosHoteis->arrayReservas = malloc(sizeof(Reservation)*100);
                todosHoteis->arrayQ4 = malloc(sizeof(Reservation)*100);
                todosHoteis->arrayReservas[0] = reservations;
                todosHoteis->arrayQ4[0] = reservations;
                todosHoteis->hotel_id = (reservations->hotel_id);
                todosHoteis->cont = 1;
                todosHoteis->tamanho_arr = 100;
                todosHoteis->ratings[0] = (reservations->rating);

                if (cont * 2 >= tam) {
                    save_array = realloc(save_array, sizeof(char*)*tam*2);
                    tam *= 2;
                }

                save_array[cont++] = (reservations->hotel_id);
                todosHoteis->tamanho = 1;
                g_hash_table_insert(media_hoteis, todosHoteis->hotel_id, todosHoteis);
            } 
        }
    }

    Save_Hoteis* todosHoteis = malloc(sizeof(Save_Hoteis));
    if (!todosHoteis) return NULL;

    todosHoteis->tamanho = j;
    todosHoteis->tabela = media_hoteis;
    todosHoteis->tamanho_lista = cont;
    todosHoteis->hoteis_lista = save_array;
    return (void*)todosHoteis;
}