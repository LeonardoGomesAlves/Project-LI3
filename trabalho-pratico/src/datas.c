#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>

#define DATA_ATUAL 2023.836     // 2023/10/01


// transforma as datas de strings num array
void dataToArray(char* str, int data[])
{
    int index = 0;
    if (str == NULL) return;
    char* aux = strdup(str);
    char* token = strtok(aux, "/ :");
    
    while (token != NULL && index < 6) {
        if (isdigit(*token)) {
            data[index++] = atoi(token);
        }
        token = strtok(NULL, "/ :");
    }
    free(aux);
}

//recebe uma data em formato de string e dá o ano
char* dataGetAno(char* str) {
    char* aux = strdup(str);
    for (int i = 0; i < 4; i++) {
        str[i] = aux[i];
    }
    str[4] = '\0';
    free(aux);
    return str;
}


int verifica_Data(char* data)  // verifica se as datas normais são validas
{
    char *aux = strdup(data);

    if (strlen(aux) == 10 && aux[4] == '/' && aux[7] == '/')
    {
        char* token;
        int i = 0;
        char* tokens[3];

        // separa os valores pelos /
        token = strtok(aux, "/");
        while (token != NULL && i < 3)
        {
            tokens[i++] = token;
            token = strtok(NULL, "/");
        }

        if (i == 3)
        {
            int ano = atoi(tokens[0]);
            int mes = atoi(tokens[1]);
            int dia = atoi(tokens[2]);

            if (dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12 && ano >= 1000 && ano <= 9999)
            {
                if(strspn(tokens[0], "0123456789") == strlen(tokens[0]) &&   // verifica se há caracteres não numericos
                   strspn(tokens[1], "0123456789") == strlen(tokens[1]) &&
                   strspn(tokens[2], "0123456789") == strlen(tokens[2])){
                    free(aux);
                    return 1; // Data válida
                }
            }  
        }
    }

    free(aux);
    return 0; // Data inválida
}


// função que verifica se uma string é composta só por digitos
int isDigitString (char* string) {
    if (!string) return 0;
    for (int i = 0; i < strlen(string); i++) {
        if (!isdigit(string[i])) return 0;
    }
    return 1;
}


// função que verifica se uma string é vazia 
int isEmpty(char* str) {
    if(strlen(str) <= 0) return 1;
    else return 0;
}

//compara se a data1 é menor que a data2, caso for, retorna -1 senao retorna 1 ou 0 se forem iguais
int comparaDatas(int data1[3], int data2[3]) {

    if (data1[0] < data2[0]) {
        return -1;
    } else if (data1[0] > data2[0]) {
        return 1;
    }

    if (data1[1] < data2[1]) {
        return -1;
    } else if (data1[1] > data2[1]) {
        return 1;
    }

    if (data1[2] < data2[2]) {
        return -1;
    } else if (data1[2] > data2[2]) {
        return 1;
    }

    return 0;
}

//calcula o preço gasto de um certo booking de um certo hotel
int gastoPorDatas (char* datainicio_Book, char* datafim_Book, char* inicio_d, char* fim_d, int per_night) {
    int inicio_book[3];
    int fim_book[3];
    int inicio[3];
    int fim[3];
    dataToArray(datainicio_Book, inicio_book);
    dataToArray(datafim_Book, fim_book);
    dataToArray(inicio_d, inicio);
    dataToArray(fim_d, fim);

    int total = 0;

    for (int i = inicio_book[2]; i <= fim_book[2]; i++) {
        int k = comparaDatas(inicio_book, fim);
        if (k <= 0 && comparaDatas(inicio_book, inicio) >= 0) {if (inicio_book[2] != fim_book[2])total++;
        }
        inicio_book[2]++;
    }

    return total * per_night;
}

