#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <strings.h>
#include <locale.h>
#include <ctype.h>
#include "../include/datas.h"
#include "../include/reservas.h"
#include "../include/utilizadores.h"
#include "../include/utilidades.h"
#include "../include/voos.h"
#include "../include/utilizadores_dados.h"
#include "../include/voos_dados.h"
#include "../include/reservas_dados.h"

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


//verifica se o utilizador é inativo
int isInactive(char* str) { 
    if (!str) return 0;
    char* temp = malloc(sizeof(str)*1000);
    strcpy(temp, str);

    if (strcasecmp(temp, "inactive") == 0){
        free(temp);
        return 1;
    }

    free(temp);
    return 0;
}

//utilizado nas queries 1 e 2, verifica se o account status pode ser válido, isto é, ou é active  ou inactive
int accountStatValido (char* asv) { 
    if (asv == NULL) return 0;
    if (strcasecmp(asv, "active") == 0 || strcasecmp(asv, "inactive") == 0) {
        
        return 1;
    }
    
    return 0;
}

//organiza as reservas por datas e, caso sejam ambas iguais, organiza pelo id da reserva
void organizaReservasUser (void* users, void* reservas, char* tag) {
    Save_users* utilizadores = (Save_users*)users;
    User* pessoa = g_hash_table_lookup(utilizadores->users_hashtable, tag);
    int total = pessoa->total_reservas;
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            void* reservaj = getReservation(reservas, pessoa->reserva[j]);
            void* reservamais1 = getReservation(reservas, pessoa->reserva[j+1]);
            if (verifica_DuasDatas(getReservationBeginDate_Reserv(reservaj), getReservationBeginDate_Reserv(reservamais1), 0) == 1) {
                char* temp = pessoa->reserva[j];
                pessoa->reserva[j] = pessoa->reserva[j+1];
                pessoa->reserva[j+1] = temp;               
            } else {
            if (datasIguais(getReservationBeginDate_Reserv(reservaj), getReservationBeginDate_Reserv(reservamais1), 0) == 1) {
                if (strcmp(getReservationId(reservas, pessoa->reserva[j]), getReservationId(reservas, pessoa->reserva[j+1])) > 0) {
                    char* temp = pessoa->reserva[j];
                    pessoa->reserva[j] = pessoa->reserva[j+1];
                    pessoa->reserva[j+1] = temp;
                }
            }
            }
        }
    }
    pessoa->total_voos_e_reservas += pessoa->total_reservas;
    for (int i = 0; i < pessoa->total_reservas; i++) {
        pessoa->voos_e_reservas[i] = strdup(pessoa->reserva[i]);
    }
    pessoa->reserva_sorted = 1;
}

//organiza os voos de um utilizador por data, caso sejam ambas iguais, organiza pelo fligh_id
void organizaVoosUser (void* users, void* flights, char* tag) { 
    Save_users* utilizadores = (Save_users*)users;
    User* pessoa = g_hash_table_lookup(utilizadores->users_hashtable, tag);
    int total = pessoa->total_voos;
    for (int tt = 0; tt < 2; tt++){
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            void* vooPrimeiro = getFlight(flights, pessoa->voos[j]);
            void* vooSegundo = getFlight(flights, pessoa->voos[j+1]);
            if (verifica_DuasDatas(getFlightSchDepa(vooPrimeiro), getFlightSchDepa(vooSegundo), 1) == 1) {
                char* temp = pessoa->voos[j];
                pessoa->voos[j] = pessoa->voos[j+1];
                pessoa->voos[j+1] = temp;               
            } else {
                if (datasIguais(getFlightSchDepa(vooPrimeiro), getFlightSchDepa(vooSegundo), 1) == 1) {
                    if(strcmp(getFlightId(vooPrimeiro), getFlightId(vooSegundo)) >0) {
                        char* temp = pessoa->voos[j];
                        pessoa->voos[j] = pessoa->voos[j+1];
                        pessoa->voos[j+1] = temp;  
                    }
                }
            }
        }
    }
    }
    pessoa->total_voos_e_reservas += pessoa->total_voos;
    for (int i = pessoa->total_voos_e_reservas - pessoa->total_voos, j = 0; i < pessoa->total_voos_e_reservas; i++, j++) {
        pessoa->voos_e_reservas[i] = strdup(pessoa->voos[j]);
    }
    pessoa->voos_sorted = 1;
}

