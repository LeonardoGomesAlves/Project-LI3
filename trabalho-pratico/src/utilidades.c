#include <stdio.h>
#include <stdlib.h>
#include "../include/utilidades.h"
#include "../include/utilizadores.h"
#include "../include/datas.h"
#include "../include/utilizadores_dados.h"
#include "../include/voos_dados.h"

typedef struct mes {
    int dias[31];
    int passDups[31];
    int passDupMes;
    int total;
} Mes;


typedef struct ano {
    Mes** meses;
    int total_ano;
    int total_unico;
} Ano;

typedef struct stats {
    GHashTable* anos;
} Stats;

//inicia o tipo de stats para a query 10 na main
void* initStats () {
    GHashTable* tabela = g_hash_table_new(g_str_hash, g_str_equal);
    return (void*)tabela;
}

//verifica se um certo ano já existe na hashtable das estatisticas
int statEncontrada(char* tag, void* stats) {
    GHashTable* estatisticas = (GHashTable*)stats;
    gpointer encontrado = g_hash_table_lookup(estatisticas, tag);
    
    if (encontrado != NULL) {
        return 1;
    } else {
        return 0;
    }
}


//função que serve so para extrair os dias de uma data com horas
void extrairData(char* dataHoraOriginal, char* dataResultado) 
{
    sscanf(dataHoraOriginal, "%10s", dataResultado);
}

// função que verifica todos os voos de um user para saber quantos voos esse user tem no mesmo dia
int isDuplicated(void* user, void* voos_dados, char* data)
{
    User* u = (User*)user;

    int counter = 0;

    char data_dias[11]; 
    extrairData(data, data_dias);

    int tamanho = getUserNFlights(u);
    for(int i = 0; i < tamanho; i++)
    {
        void* voo = getFlight(voos_dados, getUserFlightArr(u, i));
        char dia_voo[11];
        extrairData(getFlightSchDepa(voo), dia_voo);

        if (strcmp(dia_voo, data_dias) == 0) counter++;
    }

    //printf("%d ", counter);
    return counter;
}

//função que serve so para extrair o mês de uma data com horas
void extrairDataMes(char* dataHoraOriginal, char* dataResultado) 
{
    sscanf(dataHoraOriginal, "%7s", dataResultado);
}

// função que verifica todos os voos de um user para saber quantos voos esse user tem no mesmo mês
int isDuplicatedMes(void* user, void* voos_dados, char* data)
{
    User* u = (User*)user;

    int counter = 0;

    char data_dias[11]; 
    extrairDataMes(data, data_dias);

    int tamanho = getUserNFlights(u);
    for(int i = 0; i < tamanho; i++)
    {
        void* voo = getFlight(voos_dados, getUserFlightArr(u, i));
        char dia_voo[11];
        extrairDataMes(getFlightSchDepa(voo), dia_voo);

        if (strcmp(dia_voo, data_dias) == 0) counter++;
    }

    return counter;
}