int verifica_Horas(char* data)  // função que verifica se uma data com horas é válida
{
    if (data == NULL) return 0;
    char *aux = strdup(data);
    if (aux == NULL) return 0;

    char* tokenAux;
    int i = 0;
    char* partes[2];
    // divide a data em dois, uma parte só com as horas e o resto com a data do mês
    tokenAux = strtok(aux, " ");
    while (tokenAux != NULL && i < 2){
        partes[i] = tokenAux;
        tokenAux = strtok(NULL, " ");
        i++;
    }

    if(verifica_Data(partes[0]) == 1)
    {
        char *aux2 = strdup(partes[1]);

        if (aux2 != NULL && strlen(aux2) == 8 && aux2[2] == ':' && aux2[5] == ':')
        {
            char* token;
            int i = 0;
            char* tokens[3];
            // separa os campos pelos :
            token = strtok(aux2, ":");
            while (token != NULL && i < 3)
            {
                tokens[i] = token;
                token = strtok(NULL, ":");
                i++;
            }

            for (int k = 0; k < 3; k++) {
                if (isDigitString(tokens[k]) == 0) {free(aux2); free(aux); return 0;}
            }
            
                int horas = atoi(tokens[0]);
                int minutos = atoi(tokens[1]);
                int segundos = atoi(tokens[2]);

                if (horas >= 0 && horas <= 23 && minutos >= 0 && minutos <= 59 && segundos >= 0 && segundos <= 59){
                    if(strspn(tokens[0], "0123456789") == strlen(tokens[0]) &&   // verifica se há caracteres não numericos
                        strspn(tokens[1], "0123456789") == strlen(tokens[1]) &&
                        strspn(tokens[2], "0123456789") == strlen(tokens[2])){
                            free(aux);
                            free(aux2);
                            return 1; // Data válida
                        }
                }            
        }
        
        free(aux2); 
    }

    free(aux);
    return 0;
}


// função que serve para retirar as horas do campo de uma data
void retirarHorasData (char* data, char* resultado) { 
    char* temp = strdup(data);
    char* token = strtok(temp, " ");
    strcpy(resultado, token);
    free(temp);
}

//verifica se a data do inicio é posterior à data do fim, caso seja, retorna 0, senao 2 se forem iguais e -1 se a do fim for superior, é utilizada na funcao de calcular o revenue dos hoteis
int verify_datas(char* begin_date, char* end_date) {
        if(verifica_Data(begin_date) == 0 || verifica_Data(end_date) == 0) return 0;

        int inicio[3];
        int fim[3];

        dataToArray(begin_date, inicio);
        dataToArray(end_date, fim);

        if(inicio[0] > fim[0]) return 0;
        if(inicio[0] == fim[0] && inicio[1] > fim[1]) return 0;
        if(inicio[0] == fim[0] && inicio[1] == fim[1] && inicio[2] > fim[2]) return 0;
        if(inicio[0] == fim[0] && inicio[1] == fim[1] && inicio[2] == fim[2]) return 2;
        
        return -1;
}


