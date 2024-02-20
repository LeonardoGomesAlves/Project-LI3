#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include <ncurses.h>
#include "../include/voos.h"
#include "../include/reservas.h"
#include "../include/utilizadores.h"
#include "../include/datas.h"
#include "../include/queries.h"
#include "../include/utilidades.h"
#include "../include/output.h"
#include "../include/utilizadores_dados.h"
#include "../include/voos_dados.h"
#include "../include/reservas_dados.h"

//função que trata da escolha da querie que será chamada pelo input que recebemos
void escolhe_querie(int is_Interativo, char* querie, char** flag, int num, void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void *todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats)
{   
    int choose = atoi(querie);
    
    if(choose == 1)  // querie 1
    {
        
        if(strlen(querie) == 1) 
        {
            query1(is_Interativo, flag[0], allFlights, allUsers, allReservations, num, 0);  //sem fields
        }  
        else query1(is_Interativo, flag[0], allFlights, allUsers, allReservations, num, 1); // com fields
        
    }
    if(choose == 2)  // querie 2
    {
        if(flag[1] == NULL) flag[1] = "";

        if(strlen(querie) == 1) 
        {
            query2(is_Interativo, allReservations, allUsers, allFlights, num, flag[0], flag[1], 0);
        }
        else query2(is_Interativo, allReservations, allUsers, allFlights, num, flag[0], flag[1], 1);
      
    }
    if(choose == 3) // querie 3
    {
        if(strlen(querie) == 1)
        {
            query3(is_Interativo, todosHoteis, flag[0], num, 0);
        }
        else query3(is_Interativo, todosHoteis, flag[0], num, 1);
    }
    if(choose == 4) // querie 4
    {   
        if(strlen(querie) == 1)
        {
            query4(is_Interativo, allReservations, todosHoteis, flag[0], num, 0);
        }
        else query4(is_Interativo, allReservations, todosHoteis, flag[0], num, 1);
    }
    if (choose == 5) // querie 5
    {
        if (strlen(querie) == 1)
        {
            query5(is_Interativo, todosAeroportos, flag[0], flag[1], flag[2], flag[3], flag[4], num, 0);
        }
        else query5(is_Interativo, todosAeroportos, flag[0], flag[1], flag[2], flag[3], flag[4], num, 1);
    } 
    if(choose == 6) // querie 6
    {
        if(strlen(querie) == 1)
        {
            query6(is_Interativo, todosAeroportos, flag[0], flag[1], num, 0);
        }
        else query6(is_Interativo, todosAeroportos, flag[0], flag[1], num, 1);
    }
    if(choose == 7) // querie 7
    {
        if(strlen(querie) == 1)
        {
            query7(is_Interativo, todosAeroportos, flag[0], num, 0);
        }
        else query7(is_Interativo,todosAeroportos, flag[0], num, 1);
    }
    if (choose == 8) 
    {
        if(strlen(querie) == 1)
        {
            query8(is_Interativo, todosHoteis, flag[0], flag[1], flag[2], num, 0);
        }
        else query8(is_Interativo, todosHoteis, flag[0], flag[1], flag[2], num, 1);
    }   
    if(choose == 9) // querie 9
    {
        if(strlen(querie) == 1)
        {
            if (flag[1] != NULL) {     // junta as flags numa só string para resolver a querie 9
                char* aux = malloc(sizeof(flag[0]) * 3);
                strcpy(aux, flag[0]);
                strcat(aux, " ");
                strcat(aux, flag[1]);
                if(flag[2] != NULL) {
                    strcat(aux, " ");
                    strcat(aux, flag[2]);
                    query9(is_Interativo, aux, allUsers, num, 0);
                } else {
                    query9(is_Interativo, aux, allUsers, num, 0);
                }
                free(aux);
            }
            else query9(is_Interativo, flag[0], allUsers, num, 0);
        }
        else {
            if (flag[1] != NULL) {
                char* aux = malloc(sizeof(flag[0]) * 3);
                strcpy(aux, flag[0]);
                strcat(aux, " ");
                strcat(aux, flag[1]);
                if(flag[2] != NULL) {
                    strcat(aux, " ");
                    strcat(aux, flag[2]);
                    query9(is_Interativo, aux, allUsers, num, 1);
                } else {
                    query9(is_Interativo, aux, allUsers, num, 1);
                }
                free(aux);
            }
            else query9(is_Interativo, flag[0], allUsers, num, 1);
        } 
    }  
    if (choose == 10) 
    {  
        if(strlen(querie) == 2)
        {
            if(flag == NULL) query10(is_Interativo, userStats, reservStats, passStats, flightStats, NULL, NULL, num, 0);
            else if(flag[1] == NULL) query10(is_Interativo, userStats, reservStats, passStats, flightStats, flag[0], NULL, num, 0);
            else query10(is_Interativo, userStats, reservStats, passStats, flightStats, flag[0], flag[1], num, 0);
        }
        else
        {
            if(flag == NULL) query10(is_Interativo, userStats, reservStats, passStats, flightStats, NULL, NULL, num, 1);
            else if(flag[1] == NULL) query10(is_Interativo, userStats, reservStats, passStats, flightStats, flag[0], NULL, num, 1);
            else query10(is_Interativo, userStats, reservStats, passStats, flightStats, flag[0], flag[1], num, 1);
        }
    }       
}