//organiza os voos e reservas de um utilizador pelas datas, caso sejam ambas iguais, organiza pelo id
void organiza_VoosEReservasUser (void* users, void* flights, void* reservas, char* tag) { 
    Save_users* utilizadores = (Save_users*)users;
    User* pessoa = g_hash_table_lookup(utilizadores->users_hashtable, tag);                                                
    int total = pessoa->total_voos_e_reservas;
    for (int vezes = 0; vezes < 2; vezes++){
        for (int i = 0; i < total - 1; i++) {
            for (int j = 0; j < total - i - 1; j++) {
            
                if (isVoo(pessoa->voos_e_reservas[j]) == 1 && isVoo(pessoa->voos_e_reservas[j+1]) == 1) {
                    void* vooPrimeiro = getFlight(flights, pessoa->voos_e_reservas[j]);
                    void* vooSegundo = getFlight(flights, pessoa->voos_e_reservas[j+1]);
                    if (verifica_DuasDatas(getFlightSchDepa(vooPrimeiro), getFlightSchDepa(vooSegundo), 1) == 1) {
                        char* temp = pessoa->voos_e_reservas[j];
                        pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                        pessoa->voos_e_reservas[j+1] = temp;               
                    } else {
                        if (datasIguais(getFlightSchDepa(vooPrimeiro), getFlightSchDepa(vooSegundo), 1) == 1) {
                            if(strcmp(getFlightId(vooPrimeiro), getFlightId(vooSegundo)) >0) {
                                char* temp = pessoa->voos_e_reservas[j];
                                pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                                pessoa->voos_e_reservas[j+1] = temp;  
                            }
                        }
                }
                } else if (isVoo(pessoa->voos_e_reservas[j]) == 1 && isVoo(pessoa->voos_e_reservas[j+1]) == 0) {
                    void* vooPrimeiro = getFlight(flights, pessoa->voos_e_reservas[j]);
                    void* reservaSegundo = getReservation(reservas, pessoa->voos_e_reservas[j+1]);
                    char* resultado = (char*)malloc(sizeof(getReservationBeginDate_Reserv(reservaSegundo)) + strlen(" 00:00:00") + 10);
                    adicionarHorasData(getReservationBeginDate_Reserv(reservaSegundo), resultado);
                    if (verifica_DuasDatas(getFlightSchDepa(vooPrimeiro), resultado, 1) == 1) {
                        char* temp = pessoa->voos_e_reservas[j];
                        pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                        pessoa->voos_e_reservas[j+1] = temp;
                    } else {
                        if (datasIguais(getFlightSchDepa(vooPrimeiro), resultado, 1) == 1) {
                            if(strcmp(getFlightId(vooPrimeiro), getReservationId(reservas, pessoa->voos_e_reservas[j+1])) > 0) {
                                char* temp = pessoa->voos_e_reservas[j];
                                pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                                pessoa->voos_e_reservas[j+1] = temp;
                            }
                        }
                    }
                    free(resultado);
    
                } else if (isVoo(pessoa->voos_e_reservas[j]) == 0 && isVoo(pessoa->voos_e_reservas[j+1]) == 0) {
                    void* reservaPrimeiro = getReservation(reservas, pessoa->voos_e_reservas[j]);
                    void* reservaSegundo = getReservation(reservas, pessoa->voos_e_reservas[j+1]);
                    if (verifica_DuasDatas(getReservationBeginDate_Reserv(reservaPrimeiro), getReservationBeginDate_Reserv(reservaSegundo), 0) == 1) {
                        char* temp = pessoa->voos_e_reservas[j];
                        pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                        pessoa->voos_e_reservas[j+1] = temp;               
                    } else {
                    if (datasIguais(getReservationBeginDate_Reserv(reservaPrimeiro), getReservationBeginDate_Reserv(reservaSegundo), 0) == 1) {
                        if (strcmp(getReservationId(reservas, pessoa->voos_e_reservas[j]), getReservationId(reservas, pessoa->voos_e_reservas[j+1])) > 0) {
                            char* temp = pessoa->voos_e_reservas[j];
                            pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                            pessoa->voos_e_reservas[j+1] = temp;
                        }
                    }
                }
    
                } else if (isVoo(pessoa->voos_e_reservas[j]) == 0 && isVoo(pessoa->voos_e_reservas[j+1]) == 1) {
                    void* reservaPrimeiro = getReservation(reservas, pessoa->voos_e_reservas[j]);
                    void* vooSegundo = getFlight(flights, pessoa->voos_e_reservas[j+1]);
                    char* resultado = (char*)malloc(sizeof(getReservationBeginDate_Reserv(reservaPrimeiro)) + strlen(" 00:00:00") + 10);
                    adicionarHorasData(getReservationBeginDate_Reserv(reservaPrimeiro), resultado);
                    if (verifica_DuasDatas(resultado, getFlightSchDepa(vooSegundo), 1) == 1) {
                        char* temp = pessoa->voos_e_reservas[j];
                        pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                        pessoa->voos_e_reservas[j+1] = temp;
                    } else {
                        if (datasIguais(resultado, getFlightSchDepa(vooSegundo), 1) == 1) {
                            if(strcmp(getReservationId(reservas, pessoa->voos_e_reservas[j]), getFlightId(vooSegundo)) > 0) {
                                char* temp = pessoa->voos_e_reservas[j];
                                pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                                pessoa->voos_e_reservas[j+1] = temp;
                            }
                        }
                    }
                    free(resultado);
                }
            }
        }
    }
    for (int j = 0; j < total - 1; j++) {
            
                if (isVoo(pessoa->voos_e_reservas[j]) == 1 && isVoo(pessoa->voos_e_reservas[j+1]) == 1) {
                    void* vooPrimeiro = getFlight(flights, pessoa->voos_e_reservas[j]);
                    void* vooSegundo = getFlight(flights, pessoa->voos_e_reservas[j+1]);
                    if (verifica_DuasDatas(getFlightSchDepa(vooPrimeiro), getFlightSchDepa(vooSegundo), 1) == 1) {
                        char* temp = pessoa->voos_e_reservas[j];
                        pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                        pessoa->voos_e_reservas[j+1] = temp;               
                    } else {
                        if (datasIguais(getFlightSchDepa(vooPrimeiro), getFlightSchDepa(vooSegundo), 1) == 1) {
                            if(strcmp(getFlightId(vooPrimeiro), getFlightId(vooSegundo)) >0) {
                                char* temp = pessoa->voos_e_reservas[j];
                                pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                                pessoa->voos_e_reservas[j+1] = temp;  
                            }
                        }
                }
                } else if (isVoo(pessoa->voos_e_reservas[j]) == 1 && isVoo(pessoa->voos_e_reservas[j+1]) == 0) {
                    void* vooPrimeiro = getFlight(flights, pessoa->voos_e_reservas[j]);
                    void* reservaSegundo = getReservation(reservas, pessoa->voos_e_reservas[j+1]);
                    char* resultado = (char*)malloc(sizeof(getReservationBeginDate_Reserv(reservaSegundo)) + strlen(" 00:00:00") + 10);
                    adicionarHorasData(getReservationBeginDate_Reserv(reservaSegundo), resultado);
                    if (verifica_DuasDatas(getFlightSchDepa(vooPrimeiro), resultado, 1) == 1) {
                        char* temp = pessoa->voos_e_reservas[j];
                        pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                        pessoa->voos_e_reservas[j+1] = temp;
                    } else {
                        if (datasIguais(getFlightSchDepa(vooPrimeiro), resultado, 1) == 1) {
                            if(strcmp(getFlightId(vooPrimeiro), getReservationId(reservas, pessoa->voos_e_reservas[j+1])) > 0) {
                                char* temp = pessoa->voos_e_reservas[j];
                                pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                                pessoa->voos_e_reservas[j+1] = temp;
                            }
                        }
                    }
                    free(resultado);
    
                } else if (isVoo(pessoa->voos_e_reservas[j]) == 0 && isVoo(pessoa->voos_e_reservas[j+1]) == 0) {
                    void* reservaPrimeiro = getReservation(reservas, pessoa->voos_e_reservas[j]);
                    void* reservaSegundo = getReservation(reservas, pessoa->voos_e_reservas[j+1]);
                    if (verifica_DuasDatas(getReservationBeginDate_Reserv(reservaPrimeiro), getReservationBeginDate_Reserv(reservaSegundo), 0) == 1) {
                        char* temp = pessoa->voos_e_reservas[j];
                        pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                        pessoa->voos_e_reservas[j+1] = temp;               
                    } else {
                    if (datasIguais(getReservationBeginDate_Reserv(reservaPrimeiro), getReservationBeginDate_Reserv(reservaSegundo), 0) == 1) {
                        if (strcmp(getReservationId(reservas, pessoa->voos_e_reservas[j]), getReservationId(reservas, pessoa->voos_e_reservas[j+1])) > 0) {
                            char* temp = pessoa->voos_e_reservas[j];
                            pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                            pessoa->voos_e_reservas[j+1] = temp;
                        }
                    }
                }
    
                } else if (isVoo(pessoa->voos_e_reservas[j]) == 0 && isVoo(pessoa->voos_e_reservas[j+1]) == 1) {
                    void* reservaPrimeiro = getReservation(reservas, pessoa->voos_e_reservas[j]);
                    void* vooSegundo = getFlight(flights, pessoa->voos_e_reservas[j+1]);
                    char* resultado = (char*)malloc(sizeof(getReservationBeginDate_Reserv(reservaPrimeiro)) + strlen(" 00:00:00") + 10);
                    adicionarHorasData(getReservationBeginDate_Reserv(reservaPrimeiro), resultado);
                    if (verifica_DuasDatas(resultado, getFlightSchDepa(vooSegundo), 1) == 1) {
                        char* temp = pessoa->voos_e_reservas[j];
                        pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                        pessoa->voos_e_reservas[j+1] = temp;
                    } else {
                        if (datasIguais(resultado, getFlightSchDepa(vooSegundo), 1) == 1) {
                            if(strcmp(getReservationId(reservas, pessoa->voos_e_reservas[j]), getFlightId(vooSegundo)) > 0) {
                                char* temp = pessoa->voos_e_reservas[j];
                                pessoa->voos_e_reservas[j] = pessoa->voos_e_reservas[j+1];
                                pessoa->voos_e_reservas[j+1] = temp;
                            }
                        }
                    }
                    free(resultado);
                }
            }
    pessoa->voos_e_reservas_sorted = 1;
}

//verifica se uma string é voo
int isVoo (char* data) { 
    if (isdigit(data[0])) return 1; //é voo
    return 0;
}


void inicializarLocale() {
    setlocale(LC_COLLATE, "en_US.UTF-8"); 
}

//função que compara dois users
int compararUsers(const void* a, const void* b) {
    User* userA = *(User**)a;
    User* userB = *(User**)b;

    inicializarLocale();

    int compararNome = strcoll(userA->name, userB->name);
    if (compararNome != 0) {
        return compararNome;
    }

    return strcoll(userA->id, userB->id);
}

//função que verifica se um utilizador começa com um dado prefixo
int comecaComPrefix(const char* str, const char* prefix) {
    while (*prefix) {

        if (*prefix == '"') {
            prefix++;
            continue;
        }
        
        if (*prefix != *str) {
            return 0;
        }

        prefix++;
        str++;
    }

    while (*str == '"') {
        str++;
    }

    return 1;
}


// -----------------------auxiliar q9-----------------------
void ordenaNomes (void* allUsers) {
    Save_users* users = (Save_users*)allUsers;

    // ordena o array de utilizadores usando a função de comparação
    qsort(users->users_lista, users->tamanho, sizeof(User*), compararUsers);

}



