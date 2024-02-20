#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/parser.h"
#include "../include/utilizadores.h"
#include "../include/reservas.h"
#include "../include/queries.h"
#include "../include/utilizadores_dados.h"
#include "../include/reservas_dados.h"

//função que faz o parse dos datasets, utiliza a função verifica para ver se tal linha é valida; utiliza a função organiza para inserir na nossa base de dados
void* parser(FILE *type, void* (verifica)(char**, void*, FILE*, void*), void* (organiza)(void**), void* userDados, char* path, char* init, void* stats)  
{ 
    int tam = 4096;
    int j = 0;
    char linha[1000]; 
    void** array = malloc(sizeof(void*) * tam);
    fgets(linha, sizeof(linha), type);            //ignora a primeira linha do ficheiro

    char *ficheiroErros = malloc(100);
    sprintf(ficheiroErros, "./Resultados/%s", path);
    FILE *resultado = fopen(ficheiroErros, "w");
    fprintf(resultado, "%s\n", init);

    while(fgets(linha, sizeof(linha), type) != NULL) {
        linha[strcspn(linha, "\n")] = 0;

        char* delimiters = ";\n\r";
        
        char* save[14] = {0};   // cria um array com 14 campos, 14 é o maximo de campos que pode cada linha
        char *temp_buff = strdup(linha);
        char* temp_buff_copy = temp_buff;
        char *p;
        int i = 0;
        // vai separando a linha nos ;, enters e nulls
        while ((p = strsep(&temp_buff_copy, delimiters)) != NULL) { 
            save[i] = strdup(p);
            i++;
        }
        free(temp_buff);
        

        if (j * 2 >= tam) {
            array = realloc(array, sizeof(void*)*tam*2);
            tam *= 2;
        }

        array[j] = verifica(save, userDados, resultado, stats); //vê se o malloc é válido e insere os dados numa struct se estes também forem validos;
        j++;
        if (array[j-1] == NULL) {
            j--;
        }

        for (int k = 0; k < i; k++) {
            free(save[k]);
        }
        free(p);
    }

    array[j] = NULL; //caso de paragem

    free(ficheiroErros);
    fclose(resultado);

    return organiza(array);
}



//função de parse dos passageiros, lê o dataset dos passageiros e insere-os nos voos
void parser_passageiros(FILE *type, void verifica(void*, void*, char**, FILE* resultado, void*), void* users, void* voos, void* passStats) 
{
    char linha[1000];
    fgets(linha, sizeof(linha), type);  // ignora a primeira linha

    char *ficheiroErros = malloc(100);
    sprintf(ficheiroErros, "./Resultados/passengers_errors.csv");
    FILE *resultado = fopen(ficheiroErros, "w");
    fprintf(resultado, "flight_id;user_id\n");

    while(fgets(linha, sizeof(linha), type) != NULL) 
    {
        linha[strcspn(linha, "\n")] = 0;

        char* delimiters = ";\n\r";
        
        char* save[2] = {0};
        char *temp_buff = strdup(linha);
        char* temp_buff_copy = temp_buff;
        char *p;
        int i = 0;
        while ((p = strsep(&temp_buff_copy, delimiters)) != NULL) { 
            save[i] = strdup(p);
            i++;
        }
        free(temp_buff);
        
        // verifica se os passageiros são validos e vai adicionando a data a cada struct
        verifica(voos, users, save, resultado, passStats);

        for (int k = 0; k < i; k++) {
            free(save[k]);
        }
        free(p);
    }

    free(ficheiroErros);
    fclose(resultado);
}



//função de parse da query, faz a leitura de cada linha e guarda as suas informações, para depois podermos realizar as queries
void parse_query(FILE* input, void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void* todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats)  // tenho de lhe dar os saves
{
    char linha[1000];
    int percorre = 1;  // variavel pra saber qual é o num do ficheiro que tem de imprimir
    while (fgets(linha, sizeof(linha), input) != NULL) 
    {
        linha[strcspn(linha, "\n")] = 0;

        char *delimiters = " ";

        char *temp_buff = strdup(linha);
        char* temp_buff_copy = temp_buff;
        char *p;
        int i = 0;

        char *querie = NULL; 
        char **flags = NULL; // guarda todas as flags num array de strings
        
        while ((p = strsep(&temp_buff_copy, delimiters)) != NULL) 
        {
            if (i == 0) 
            {
                querie = strdup(p);
            } 
            else {
                flags = realloc(flags, (i + 1) * sizeof(char*));
                flags[i - 1] = strdup(p);
                flags[i] = NULL;
            }
            i++;
        }
        //printf("%d\n", j++);
        escolhe_querie(0, querie, flags, percorre, allFlights, allUsers, allReservations, todosHoteis, todosAeroportos, userStats, flightStats, reservStats, passStats);

        for (int j = 0; j < i - 1; j++) {
            free(flags[j]);
        }

        percorre++;
        free(temp_buff);
        free(querie);
        free(flags);

    }
}