//------------------------------------- QUERY 1 -------------------------------------
 


//query 1, retorna dados relativos a voos, reservas ou utilizadores
void query1(int is_Interativo, char *id, void *allFlights, void *allUsers, void* allReservations, int i, int type)
{
    char* chave = strdup(id);
    char comparaBook[4];
    for(int k = 0; k < 4; k++) comparaBook[k] = chave[k];

    double custo;
    int idade;

    if(strcmp(comparaBook, "Book") == 0)   // estamos a tratar de um id de uma reservation
    {
        if(procuraReservas(allReservations, chave) == 1)  // verifica se a reseva existe
        {
            void* encontrou2 = getReservation(allReservations, chave);
            custo = calculaCustoPerNoite(encontrou2); 

            print_q1(is_Interativo, id, 0, 1, encontrou2, custo, 0, i, type);
            
        } 
        else print_q1(is_Interativo, id, 0, 0, NULL, 0, 0, i, type);
    }
    else
    {
        if(strspn(chave, "0123456789") == strlen(chave))   // estamos a tratar do id de um flight
        {
            if(procuraVoo(allFlights, chave) == 1) // primeiro verica se o voo existe
            {
                void* encontrou = getFlight(allFlights, chave);

                print_q1(is_Interativo, id, 1, 1, encontrou, 0, 0, i, type);                
            }   
            else print_q1(is_Interativo, id, 1, 0, NULL, 0, 0, i, type);
        }
        else  // estamos a tratar de um id de um user
        {
            if(procuraUser(allUsers, chave) == 1)  // primeiro procura se o user existe
            {
                void* encontrou = getUser(allUsers, chave);

                if (isInactive(getUserStatus(encontrou)) == 1)  // e depois verifica se o user é inátivo
                {
                    free(chave);
                    print_q1(is_Interativo, id, 2, 0, NULL, 0, 0, i, type);
                    return;
                }

                idade = calculaIdades(getUserBirthdate(encontrou));

                print_q1(is_Interativo, id, 2, 1, encontrou, 0, idade, i, type);
            }
            else print_q1(is_Interativo, id, 2, 0, NULL, 0, 0, i, type);
        }
    }

    free(chave);   
}



//------------------------------------- QUERY 2 -------------------------------------