// função que verifica se a data do fim ocorre depois da data do início
int verifica_DuasDatas(char* begin_date, char* end_date, int type) // o type serve pra saber se estamos a trabalhar com horas na data ou não
{
    if (isEmpty(begin_date) == 1 || isEmpty(end_date) == 1) return 0;
    if(type == 0) // datas so com o dia
    {
        if(verifica_Data(begin_date) == 0 || verifica_Data(end_date) == 0) return 0;

        int inicio[3];
        int fim[3];

        dataToArray(begin_date, inicio);
        dataToArray(end_date, fim);

        if(inicio[0] > fim[0]) return 0;
        if(inicio[0] == fim[0] && inicio[1] > fim[1]) return 0;
        if(inicio[0] == fim[0] && inicio[1] == fim[1] && inicio[2] > fim[2]) return 0;
    }
    if (type == 1) // datas com dia e horas
    {
        if(verifica_Horas(begin_date) == 0 || verifica_Horas(end_date) == 0) return 0;

        int iniHoras[6];
        int fimHoras[6];

        dataToArray(begin_date, iniHoras);
        dataToArray(end_date, fimHoras);

        if (iniHoras[0] > fimHoras[0]) return 0;
        if (iniHoras[0] == fimHoras[0] && iniHoras[1] > fimHoras[1]) return 0;
        if (iniHoras[0] == fimHoras[0] && iniHoras[1] == fimHoras[1] && iniHoras[2] > fimHoras[2]) return 0;
        if (iniHoras[0] == fimHoras[0] && iniHoras[1] == fimHoras[1] && iniHoras[2] == fimHoras[2] && iniHoras[3] > fimHoras[3]) return 0;
        if (iniHoras[0] == fimHoras[0] && iniHoras[1] == fimHoras[1] && iniHoras[2] == fimHoras[2] && iniHoras[3] == fimHoras[3] && iniHoras[4] > fimHoras[4]) return 0;
        if (iniHoras[0] == fimHoras[0] && iniHoras[1] == fimHoras[1] && iniHoras[2] == fimHoras[2] && iniHoras[3] == fimHoras[3] && iniHoras[4] == fimHoras[4] && iniHoras[5] > fimHoras[5]) return 0;
    }
    if (type == 3) {
        int inicio[3];
        char* resultado = (char*)malloc(sizeof(end_date));
        retirarHorasData(end_date, resultado);
        int fim[3];
        dataToArray(resultado, fim);
        dataToArray(begin_date, inicio);
        
        if(inicio[0] > fim[0]) {free(resultado); return 0;}
        if(inicio[0] == fim[0] && inicio[1] > fim[1]) {free(resultado); return 0;}
        if(inicio[0] == fim[0] && inicio[1] == fim[1] && inicio[2] > fim[2]) {free(resultado); return 0;}

        free(resultado);
    }

    return 1;
}


// função que testa se duas datas são iguais
int datasIguais(char* begin_date, char* end_date, int type) { // 1 - são iguais
    if (type == 0) {    
        int inicio[3];
        int fim[3];

        dataToArray(begin_date, inicio);
        dataToArray(end_date, fim);

        if (inicio[0] == fim[0] && inicio[1] == fim[1] && inicio[2] == fim[2]) return 1;
    } else {
        int iniHoras[6];
        int fimHoras[6];

        dataToArray(begin_date, iniHoras);
        dataToArray(end_date, fimHoras);

        if (iniHoras[0] == fimHoras[0] && iniHoras[1] == fimHoras[1] && iniHoras[2] == fimHoras[2]) return 1;
    }
    return 0;
}


// função que adiciona as horas a uma data
void adicionarHorasData (char* data, char* resultado) { 

    if (resultado != NULL) {
        strcpy(resultado, data);
        strcat(resultado, " 00:00:00");
    }
}



// função que calcula a idade de um utilizador
int calculaIdades(char* str)
{
    int data[3];
    dataToArray(str, data);

    double total_anos = data[0] + (data[1] / 12.0) + (data[2] / 365.0);

    double idade = DATA_ATUAL - total_anos;

    return idade;
}           


// função que calcula quantas noites durou uma reserva
int calculaNoites(char* begin_date, char* end_date)
{
    int inicio[3];
    int fim[3];
    dataToArray(begin_date, inicio);
    dataToArray(end_date, fim);

    int noites = fim[2] - inicio[2];

    return noites;
}


// função que calcula o tempo de delay no levantamento de um voo
int calculaDelays(char* schedule_date, char* real_date)
{
    int schedule[6];
    int real[6];
    dataToArray(schedule_date, schedule);
    dataToArray(real_date, real);

    int delay = (real[3] - schedule[3]) * 3600 + (real[4] - schedule[4]) * 60 + (real[5] - schedule[5]);  

    return delay;
}

//caso a data tiver apenas dias, acrescenta o campo das horas, sendo o default a meia noite
void preencheData(char* data, char* resultado) {
    strcpy(resultado, data);
    strcat(resultado, " 00:00:00");
}