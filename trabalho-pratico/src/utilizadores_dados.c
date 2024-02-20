#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <strings.h>
#include <locale.h>
#include <ctype.h>
#include "../include/utilizadores_dados.h"
#include "../include/datas.h"
#include "../include/utilizadores.h"
#include "../include/utilidades.h"
#include "../include/voos_dados.h"

typedef struct save_users { 
    GHashTable* users_hashtable;
    void** users_lista;
    int tamanho;
} Save_users;

typedef struct user {
    char* id; //username
    char name[50];
    char birth_date[12];
    char sex[2]; // M e F
    char passport[10]; // e.g: LK12938
    char country_code[3];
    char account_creation[21];
    char account_status[9];
    char* ano_creation;
    char* voos[50];   
    int total_voos;
    char* reserva[50];
    int total_reservas;
    int seen21;
    int seen22;
    int seen23;
    double preco_gasto_reservas;
    char* voos_e_reservas[100];
    int total_voos_e_reservas;
    int voos_e_reservas_sorted;
    int reserva_sorted;
    int voos_sorted;
} User;

// verfica se o country code é válido
int countrycodeValido (char* code) { 
    if (!code) return 0;
    char* temp = g_strdup(code);
    for (int i = 0; i < strlen(temp); i++) {
        if (!isalpha(temp[i])) {free(temp); return 0;}
    }
    if (strlen(temp) != 2) {free(temp); return 0;}
    free(temp);
    return 1;
}

// verifica se o email é válido
int emailValido (char* email1, char* username) { 
    if (!email1) return 0;
    char* email = g_strdup(email1);
    if (!email) {free(email); return 0;}
    int n = strlen(email);
    int narr = 0;
    for (int i = 0; i < n; i++) {
        if (email[i] == '@') narr++;    
    }
    if (narr != 1) {free(email); return 0;}

    char* saveprt1;    
    char *token = strtok_r(email, "@", &saveprt1);
    
    if (token == NULL) {free(email); return 0;}
    if (strlen(token) < 1) {free(email); return 0;}
    token = strtok_r(NULL, "@", &saveprt1);
    if (!token) {free(email); return 0;}
    if (strlen(token) < 1) {free(email); return 0;}
    free(email);
    return 1;
}