//função da query 2, dá nos as informações de um utilizador organizada por datas relativas às reservas, voos ou ambos
void query2(int is_Interativo, void* allReservations, void* allUsers, void* allFlights, int i, char* tag, char* info, int type)  //info corresponde a ser flights, reservations e se for null é ambos
{
    if (procuraUser(allUsers, tag) == 0) {
    
        print_q2(is_Interativo, 0, NULL, NULL, NULL, 0, i, tag, 2, type);
        return;
    }

    void *user = getUser(allUsers, tag);

    if (isInactive(getUserStatus(user)) == 1) 
    {
        print_q2(0, 0, NULL, NULL, NULL, 0, i, tag, 0, 0);
        return;
    }

    if(strcmp(info, "flights") == 0) 
    {
        if (getUserVoosSorted(user) == 0) organizaVoosUser(allUsers, allFlights, tag);

        int limite = getUserTotalFlights(allUsers, tag);
        print_q2(is_Interativo, 1, user, allReservations, allFlights, limite, i, tag, 0, type);
    }
    else if(strcmp(info, "reservations") == 0) 
    {
        if (getUserReservasSorted(user) == 0) organizaReservasUser(allUsers, allReservations, tag);
        
        int limite = getUserTotalReservations(allUsers, tag);
        print_q2(is_Interativo, 1, user, allReservations, allFlights, limite, i, tag, 1, type);
    }
    else {
        if (getUserReservasSorted(user) == 0) {organizaReservasUser(allUsers, allReservations, tag);}
        if (getUserVoosSorted(user) == 0) {organizaVoosUser(allUsers, allFlights, tag);}
        if (getUserVoosReservasSorted(user) == 0) {organiza_VoosEReservasUser(allUsers, allFlights, allReservations, tag);}

        int limite = getUserTotalFnR(user);
        print_q2(is_Interativo, 1, user, allReservations, allFlights, limite, i, tag, 2, type);
    }
}


//------------------------------------- QUERY 3 -------------------------------------

//função da query3, calcula a média de um dado hotel
void query3 (int is_Interativo, void* todosHoteis, char* tag, int i, int type) 
{
    int k = procuraHotel(todosHoteis, tag);

    if (k == 1) 
    {
        double res = calculaMediaHotel(todosHoteis, tag);

        print_q3(is_Interativo, 1, tag, res, i, type);
    } 
    else print_q3(is_Interativo, 0, tag, 0, i, type);
}


//------------------------------------- QUERY 4 -------------------------------------


//função da query 4, que organiza as reservas de um dado hotel por data
void query4(int is_Interativo, void* allReservations, void* todosHoteis, char* tag, int i, int type) 
{
    int k = procuraHotel(todosHoteis, tag);
    if (k == 1) {
        sortBookingHoteis(allReservations, todosHoteis, tag);
        void* hotel = getHotel(todosHoteis, tag);
        int tamanho = getHotelTamanho(hotel);
        
        print_q4(is_Interativo, allReservations, 1, hotel, tamanho, tag, i, type);
    }
    else print_q4(is_Interativo, allReservations, 0, NULL, 0, tag, i, type);
}

//------------------------------------- QUERIE 5 -------------------------------------

//funcao da query 5, responsavel por listar os voos de origem de um dado aeroporto ordenados por data departida ou identificador do voo
void query5(int is_Interarivo, void* todosAeroportos, char* origin, char* begin_d, char* begin_h, char* end_d, char* end_h, int i, int type) 
{
    // transformação da data de inicio
    char* aux = malloc(strlen(begin_d) + 2);
    strcpy(aux, begin_d);
    strcat(aux, " ");
    char* temp_b = malloc(strlen(aux) + strlen(begin_h) + 1);
    strcpy(temp_b, aux);
    strcat(temp_b, begin_h);

    char* begin = malloc(strlen(temp_b) + 1);
    int index = 0;
    for(int p = 0; temp_b[p] != '\0'; p++) {
        if(temp_b[p] != '"'){
            begin[index] = temp_b[p];
            index++;
        }
    }
    begin[index] = '\0';

    // transformação da data de fim
    char* aux2 = malloc(strlen(end_d) + 2);
    strcpy(aux2, end_d);
    strcat(aux2, " ");
    
    char* temp_e = malloc(strlen(aux2) + strlen(end_h) + 1);
    strcpy(temp_e, aux2);
    strcat(temp_e, end_h);

    char* end = malloc(strlen(temp_e) + 1);
    index = 0;
    for(int p = 0; temp_e[p] != '\0'; p++) {
        if(temp_e[p] != '"'){
            end[index] = temp_e[p];
            index++;
        }
    }
    end[index] = '\0';

    void* aeroporto = encontrarAeroportoArray(todosAeroportos, origin);

    if(aeroporto != NULL)
    {    
        ordenaVooDecrescente(aeroporto);

        int total_voos = getAeroportoTotal(aeroporto);
        
        print_q5(is_Interarivo, 1, aeroporto, origin, total_voos, begin, end, i, type);
    }
    else print_q5(is_Interarivo, 0, NULL, origin, 0, begin, end, i, type);

    free(begin);
    free(aux);
    free(temp_b);
    free(temp_e);
    free(aux2);
    free(end);
}