//funcao responsavel por adicionar as metricas necessarias para a query 10 no sitio correto
void addStats (char* data, void* stats, char* anoTag, void* user_d, void* voos_dados) 
{
    GHashTable* estatisticas = (GHashTable*)stats;
    int datas[6];
    dataToArray(data, datas);

    int k = statEncontrada(anoTag, stats);
    if (k == 1) 
    {
        Ano* ano = g_hash_table_lookup(estatisticas, anoTag);
        ano->total_ano++;
        ano->meses[(datas[1])-1]->dias[(datas[2])-1]++;

        if (user_d != NULL)
        {    
            if (datas[0] == 2021) {
                if (getUserSeen(user_d, 21) == 0) {
                    setUserSeen(user_d, 21);
                    ano->total_unico++;
                }
            }
            if (datas[0] == 2022) {
                if (getUserSeen(user_d, 22) == 0) {
                    setUserSeen(user_d, 22);
                    ano->total_unico++;
                }
            }
            if (datas[0] == 2023) {
                if (getUserSeen(user_d, 23) == 0) {
                    setUserSeen(user_d, 23);
                    ano->total_unico++;
                }
            }

            int d = isDuplicated(user_d, voos_dados, data);
            int dMes = isDuplicatedMes(user_d, voos_dados, data);
            
            if(d > 1)
            {
                ano->meses[datas[1]-1]->passDups[datas[2]-1]++;
            }

            if(dMes > 1)
            {
                ano->meses[datas[1]-1]->passDupMes++;
            }

        }
        ano->meses[(datas[1])-1]->total++;
    } 
    else 
    {
        Ano* ano = malloc(sizeof(Ano));
        ano->total_unico = 0;
        ano->meses = malloc(sizeof(Mes) * 12);
        for (int i = 0; i < 12; i++) {
            Mes* mes = malloc(sizeof(Mes));
            mes->passDupMes = 0;
            mes->total = 0;
            for (int j = 0; j < 31; j++) {
                mes->dias[j] = 0;
                mes->passDups[j] = 0;
                }
            ano->meses[i] = mes;
        }
        if (user_d != NULL) 
        {
            if (datas[0] == 2021) {
                if (getUserSeen(user_d, 21) == 0) {
                    setUserSeen(user_d, 21);
                    ano->total_unico = 1;
                }
            }
            if (datas[0] == 2022) {
                if (getUserSeen(user_d, 22) == 0) {
                    setUserSeen(user_d, 22);
                    ano->total_unico = 1;
                }
            }
            if (datas[0] == 2023) {
                if (getUserSeen(user_d, 23) == 0) {
                    setUserSeen(user_d, 23);
                    ano->total_unico = 1;
                }
            }
        }
        ano->meses[(datas[1])-1]->total = 1;
        ano->meses[(datas[1])-1]->dias[(datas[2])-1] = 1;
        ano->total_ano = 1;
        g_hash_table_insert(estatisticas, anoTag, ano);
    }
}

//funcoes de get e set

int getTotalMesUnico (void* stats, char* tag, int mes) { //total de passageiros unicos no mes
    GHashTable* estatisticas = (GHashTable*)stats;
    Ano* ano = g_hash_table_lookup(estatisticas, tag);
    
    if(ano == NULL) return 0;
    else 
    {
        int pass = getTotalMes(stats, tag, mes);
    
        return (pass - ano->meses[mes-1]->passDupMes);
    }
}

int getTotalDias(void* stats, char* tag, int mes, int dia)
{
    GHashTable* estatisticas = (GHashTable*)stats;
    Ano* ano = g_hash_table_lookup(estatisticas, tag);

    if(ano == NULL) return 0;
    else return ano->meses[mes-1]->dias[dia-1];
}

int getTotalDiasUnicos (void* stats, char* tag, int mes, int dia) 
{
    GHashTable* estatisticas = (GHashTable*)stats;
    Ano* ano = g_hash_table_lookup(estatisticas, tag);

    if(ano == NULL) return 0;
    else 
    {
        int pass = getTotalDias(stats, tag, mes, dia);

        return (pass - ano->meses[mes-1]->passDups[dia-1]);
    }
} 



int getTotalAno (void* stats, char* tag) {
    GHashTable* estatisticas = (GHashTable*)stats;
    Ano* ano = g_hash_table_lookup(estatisticas, tag);
    if (ano == NULL) return 0;
    return ano->total_ano;
}

int getTotalAnoUnico (void* stats, char* tag) {
    GHashTable* estatisticas = (GHashTable*)stats;
    Ano* ano = g_hash_table_lookup(estatisticas, tag);
    if (ano == NULL) return 0;
    return ano->total_unico;
}

int getTotalMes (void* stats, char* tag, int mes) {
    GHashTable* estatisticas = (GHashTable*)stats;
    Ano* ano = g_hash_table_lookup(estatisticas, tag);
    if (ano == NULL) return 0;
    return ano->meses[mes-1]->total;

}

//funcao que dá free às estatisticas
void free_stats_hash (void* tabela) {
    GHashTable* estatisticas = (GHashTable*)tabela;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, estatisticas);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Ano* ano = (Ano*)value;

        for (int i = 0; i < 12; i++) {
            if (ano->meses[i] != NULL) {
                free(ano->meses[i]);
            }
        }

        free(ano->meses);
        free(ano);
    }

    g_hash_table_destroy(estatisticas);
}