//verifica se o utilizador é valido e caso for, inicia as suas variáveis e retorna uma stuct de usuarios
void *verificaUser(char** info, void* null, FILE* resultado, void* stats) 
{
    if (!info[11] || isEmpty(info[0]) == 1 || isEmpty(info[1]) == 1 || emailValido(info[2], info[0]) == 0 ||
        isEmpty(info[3]) == 1 || verifica_Data(info[4]) == 0 || isEmpty(info[5]) == 1 ||
        isEmpty(info[6]) == 1 || countrycodeValido(info[7]) == 0  || isEmpty(info[8]) == 1 ||
        isEmpty(info[9]) == 1 || verifica_Horas(info[9]) == 0 || isEmpty(info[10]) == 1 || (isEmpty(info[11])) == 1 || 
        strcmp(info[4], info[9]) >= 0 || accountStatValido(info[11]) == 0)
        {
            int t = 12;
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
        else {   

            struct user *u = malloc(sizeof(struct user));
            if (!u) return NULL;
            
            u->id = strdup(info[0]);
            strcpy(u->name, info[1]);
            strcpy(u->birth_date, info[4]);
            strcpy(u->sex, info[5]);
            strcpy(u->passport, info[6]);
            strcpy(u->country_code, info[7]);
            strcpy(u->account_creation, info[9]);
            strcpy(u->account_status, info[11]);
            u->ano_creation = dataGetAno(strdup(info[9]));
            u->seen21 = 0;
            u->seen22 = 0;
            u->seen23 = 0;
            u->total_voos = 0;
            u->total_reservas = 0;
            u->preco_gasto_reservas = 0;
            u->total_voos_e_reservas = 0;
            u->voos_e_reservas_sorted = 0;
            u->reserva_sorted = 0;
            u->voos_sorted = 0;
            addStats(u->account_creation, stats, u->ano_creation, NULL, NULL);
            return u;
        }
    
}

//funcoes de get e set

int getUserSeen (void* user, int ano) {
    User* usuario = (User*)user;
    if (ano == 21) return usuario->seen21;
    if (ano == 22) return usuario->seen22;
    if (ano == 23) return usuario->seen23;
    return 0;
}

void setUserSeen (void* user, int ano) {
    User* usuario = (User*)user;
    if (ano == 21) usuario->seen21 = 1;
    if (ano == 22) usuario->seen22 = 1;
    if (ano == 23) usuario->seen23 = 1;
}

void setUserReservation (void *user_dados, char* name, void *reservas_dados, char* reserva) {
    Save_users* users = (Save_users*)user_dados;
    User* currentUser = g_hash_table_lookup(users->users_hashtable, name);
    currentUser->reserva[getUserTotalReservations(user_dados, name)] = reserva;
}

void setUserFlights (void *user_dados, char* name, void* flight_dados, char* voo) {
    Save_users* users = (Save_users*)user_dados;
    User* currentUser = g_hash_table_lookup(users->users_hashtable, name);
    currentUser->voos[getUserTotalFlights(user_dados, name)] = getFlightId_V(flight_dados, voo);
}

int getUserTotalFlights (void *user_dados, char* name) {
    Save_users* users = (Save_users*)user_dados;
    User* currentUser = g_hash_table_lookup(users->users_hashtable, name);
    return currentUser->total_voos;
}

int getUserTotalReservations (void *user_dados, char* name) {
    Save_users* users = (Save_users*)user_dados;
    User* currentUser = g_hash_table_lookup(users->users_hashtable, name);
    return currentUser->total_reservas;
}

void addUserFlight (void *user_dados, char* name) {
    Save_users* users = (Save_users*)user_dados;
    User* currentUser = g_hash_table_lookup(users->users_hashtable, name);
    currentUser->total_voos++;
}

void addUserReserva (void *user_dados, char* name) {
    Save_users* users = (Save_users*)user_dados;
    User* currentUser = g_hash_table_lookup(users->users_hashtable, name);
    currentUser->total_reservas++;
}

void precoGastoReservas (void *user_dados, char* name, double preco) {
    Save_users* users = (Save_users*)user_dados;
    User* currentUser = g_hash_table_lookup(users->users_hashtable, name);
    currentUser->preco_gasto_reservas += preco;
}

void* getUser (void *user_dados, char* name) {
    Save_users* users = (Save_users*)user_dados;
    User* user = g_hash_table_lookup(users->users_hashtable, name);
    return user;
}

char* getUserName (void *user_dados) {
    User* user = (User*)user_dados;
    return user->name;
}

char* getUserSex (void *user_dados) {
    User* user = (User*)user_dados;
    return user->sex;
}

char* getUserCountryC (void *user_dados) {
    User* user = (User*)user_dados;
    return user->country_code;
}

char* getUserPassport (void *user_dados) {
    User* user = (User*)user_dados;
    return user->passport;
}

int getUserNFlights (void *user_dados) {
    User* user = (User*)user_dados;
    return user->total_voos;
}

int getUserNReser (void *user_dados) {
    User* user = (User*)user_dados;
    return user->total_reservas;
}

double getUserGastoReserv (void *user_dados) {
    User* user = (User*)user_dados;
    return user->preco_gasto_reservas;
}

char* getUserStatus (void *user_dados) {
    User* user = (User*)user_dados;
    return user->account_status;
}

char* getUserBirthdate (void *user_dados) {
    User* user = (User*)user_dados;
    return user->birth_date;
}

int getUserVoosSorted (void* user_dados) {
    User* user = (User*)user_dados;
    return user->voos_sorted;
}

int getUserReservasSorted (void* user_dados) {
    User* user = (User*)user_dados;
    return user->reserva_sorted;
}

int getUserVoosReservasSorted (void *user_dados) {
    User* user = (User*)user_dados;
    return user->voos_e_reservas_sorted;
}

char* getUserFlightArr (void* user_dados, int ind) {
    User* user = (User*)user_dados;
    return user->voos[ind];
}

char* getUserReservArr (void* user_dados, int ind) {
    User* user = (User*)user_dados;
    return user->reserva[ind];
}

char* getUserFnRArr (void *user_dados, int ind) {
    User* user = (User*)user_dados;
    return user->voos_e_reservas[ind];
}

int getUserTotalFnR(void *user_dados) {
    User* user = (User*)user_dados;
    return user->total_voos_e_reservas;
}

void* getUserArray (void *user_dados, int ind) {
    Save_users* users = (Save_users*)user_dados;
    return users->users_lista[ind];
}

int getSaveUserTamanho (void *user_dados) {
    Save_users* users = (Save_users*)user_dados;
    return users->tamanho;
}

char* getUserId (void *user_dados) {
    User* user = (User*)user_dados;
    return user->id;
}

//organiza todos os utilizadores validos numa hashtable e numa lista, a tag utilizada na hashtable é o id do user
void *organizaUser (void** array) {
    GHashTable* tabela = g_hash_table_new(g_str_hash, g_str_equal);

    int i;
    for(i = 0; array[i]; i++){
        User* usuario = array[i];
        g_hash_table_insert(tabela, (usuario->id), usuario);
    }

    Save_users* save_users = malloc(sizeof(Save_users));
    save_users->users_lista = array;
    save_users->users_hashtable = tabela;
    save_users->tamanho = i;

    return save_users;
}

// procura se um utilizador é valido, isto é, se está na hashtable
int procuraUser(void *tabela2, char* user) { 
    Save_users* tabela = (Save_users*)tabela2;
    gpointer valorEncontrado = g_hash_table_lookup(tabela->users_hashtable, user);
    if (valorEncontrado != NULL) {      
        return 1;
    } else {
        return 0;
    }
}

// ---------------------------------- FREES ----------------------------------

//função que dá free da data dos users
void free_users_data(void* data) {
    if (data == NULL) {
        return;
    }

    Save_users* save_users = (Save_users*)data;

    for (int i = 0; save_users->users_lista[i]; i++) 
    {
        User* currentUser = (User*)save_users->users_lista[i];

        // liberta a memória de cada campo da struct
        free(currentUser->id);
        free(currentUser->ano_creation);

        // liberta a memória para cada voo ou reserva na lista 
        for (int j = 0; j < currentUser->total_voos_e_reservas; j++) {
            free(currentUser->voos_e_reservas[j]);
        }

        free(currentUser);
    }

    g_hash_table_destroy(save_users->users_hashtable);
    free(save_users->users_lista);
    free(save_users);
}