//------------------------------------- QUERIE 6 -------------------------------------

//função da query 6, responsável por listar o top N de aeroportos com mais passageiros, a funcao ordena os aeroportos por ano e posteriormente basta chamar a funcao print
void query6(int is_Interativo, void* todosAeroportos, char* info, char* top, int i, int type)
{
    int N = atoi(top);
    if (strlen(info) > 4 && is_Interativo == 1) return;
    int ano = atoi(info);

    int tamanho = getAeroportosTamanho(todosAeroportos);
    if(ano == 2023)
    {
        ordenarPassageiros2023(todosAeroportos);
        
        print_q6(is_Interativo, todosAeroportos, tamanho, 2023, N, i, type);
    }
    else if(ano == 2022)
    {
        ordenarPassageiros2022(todosAeroportos);
        
        print_q6(is_Interativo, todosAeroportos, tamanho, 2022, N, i, type);
    }
    else if(ano == 2021)
    {
        ordenarPassageiros2021(todosAeroportos);
        
        print_q6(is_Interativo, todosAeroportos, tamanho, 2021, N, i, type);
    }
}


//------------------------------------- QUERIE 7 -------------------------------------

//função da query 7, lista o top N de aeroportos com a maior mediana de atrasos nos voos
void query7(int is_Interativo, void* todosAeroportos, char* top, int i, int type)
{
    int N = atoi(top);

    ordenarPorMedianaDecrescente(todosAeroportos); 

    int tamanho = getAeroportosTamanho(todosAeroportos);
    
    print_q7(is_Interativo, todosAeroportos, tamanho, N, i, type);
} 

//------------------------------------- QUERIE 8 -------------------------------------
//função da query 8, ordena por data as reservas de um certo hotel e vai verificar quais sao as datas que estao no timespan da query, posteriormente calcula o revenue
void query8(int is_Interativo, void* todosHoteis, char* hotel, char* begin_date1, char* end_date1, int i, int type) 
{
    
    ordenaPorData(todosHoteis, hotel);
    void* currentHotel = getHotel(todosHoteis, hotel);
    int revenue = 0;

    if(currentHotel != NULL)
    {
        int tamanho = getHotelTamanho(currentHotel);
        for (int i = 0; i < tamanho; i++) {
            if ((verify_datas(begin_date1, getHotelEndDate(currentHotel, i)) == 0) || (verify_datas(getHotelBeginDate(currentHotel, i), end_date1) == 0)) {/* fprintf(erros2, "NÃO\n"); */}
            else {
                    int k = gastoPorDatas(getHotelBeginDate(currentHotel, i), getHotelEndDate(currentHotel, i), begin_date1, end_date1, getHotelPricePerNight(currentHotel, i));
                    revenue += k;
                 }
        }

        print_q8(is_Interativo, 1, hotel, revenue, begin_date1, end_date1, i, type);
    }
    else print_q8(is_Interativo, 0, hotel, revenue, begin_date1, end_date1, i, type);
}


//------------------------------------- QUERIE 9 -------------------------------------

//função da query 9, dado um prefixo, lista os nomes dos utilizadores que começam por esse prefixo, por odem alfabética
void query9(int is_Interativo, char* prefix, void* allUsers, int i, int type) 
{
    // retira da string "" se ouver 
    char* aux = malloc(sizeof(prefix));
    int index = 0;
    for (int i = 0; i < strlen(prefix); i++) {
        if (prefix[i] != '"') {
            aux[index] = prefix[i];
            index++;
        }
    }
    aux[index] = '\0';

    int tamanho = getSaveUserTamanho(allUsers);

    print_q9(is_Interativo, prefix, allUsers, tamanho, i, type);

    free(aux);
}

//------------------------------------- QUERIE 10 -------------------------------------
// a query 10 é responsável por mostrar varias metricas do programa, no entanto, apenas é utilizada a funcao print pois
// todos os tipos de dados que precisamos sao feitos no parser do programa
void query10 (int is_Interativo, void* U_stats, void* R_stats, void* P_stats, void* V_stats, char* ano_c, char* mes_c, int i, int type) 
{
    print_q10(is_Interativo, U_stats, R_stats, P_stats, V_stats, ano_c, mes_c, i, type);
}
