#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include <ncurses.h>
#include "../include/parser.h"
#include "../include/datas.h"
#include "../include/utilizadores.h"
#include "../include/reservas.h"
#include "../include/queries.h"
#include "../include/parser_interativo.h"
#include "../include/utilizadores_dados.h"
#include "../include/reservas_dados.h"

//verifica se uma str é maiscula, caso nao for retorna 0
int isMaiuscula(const char *str) {
    while (*str) {
        if (!isupper((unsigned char)*str)) {
            return 0; 
        }
        str++;
    }
    return 1; 
}

//funcao que verifica o input do utilizador no modo interativo, e verifica se é ou não válido, posteriormente é enviado para uma funcao q faz a sua associacao ao tipo de print a fazer no ecra
int verifica_Input(char* query, char** flags, int i)
{
    int choose = atoi(query);
    if(choose < 1 || choose > 10) return 1;

    if(choose == 1)
    {
        if(i != 1) return 2;
    }
    else if(choose == 2)
    {
        if(i == 2)
        {
            if(strcmp(flags[1], "flights") != 0 && strcmp(flags[1], "reservations") != 0) return 3;
        }
        else if(i != 1) return 4;
    }
    else if(choose == 3)
    {
        if(i != 1) return 5;
    }
    else if(choose == 4)
    {
        if(i != 1) return 6;
    }
    else if(choose == 5)
    {
        if(i == 5)
        {
            if(strlen(flags[0]) != 3 || isMaiuscula(flags[0]) != 1) return 8;

            char* aux = strdup(flags[1]); 
            char* temp_b = strcat(aux, " ");
            temp_b = strcat(temp_b, flags[2]);

            char* begin = malloc(strlen(temp_b) + 1); 
            int index = 0;
            for(int p = 0; temp_b[p] != '\0'; p++) {
                if(temp_b[p] != '"'){
                    begin[index] = temp_b[p];
                    index++;
                }
            }
            begin[index] = '\0';

            free(aux);  

            char* aux2 = strdup(flags[3]);  
            char* temp_e = strcat(aux2, " ");
            temp_e = strcat(temp_e, flags[4]);

            char* end = malloc(strlen(temp_e) + 1); 
            index = 0;
            for(int p = 0; temp_e[p] != '\0'; p++) {
                if(temp_e[p] != '"'){
                    end[index] = temp_e[p];
                    index++;
                }
            }
            end[index] = '\0';

            free(aux2);  

            if (verifica_DuasDatas(begin, end, 1) == 0) return 9;

        }
        else return 7;
    }
    else if(choose == 6)
    {
        if(i == 2)
        {
            int ano = atoi(flags[0]);

            if(ano < 2021 || ano > 2023) return 10;
            if(atoi(flags[1]) <= 0) return 12;
        }
        else return 11;
    }
    else if(choose == 7)
    {
        if(i == 1)
        {
            if(atoi(flags[0]) <= 0) return 12;
        }
        else return 13;
    }
    else if(choose == 8)
    {
        if(i == 3)
        {
            if(verifica_DuasDatas(flags[1], flags[2], 0) == 0) return 15;
        }
        else return 14;
    }
    else if(choose == 9)
    {
        if(i != 1) return 16;
    }
    else if(choose == 10)
    {
        if(i == 2)
        {
            int ano = atoi(flags[0]);
            int mes = atoi(flags[1]);

            if(ano < 2010 || ano > 2023) return 17;
            if(mes < 1 || mes > 12) return 19;
        }
        else if(i == 1)
        {
            int ano = atoi(flags[0]);
            if(ano < 2010 || ano > 2023) return 17;
        }
        else if(i != 0) return 18;
    }

    return 0;
}

//funcao responsavel por printar os erros do input do utilizador no menu das queries
void print_erros(WINDOW* input, int x)
{
    wmove(input, 20, 20);
    wclrtoeol(input);
    wrefresh(input);
    mvwprintw(input, 19, 20, "ERRO!");
    if(x == 1) mvwprintw(input, 20, 20, "Número de query inválido, por favor insira um valor entre 1 e 10, inclusive.");
    else if(x == 2) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 1, exprimente '1(F) <ID>'.");
    else if(x == 3) mvwprintw(input, 20, 20, "Segundo argumento inválido, exprimente '2(F) <ID> [flights/reservations]'.");
    else if(x == 4) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 2, exprimente '2(F) <ID> [flights/reservations]'.");
    else if(x == 5) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 3, exprimente '3(F) <ID>'.");
    else if(x == 6) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 4, exprimente '4(F) <ID>'.");
    else if(x == 7) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 5, exprimente '5(F) <Name> <Begin_date> <End_date>'.");
    else if(x == 8) mvwprintw(input, 20, 20, "Por favor insira um nome de aeroporto constítuido por 3 letras maiúsculas.");
    else if(x == 9) mvwprintw(input, 20, 20, "Datas inválidas, experimente usar 'ano/mês/dia horas:minutos:segundos'.");
    else if(x == 10) mvwprintw(input, 20, 20, "Por favor insira um ano entre 2021 e 2023, inclusive.");
    else if(x == 11) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 6, exprimente '6(F) <Year> <N>'.");
    else if(x == 12) mvwprintw(input, 20, 20, "Por favor insira um valor válido para o top.");
    else if(x == 13) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 7, exprimente '7(F) <N>'.");
    else if(x == 14) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 8, exprimente '8(F) <ID> <Begin_date> <End_date>'.");
    else if(x == 15) mvwprintw(input, 20, 20, "Datas inválidas, experimente usar 'ano/mês/dia'.");
    else if(x == 16) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 9, exprimente '9(F) <Prefix>'.");
    else if(x == 17) mvwprintw(input, 20, 20, "Por favor insira um ano entre 2010 e 2023, inclusive.");
    else if(x == 18) mvwprintw(input, 20, 20, "Número inválido de argumentos para executar a query 10, exprimente '10(F) [Year[Month]]'.");
    else if(x == 19) mvwprintw(input, 20, 20, "Por favor insira um mês válido, inclusive.");
}



//funcao responsavel por receber o input do utilizador, e fazer a respetiva associao à query inserida
int parse_query_Interativo(char* input, void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void* todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats) 
{
    int valid = 1;
    if (input != NULL) 
    {
        // foram de voltar atras no monu de input
        if(strcmp(input, "back") == 0) 
        {
            valid = 0;
            return valid;
        }

        char *delimiters = " ";

        char *temp_buff = strdup(input);
        char* temp_buff_copy = temp_buff;
        char *p;
        int i = 0;

        char *query = NULL; 
        char **flags = NULL; // guarda todas as flags num array de strings
        
        while ((p = strsep(&temp_buff_copy, delimiters)) != NULL) 
        {
            if (i == 0) 
            {
                query = strdup(p);
            } 
            else {
                flags = realloc(flags, (i + 1) * sizeof(char*));
                flags[i - 1] = strdup(p);
                flags[i] = NULL;
            }
            i++;
        }

        valid = verifica_Input(query, flags, i-1);
        if(valid == 0) 
        {
            escolhe_querie(1, query, flags, 0, allFlights, allUsers, allReservations, todosHoteis, todosAeroportos, userStats, flightStats, reservStats, passStats);
        }

        for (int j = 0; j < i - 1; j++) {
            free(flags[j]);
        }

        free(temp_buff);
        free(query);
        free(flags);
    }

    return valid;
}
