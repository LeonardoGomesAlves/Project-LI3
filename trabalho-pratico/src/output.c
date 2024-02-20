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



//MODULO RESPONSAVEL POR FAZER O PRINT DAS QUERIES, TANTO PARA O MODO INTERATIVO COMO PARA O MODO BATCH. É RESPONSAVEL TAMBEM POR FAZER AS VERIFICAÇÕES IMPOSTAS PELAS QUERIES

/* ------------------------------------- QUERY 1 ----------------------------------------------- */

// função auxiliar que serve só para colocar em maiúscula a primeira letra do includes_freakfast  
void statusToUpper (char* str, char* resultado) {
    resultado = strcpy(resultado, str);
    if (resultado[0] == 'f') resultado[0] = 'F';
    else (resultado[0] = 'T');   
}

void print_q1(int is_Interativo, char *id, int identificador, int encontrou, void* info, double custo, int idade, int i, int type)
{
    if(is_Interativo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", i);
        FILE *resultado = fopen(file_name, "w");

        if(encontrou == 1)
        {
            if(identificador == 0)  // reserva
            {
                char* pequenoalmoco = malloc(sizeof(getBreakfast_Reserv(info)));
                char* breakfast = getBreakfast_Reserv(info);

                if (strlen(breakfast) != 0) {statusToUpper(breakfast, pequenoalmoco);}
                if (strlen(pequenoalmoco) == 1) {
                    char* temp = "True";
                    pequenoalmoco = realloc(pequenoalmoco, sizeof(temp));
                    pequenoalmoco = strdup(temp);
                }
                if (strlen(breakfast) == 0) {
                    char* temp = "False";
                    pequenoalmoco = realloc(pequenoalmoco, sizeof(temp));
                    pequenoalmoco = strdup(temp);
                }

                if(type == 0) {
                    fprintf(resultado, "%s;%s;%d;%s;%s;%s;%d;%.3f\n", getReservationHotelId_Reserv(info), getReservationHotelName_Reserv(info), getReservationHotelStars_Reserv(info), getReservationBeginDate_Reserv(info), getReservationEndDate_Reserv(info), pequenoalmoco, getNnoites_Reserv(info), custo);        
                }
                else if(type == 1)
                {
                    fprintf(resultado, "--- 1 ---\n");
                    fprintf(resultado, "hotel_id: %s\n", getReservationHotelId_Reserv(info));
                    fprintf(resultado, "hotel_name: %s\n", getReservationHotelName_Reserv(info));
                    fprintf(resultado, "hotel_stars: %d\n", getReservationHotelStars_Reserv(info));
                    fprintf(resultado, "begin_date: %s\n", getReservationBeginDate_Reserv(info));
                    fprintf(resultado, "end_date: %s\n", getReservationEndDate_Reserv(info));
                    fprintf(resultado, "includes_breakfast: %s\n", pequenoalmoco);
                    fprintf(resultado, "nights: %d\n", getNnoites_Reserv(info));
                    fprintf(resultado, "total_price: %.3f\n", custo);
                }

                free(pequenoalmoco);
            }
            else if(identificador == 1)  // voo
            {
                if(type == 0) {
                    fprintf(resultado, "%s;%s;%s;%s;%s;%s;%d;%d\n", getFlightAirline(info), getFlightPlaneModel(info), getFlightOrigin(info), getFlightDestination(info), getFlightSchDepa(info), getFlightSchArr(info), getFlightPassengers(info), getFlightDelay(info));
                }
                else if(type == 1)
                {
                    fprintf(resultado, "--- 1 ---\n");
                    fprintf(resultado, "airline: %s\n", getFlightAirline(info));
                    fprintf(resultado, "plane_model: %s\n", getFlightPlaneModel(info));
                    fprintf(resultado, "origin: %s\n", getFlightOrigin(info));
                    fprintf(resultado, "destination: %s\n", getFlightDestination(info));
                    fprintf(resultado, "schedule_departure_date: %s\n", getFlightSchDepa(info));
                    fprintf(resultado, "schedule_arrival_date: %s\n", getFlightSchArr(info));
                    fprintf(resultado, "passengers: %d\n", getFlightPassengers(info));
                    fprintf(resultado, "delay: %d\n", getFlightDelay(info));
                }
            }
            else if(identificador == 2)  // user
            {
                if(type == 0) {
                    fprintf(resultado, "%s;%s;%d;%s;%s;%d;%d;%.3f\n", getUserName(info), getUserSex(info), idade, getUserCountryC(info), getUserPassport(info), getUserNFlights(info), getUserNReser(info), getUserGastoReserv(info));
                }
                else if(type == 1)
                {
                    fprintf(resultado, "--- 1 ---\n");
                    fprintf(resultado, "name: %s\n", getUserName(info));
                    fprintf(resultado, "sex: %s\n", getUserSex(info));
                    fprintf(resultado, "age: %d\n", idade);
                    fprintf(resultado, "country_code: %s\n", getUserCountryC(info));
                    fprintf(resultado, "passport: %s\n", getUserPassport(info));
                    fprintf(resultado, "number_of_flights: %d\n", getUserNFlights(info));
                    fprintf(resultado, "number_of_reservations: %d\n", getUserNReser(info));
                    fprintf(resultado, "total_spent: %.3f\n", getUserGastoReserv(info));
                }
            }
        }

        free(file_name);
        fclose(resultado);
    } 
    else 
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        mvwprintw(query, 4, 10, "1 %s", id);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        wrefresh(query);

        mvwprintw(query, 34, 90, "Página 1 de 1");
        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");

        wrefresh(query);

        if(encontrou == 1)
        {
            if(identificador == 0)  // reserva
            {
                char* pequenoalmoco = malloc(sizeof(getBreakfast_Reserv(info)));
                char* breakfast = getBreakfast_Reserv(info);

                if (strlen(breakfast) != 0) {statusToUpper(breakfast, pequenoalmoco);}
                if (strlen(pequenoalmoco) == 1) {
                    char* temp = "True";
                    pequenoalmoco = realloc(pequenoalmoco, sizeof(temp));
                    pequenoalmoco = strdup(temp);
                }
                if (strlen(breakfast) == 0) {
                    char* temp = "False";
                    pequenoalmoco = realloc(pequenoalmoco, sizeof(temp));
                    pequenoalmoco = strdup(temp);
                }

                if(type == 0) {
                    mvwprintw(resultado, 1, 2, "%s;%s;%d;%s;%s;%s;%d;%.3f", getReservationHotelId_Reserv(info), getReservationHotelName_Reserv(info), getReservationHotelStars_Reserv(info), getReservationBeginDate_Reserv(info), getReservationEndDate_Reserv(info), pequenoalmoco, getNnoites_Reserv(info), custo);      
                }
                else if(type == 1)
                {
                    mvwprintw(resultado, 1, 2, "--- 1 ---");
                    mvwprintw(resultado, 2, 2, "hotel_id: %s", getReservationHotelId_Reserv(info));
                    mvwprintw(resultado, 3, 2, "hotel_name: %s", getReservationHotelName_Reserv(info));
                    mvwprintw(resultado, 4, 2, "hotel_stars: %d", getReservationHotelStars_Reserv(info));
                    mvwprintw(resultado, 5, 2, "begin_date: %s", getReservationBeginDate_Reserv(info));
                    mvwprintw(resultado, 6, 2, "end_date: %s", getReservationEndDate_Reserv(info));
                    mvwprintw(resultado, 7, 2, "includes_breakfast: %s", pequenoalmoco);
                    mvwprintw(resultado, 8, 2, "nights: %d", getNnoites_Reserv(info));
                    mvwprintw(resultado, 9, 2, "total_price: %.3f", custo);
                }
            }
            else if(identificador == 1)  // voo
            {
                if(type == 0) {
                    mvwprintw(resultado, 1, 2, "%s;%s;%s;%s;%s;%s;%d;%d", getFlightAirline(info), getFlightPlaneModel(info), getFlightOrigin(info), getFlightDestination(info), getFlightSchDepa(info), getFlightSchArr(info), getFlightPassengers(info), getFlightDelay(info));
                }
                else if(type == 1)
                {
                    mvwprintw(resultado, 1, 2, "--- 1 ---");
                    mvwprintw(resultado, 2, 2, "airline: %s", getFlightAirline(info));
                    mvwprintw(resultado, 3, 2,"plane_model: %s", getFlightPlaneModel(info));
                    mvwprintw(resultado, 4, 2,"origin: %s", getFlightOrigin(info));
                    mvwprintw(resultado, 5, 2,"destination: %s", getFlightDestination(info));
                    mvwprintw(resultado, 6, 2,"schedule_departure_date: %s", getFlightSchDepa(info));
                    mvwprintw(resultado, 7, 2,"schedule_arrival_date: %s", getFlightSchArr(info));
                    mvwprintw(resultado, 8, 2,"passengers: %d", getFlightPassengers(info));
                    mvwprintw(resultado, 9, 2,"delay: %d", getFlightDelay(info));
                }
            }
            else if(identificador == 2)  // user
            {
                if(type == 0) {
                    mvwprintw(resultado, 1, 2, "%s;%s;%d;%s;%s;%d;%d;%.3f", getUserName(info), getUserSex(info), idade, getUserCountryC(info), getUserPassport(info), getUserNFlights(info), getUserNReser(info), getUserGastoReserv(info));
                }
                else if(type == 1)
                {
                    mvwprintw(resultado, 1, 2, "--- 1 ---");
                    mvwprintw(resultado, 2, 2, "name: %s", getUserName(info));
                    mvwprintw(resultado, 3, 2, "sex: %s", getUserSex(info));
                    mvwprintw(resultado, 4, 2, "age: %d", idade);
                    mvwprintw(resultado, 5, 2, "country_code: %s", getUserCountryC(info));
                    mvwprintw(resultado, 6, 2, "passport: %s", getUserPassport(info));
                    mvwprintw(resultado, 7, 2, "number_of_flights: %d", getUserNFlights(info));
                    mvwprintw(resultado, 8, 2, "number_of_reservations: %d", getUserNReser(info));
                    mvwprintw(resultado, 9, 2, "total_spent: %.3f", getUserGastoReserv(info));
                }
            }
        }

        wrefresh(resultado);
        int key;
        do{
            key = wgetch(resultado);
        }
        while(key != 'q');
        endwin();
    }
}





/* ------------------------------------- QUERY 2 ----------------------------------------------- */

void print_q2(int is_Interativo, int encontrou, void* user, void* allReservations, void* allFlights, int N ,int i, char* tag, int identificador, int type)
{
    if(is_Interativo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", i);
        FILE *resultado = fopen(file_name, "w");

        if(encontrou == 1)
        {
            if(identificador == 0) // flight
            {
                for (int i = 0; i < N; i++)
                {  
                    char* voo = getUserFlightArr(user, i);
                    void* flight = getFlight(allFlights, voo);
                    char* dep = getFlightSchDepa(flight);
                    char* data = (char*)malloc(sizeof(dep) + 5);
                    retirarHorasData(dep, data);

                    if(type == 0)       
                    {
                        fprintf(resultado,"%s;%s\n", voo, data);
                    }
                    else
                    {
                        fprintf(resultado, "--- %d ---\n", i+1);
                        fprintf(resultado, "id: %s\n", voo);
                        fprintf(resultado, "date: %s\n", data);
                        if (i != N - 1) {fprintf(resultado, "\n");}
                    }

                    free(data);
                }   
            }
            else if(identificador == 1) // reservation
            {
                for (int i = 0; i < N; i++) 
                {
                    char* book = getUserReservArr(user, i);
                    void* reserva = getReservation(allReservations, book);

                    if(type == 0)    
                    {
                        fprintf(resultado,"%s;%s\n", book, getReservationBeginDate_Reserv(reserva));       
                    }
                    else
                    {
                        fprintf(resultado, "--- %d ---\n", i+1);
                        fprintf(resultado, "id: %s\n", book);
                        fprintf(resultado, "date: %s\n", getReservationBeginDate_Reserv(reserva));
                        if (i != N - 1) {fprintf(resultado, "\n");}
                    }
                }       
            }
            else if(identificador == 2) // flight and reservations
            {
                for(int i = 0; i < N; i++) 
                {
                    if (isVoo(getUserFnRArr(user, i)) == 1) 
                    {
                        void* flight = getFlight(allFlights, getUserFnRArr(user, i));
                        char* data = (char*)malloc(sizeof(getFlightSchDepa(flight)) + 5);
                        retirarHorasData(getFlightSchDepa(flight), data);

                        if(type == 0)    
                        {
                            fprintf(resultado, "%s;%s;flight\n", getUserFnRArr(user, i), data);
                        }
                        else
                        {
                            fprintf(resultado, "--- %d ---\n", i+1);
                            fprintf(resultado, "id: %s\n", getUserFnRArr(user, i));
                            fprintf(resultado, "date: %s\n", data);
                            fprintf(resultado, "type: flight\n");
                            if (i != N - 1) {fprintf(resultado, "\n");}
                        }
                        free(data);
                    }
                    else 
                    {
                        void* reserva = getReservation(allReservations, getUserFnRArr(user, i));

                        if(type == 0)    
                        {
                            fprintf(resultado,"%s;%s;reservation\n", getUserFnRArr(user, i), getReservationBeginDate_Reserv(reserva));      
                        }
                        else if(type == 1)
                        {
                            fprintf(resultado, "--- %d ---\n", i+1);
                            fprintf(resultado, "id: %s\n", getUserFnRArr(user, i));
                            fprintf(resultado, "date: %s\n", getReservationBeginDate_Reserv(reserva));
                            fprintf(resultado, "type: reservation\n");
                            if (i != N - 1) {fprintf(resultado, "\n");}
                        }
                    }
                }
            }
        }

        free(file_name);
        fclose(resultado);
    }
    else
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        if(identificador == 0) mvwprintw(query, 4, 10, "2 %s flights", tag);
        else if(identificador == 1) mvwprintw(query, 4, 10, "2 %s reservations", tag);
        else mvwprintw(query, 4, 10, "2 %s", tag);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        keypad(resultado, TRUE);
        wrefresh(query);

        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
        
        int n_paginas = 0;
        if(type == 0)
        {
            n_paginas = N / 25;
            n_paginas++;
        }
        else if(type == 1)
        {
            if(identificador == 2)
            {
                n_paginas = N / 5;
                if (N % 5 != 0) n_paginas++;
            }
            else
            {
                n_paginas = N / 6;
                if (N % 6 != 0) n_paginas++;
            }
        }

        int startY = 1;

        if(encontrou == 1)
        {
            if(identificador == 0) // flight
            {
                int key = 0;
                int pagina = 0;
                while(key != 'q')
                {
                    int inicial;
                    int add;
                    if (type == 0) {
                        inicial = pagina * 25;
                        add = 25;
                    } else {
                        inicial = pagina * 6;
                        add = 6;
                    }

                    wclear(resultado);  // Limpa a janela resultado
                    box(resultado, 0, 0);

                    startY = 1;
                    for (int i = inicial; i < N && i < inicial + add; i++)
                    {  
                        char* voo = getUserFlightArr(user, i);
                        void* flight = getFlight(allFlights, voo);
                        char* dep = getFlightSchDepa(flight);
                        char* data = (char*)malloc(sizeof(dep) + 5);
                        retirarHorasData(dep, data);

                        if(type == 0)       
                        {
                            mvwprintw(resultado, startY++, 2, "%s;%s", voo, data);
                        }
                        else
                        {
                            mvwprintw(resultado, startY++, 2, "--- %d ---", i+1);
                            mvwprintw(resultado, startY++, 2, "id: %s", voo);
                            mvwprintw(resultado, startY++, 2, "date: %s", data);
                            startY++;
                        }

                        free(data);
                    }   

                    mvwprintw(query, 34, 90, "                   ");
                    mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, n_paginas);
                
                    wrefresh(query);
                    wrefresh(resultado);

                    key = wgetch(resultado);
                    if ((key == KEY_LEFT) && pagina > 0) {
                        pagina--;
                        wrefresh(resultado);
                    }
                    else if ((key == KEY_RIGHT) && pagina < n_paginas - 1) {
                        pagina++;
                        wrefresh(resultado);
                    }
                }
            }
            else if(identificador == 1) // reservas
            {
                int key = 0;
                int pagina = 0;
                while(key != 'q')
                {
                    int inicial;
                    int add;
                    if (type == 0) {
                        inicial = pagina * 25;
                        add = 25;
                    } else {
                        inicial = pagina * 6;
                        add = 6;
                    }

                    wclear(resultado);  // Limpa a janela resultado
                    box(resultado, 0, 0);

                    startY = 1;
                    for (int i = inicial; i < N && i < inicial + add; i++)
                    {  
                        char* book = getUserReservArr(user, i);
                        void* reserva = getReservation(allReservations, book);

                        if(type == 0)    
                        {
                            mvwprintw(resultado, startY++, 2, "%s;%s", book, getReservationBeginDate_Reserv(reserva));       
                        }
                        else
                        {
                            mvwprintw(resultado, startY++, 2,  "--- %d ---", i+1);
                            mvwprintw(resultado, startY++, 2,  "id: %s", book);
                            mvwprintw(resultado, startY++, 2,  "date: %s", getReservationBeginDate_Reserv(reserva));
                            startY++;
                        }
                    }   

                    mvwprintw(query, 34, 90, "                   ");
                    mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, n_paginas);
                
                    wrefresh(query);
                    wrefresh(resultado);

                    key = wgetch(resultado);
                    if ((key == KEY_LEFT) && pagina > 0) {
                        pagina--;
                        wrefresh(resultado);
                    }
                    else if ((key == KEY_RIGHT) && pagina < n_paginas - 1) {
                        pagina++;
                        wrefresh(resultado);
                    }
                }
            }
            else if(identificador == 2)
            {
                int key = 0;
                int pagina = 0;
                while(key != 'q')
                {
                    int inicial;
                    int add;
                    if (type == 0) {
                        inicial = pagina * 25;
                        add = 25;
                    } else {
                        inicial = pagina * 5;
                        add = 5;
                    }

                    wclear(resultado);  // Limpa a janela resultado
                    box(resultado, 0, 0);

                    startY = 1;
                    for (int i = inicial; i < N && i < inicial + add; i++)
                    {  
                        if (isVoo(getUserFnRArr(user, i)) == 1) 
                        {
                            void* flight = getFlight(allFlights, getUserFnRArr(user, i));
                            char* data = (char*)malloc(sizeof(getFlightSchDepa(flight)) + 5);
                            retirarHorasData(getFlightSchDepa(flight), data);

                            if(type == 0)    
                            {
                                mvwprintw(resultado, startY++, 2, "%s;%s;flight", getUserFnRArr(user, i), data);
                            }
                            else
                            {
                                mvwprintw(resultado, startY++, 2, "--- %d ---", i+1);
                                mvwprintw(resultado, startY++, 2, "id: %s", getUserFnRArr(user, i));
                                mvwprintw(resultado, startY++, 2, "date: %s", data);
                                mvwprintw(resultado, startY++, 2, "type: flight");
                                if (i != N - 1) startY++;
                            }
                            free(data);
                        }
                        else 
                        {
                            void* reserva = getReservation(allReservations, getUserFnRArr(user, i));

                            if(type == 0)    
                            {
                                mvwprintw(resultado, startY++, 2,"%s;%s;reservation", getUserFnRArr(user, i), getReservationBeginDate_Reserv(reserva));      
                            }
                            else if(type == 1)
                            {
                                mvwprintw(resultado, startY++, 2, "--- %d ---", i+1);
                                mvwprintw(resultado, startY++, 2, "id: %s", getUserFnRArr(user, i));
                                mvwprintw(resultado, startY++, 2, "date: %s", getReservationBeginDate_Reserv(reserva));
                                mvwprintw(resultado, startY++, 2, "type: reservation");
                                startY++;
                            }
                        }
                    }   

                    mvwprintw(query, 34, 90, "                   ");
                    mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, n_paginas);
                
                    wrefresh(query);
                    wrefresh(resultado);

                    key = wgetch(resultado);
                    if ((key == KEY_LEFT) && pagina > 0) {
                        pagina--;
                        wrefresh(resultado);
                    }
                    else if ((key == KEY_RIGHT) && pagina < n_paginas - 1) {
                        pagina++;
                        wrefresh(resultado);
                    }
                }
            }
        }
        else
        {
            mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
            mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
            wrefresh(query);
            int key;
            do{
                key = wgetch(resultado);
            }
            while(key != 'q');
        }

        endwin(); 
    }
}





/* ------------------------------------- QUERY 3 ----------------------------------------------- */

void print_q3(int is_Interativo, int encontrou, char* tag, double rating, int i, int type)
{
    if(is_Interativo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", i);
        FILE *resultado = fopen(file_name, "w");

        if(encontrou == 1)
        {
            if(type == 0)
            {
                fprintf(resultado, "%.3f\n", rating);
            }
            else if(type == 1)
            {
                fprintf(resultado, "--- 1 ---\n");
                fprintf(resultado, "rating: %.3f\n", rating);
            }
        }

        free(file_name);
        fclose(resultado);
    }
    else 
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        mvwprintw(query, 4, 10, "3 %s", tag);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);

        mvwprintw(query, 34, 90, "Página 1 de 1");
        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(query);

        if(encontrou == 1)
        {
            if(type == 0)
            {
                mvwprintw(resultado, 1, 2, "%.3f", rating);
            }
            else if(type == 1)
            {
                mvwprintw(resultado, 1, 2, "--- 1 ---");
                mvwprintw(resultado, 2, 2, "rating: %.3f", rating);
            }
        }

        wrefresh(resultado);
        int key;
        do{
            key = wgetch(resultado);
        }
        while(key != 'q');
        endwin();
    }
}




/* ------------------------------------- QUERY 4 ----------------------------------------------- */

void print_q4(int is_Interativo, void* todasReservas, int encontrou, void* hotel, int tamanho, char* tag, int i, int type) 
{
    if(is_Interativo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", i);
        FILE *resultado = fopen(file_name, "w");

        if(encontrou == 1)
        {
            for (int i = 0; i < tamanho; i++) 
            {
                void* reserva = getHotelReserv(hotel, i);

                double custo = calculaCustoPerNoite(reserva); //cuidado com void

                if(type == 0)
                {
                    fprintf(resultado, "%s;%s;%s;%s;%s;%.3f\n", getReservationId_Reserv(reserva), getReservationBeginDate_Reserv(reserva), getReservationEndDate_Reserv(reserva), getReservationUserId_Reserv(reserva), getReservationRating(reserva), custo);
                }
                else if(type == 1)
                {
                    fprintf(resultado, "--- %d ---\n", i+1);
                    fprintf(resultado, "id: %s\n", getReservationId_Reserv(reserva));
                    fprintf(resultado, "begin_date: %s\n", getReservationBeginDate_Reserv(reserva));
                    fprintf(resultado, "end_date: %s\n", getReservationEndDate_Reserv(reserva));
                    fprintf(resultado, "user_id: %s\n", getReservationUserId_Reserv(reserva));
                    fprintf(resultado, "rating: %s\n", getReservationRating(reserva));
                    fprintf(resultado, "total_price: %.3f\n", custo);
                    if (i != tamanho - 1) {fprintf(resultado, "\n");}
                }
            } 
        }

        free(file_name);
        fclose(resultado);
    }
    else
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        mvwprintw(query, 4, 10, "4 %s ", tag);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        keypad(resultado, TRUE);
        wrefresh(query);

        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
        
        int n_paginas = 0;
        if(type == 0)
        {
            n_paginas = tamanho / 25;
            n_paginas++;
        }
        else if(type == 1)
        {
            n_paginas = tamanho / 3;
            if (tamanho % 3 != 0) n_paginas++;
        }

        int startY = 1;
        if(encontrou == 1)
        {
            int key = 0;
            int pagina = 0;
            while(key != 'q')
            {
                int inicial;
                int add;
                if (type == 0) {
                    inicial = pagina * 25;
                    add = 25;
                } else {
                    inicial = pagina * 3;
                    add = 3;
                }

                wclear(resultado);  // Limpa a janela resultado
                box(resultado, 0, 0);

                startY = 1;
                for (int i = inicial; i < tamanho && i < inicial + add; i++)
                {  
                    void* reserva = getHotelReserv(hotel, i);

                    double custo = calculaCustoPerNoite(reserva); //cuidado com void

                    if(type == 0)    
                    {
                        mvwprintw(resultado, startY++, 2, "%s;%s;%s;%s;%s;%.3f", getReservationId_Reserv(reserva), getReservationBeginDate_Reserv(reserva), getReservationEndDate_Reserv(reserva), getReservationUserId_Reserv(reserva), getReservationRating(reserva), custo);
                    }
                    else
                    {
                        mvwprintw(resultado, startY++, 2, "--- %d ---", i+1);
                        mvwprintw(resultado, startY++, 2, "id: %s", getReservationId_Reserv(reserva));
                        mvwprintw(resultado, startY++, 2, "begin_date: %s", getReservationBeginDate_Reserv(reserva));
                        mvwprintw(resultado, startY++, 2, "end_date: %s", getReservationEndDate_Reserv(reserva));
                        mvwprintw(resultado, startY++, 2, "user_id: %s", getReservationUserId_Reserv(reserva));
                        mvwprintw(resultado, startY++, 2, "rating: %s", getReservationRating(reserva));
                        mvwprintw(resultado, startY++, 2, "total_price: %.3f", custo);
                        startY++;
                    }
                }   

                mvwprintw(query, 34, 90, "                   ");
                mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, n_paginas);
                
                wrefresh(query);
                wrefresh(resultado);

                key = wgetch(resultado);
                if ((key == KEY_LEFT) && pagina > 0) {
                    pagina--;
                    wrefresh(resultado);
                }
                else if ((key == KEY_RIGHT) && pagina < n_paginas - 1) {
                    pagina++;
                    wrefresh(resultado);
                }
            }
        }
        else
        {
            mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
            mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
            wrefresh(query);
            int key;
            do{
                key = wgetch(resultado);
            }
            while(key != 'q');
        }

        endwin(); 
    }

}



/* ------------------------------------- QUERY 5 ----------------------------------------------- */

void print_q5(int is_Interarivo, int encontrou, void* aeroporto, char* tag, int total_voos, char* begin, char* end, int num, int type) 
{
    if(is_Interarivo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", num);
        FILE *resultado = fopen(file_name, "w");

        if(encontrou == 1)
        {
            int inicioA = getIndiceInicio(aeroporto, end);
            int fimA = getIndiceFim(aeroporto, begin);

            if(inicioA != -1)
            {
                int c = 1;
                int v = 0;
                for(int j = inicioA; j < fimA; j++)
                {

                    if(type == 0)
                    {
                        fprintf(resultado, "%s;%s;%s;%s;%s\n", getAeroportoArrayFlID(aeroporto, j), getAeroportoArraySchDep(aeroporto, j), getAeroportoArrayDest(aeroporto, j), getAeroportoArrayAirline(aeroporto, j), getAeroportoArrayModel(aeroporto, j));
                    }
                    else if(type == 1)
                    {
                        if(v == 0) 
                        {
                            fprintf(resultado, "--- %d ---\n", c);
                            v = 1;
                        }
                        else fprintf(resultado, "\n--- %d ---\n", c);
                        fprintf(resultado, "id: %s\n", getAeroportoArrayFlID(aeroporto, j));
                        fprintf(resultado, "schedule_departure_date: %s\n", getAeroportoArraySchDep(aeroporto, j));
                        fprintf(resultado, "destination: %s\n", getAeroportoArrayDest(aeroporto, j));
                        fprintf(resultado, "airline: %s\n", getAeroportoArrayAirline(aeroporto, j));
                        fprintf(resultado, "plane_model: %s\n", getAeroportoArrayModel(aeroporto, j));

                        c++;
                    }

                }   
            }
        }

        free(file_name);
        fclose(resultado);        
    }
    else 
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        mvwprintw(query, 4, 10, "5 %s %s %s", tag, begin, end);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        keypad(resultado, TRUE);
        wrefresh(query);

        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
        
        int inicioA = getIndiceInicio(aeroporto, end);
        int fimA = getIndiceFim(aeroporto, begin);

        int startY = 1;
        if(encontrou == 1)
        {
            if(inicioA != -1)
            {
                int total = fimA - inicioA;
                int n_paginas = 0;
                if(type == 0)
                {
                    n_paginas = total / 25;
                    n_paginas++;
                }
                else if(type == 1)
                {
                    n_paginas = total / 3;
                    if (total % 3 != 0) n_paginas++;
                }

                int key = 0;
                int pagina = 0;
                while(key != 'q')
                {
                    int inicial;
                    int add;
                    if (type == 0) {
                        inicial = pagina * 25;
                        add = 25;
                    } else {
                        inicial = pagina * 3;
                        add = 3;
                    }

                    wclear(resultado);  // Limpa a janela resultado
                    box(resultado, 0, 0);

                    startY = 1;
                    int per_pagina = 0;
                    int c = inicial;
                    for (int i = inicioA; i < fimA && c < inicial + add; i++)
                    {  
                        per_pagina++;
                        if(type == 0)
                        {
                            mvwprintw(resultado, startY++, 2, "%s;%s;%s;%s;%s", getAeroportoArrayFlID(aeroporto, i), getAeroportoArraySchDep(aeroporto, i), getAeroportoArrayDest(aeroporto, i), getAeroportoArrayAirline(aeroporto, i), getAeroportoArrayModel(aeroporto, i));
                        }
                        else if(type == 1)
                        {
                            mvwprintw(resultado, startY++, 2, "--- %d ---", c+1);
                            mvwprintw(resultado, startY++, 2, "id: %s", getAeroportoArrayFlID(aeroporto, i));
                            mvwprintw(resultado, startY++, 2, "schedule_departure_date: %s", getAeroportoArraySchDep(aeroporto, i));
                            mvwprintw(resultado, startY++, 2, "destination: %s", getAeroportoArrayDest(aeroporto, i));
                            mvwprintw(resultado, startY++, 2, "airline: %s", getAeroportoArrayAirline(aeroporto, i));
                            mvwprintw(resultado, startY++, 2, "plane_model: %s", getAeroportoArrayModel(aeroporto, i));
                            startY++;

                        }
                        c++;

                    }   

                    mvwprintw(query, 34, 90, "                   ");
                    mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, n_paginas);

                    wrefresh(query);
                    wrefresh(resultado);

                    key = wgetch(resultado);
                    if ((key == KEY_LEFT) && pagina > 0) {
                        pagina--;
                        int remove = (type == 0 ? 25 : 3);
                        inicioA -= (per_pagina > remove ? per_pagina : remove);
                        wrefresh(resultado);
                    }
                    else if ((key == KEY_RIGHT) && pagina < n_paginas - 1) {
                        pagina++;
                        inicioA += per_pagina;
                        wrefresh(resultado);
                    }
                }
            }
        }
        else
        {
            mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
            mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
            wrefresh(query);
            int key;
            do{
                key = wgetch(resultado);
            }
            while(key != 'q');
        }

        endwin();
    }
}



/* ------------------------------------- QUERY 6 ----------------------------------------------- */

void print_q6(int is_Interativo, void* todosAeroportos, int tamanho, int ano, int top, int num, int type)
{
    if(is_Interativo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", num);
        FILE *resultado = fopen(file_name, "w");
        
        if(ano == 2023)
        {    
            for(int j = 0; j < top && j < tamanho; j++)
            {
                if(type == 0)
                {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    fprintf(resultado, "%s;%d\n", getAeroportoName(aeroporto), getAeroportoP23(aeroporto));

                }
                else if(type == 1)
                {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    fprintf(resultado, "--- %d ---\n", j+1);
                    fprintf(resultado, "name: %s\n", getAeroportoName(aeroporto));
                    fprintf(resultado, "passengers: %d\n", getAeroportoP23(aeroporto));
                    if (j < top - 1 && j < tamanho - 1) 
                    {
                        fprintf(resultado, "\n");
                    }
                }
            }
        }
        else if(ano == 2022)
        {
            for(int j = 0; j < top && j < tamanho; j++)
            {
                if(type == 0)
                {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    fprintf(resultado, "%s;%d\n", getAeroportoName(aeroporto), getAeroportoP22(aeroporto));

                }
                else if(type == 1)
                {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    fprintf(resultado, "--- %d ---\n", j+1);
                    fprintf(resultado, "name: %s\n", getAeroportoName(aeroporto));
                    fprintf(resultado, "passengers: %d\n", getAeroportoP22(aeroporto));
                    if (j < top - 1 && j < tamanho - 1) 
                    {
                        fprintf(resultado, "\n");
                    }
                }
            }
        }
        else if(ano == 2021)
        {
            for(int j = 0; j < top && j < tamanho; j++)
            {
                if(type == 0)
                {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    fprintf(resultado, "%s;%d\n", getAeroportoName(aeroporto), getAeroportoP21(aeroporto));

                }
                else if(type == 1)
                {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    fprintf(resultado, "--- %d ---\n", j+1);
                    fprintf(resultado, "name: %s\n", getAeroportoName(aeroporto));
                    fprintf(resultado, "passengers: %d\n", getAeroportoP21(aeroporto));
                    if (j < top - 1 && j < tamanho - 1) 
                    {
                        fprintf(resultado, "\n");
                    }
                }
            }
        }
        
        free(file_name);
        fclose(resultado);
    }
    else
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        mvwprintw(query, 4, 10, "6 %d %d", ano, top);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        keypad(resultado, TRUE);
        wrefresh(query);

        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");

        
        int utilizavel = 0;
        if (top > tamanho) utilizavel = tamanho;
        else utilizavel = top;

        int paginas = (utilizavel) / 25;
        if (type == 0) { // 25 é o numero de linhas q cabem do type 0 por pagina
            paginas = (utilizavel) / 25;
            paginas++;
        } else { // cada type 1 gasta 3 linhas,  logo * 3
            paginas = utilizavel / 6;
            if (utilizavel % 6 != 0) paginas++;
        }

        int startY = 1;
        int pagina = 0;
        int key = 0;
        while (key != 'q') 
        {
            int inicial;
            int add;
            if (type == 0) {
                inicial = pagina * 25;
                add = 25;
            } else {
                inicial = pagina * 6;
                add = 6;
            }

            wclear(resultado);  // Limpa a janela resultado
            box(resultado, 0, 0);

            startY = 1;
            for(int j = inicial; j < top && j < tamanho && j < inicial + add; j++) 
            {   
                if(type == 0)
                {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    if(ano == 2023) mvwprintw(resultado, startY++, 2, "%s;%d", getAeroportoName(aeroporto), getAeroportoP23(aeroporto));
                    else if(ano == 2022) mvwprintw(resultado, startY++, 2, "%s;%d", getAeroportoName(aeroporto), getAeroportoP22(aeroporto));
                    else if(ano == 2021) mvwprintw(resultado, startY++, 2, "%s;%d", getAeroportoName(aeroporto), getAeroportoP21(aeroporto));
                }
                else if (type == 1) {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    mvwprintw(resultado, startY++, 2, "--- %d ---", j+1);
                    mvwprintw(resultado, startY++, 2, "name: %s", getAeroportoName(aeroporto));
                    if(ano == 2023) mvwprintw(resultado, startY++, 2, "passengers: %d", getAeroportoP23(aeroporto));
                    else if(ano == 2022) mvwprintw(resultado, startY++, 2, "passengers: %d", getAeroportoP22(aeroporto));
                    else if(ano == 2021) mvwprintw(resultado, startY++, 2, "passengers: %d", getAeroportoP21(aeroporto));
                    startY++;
                }
            }

            mvwprintw(query, 34, 90, "                   ");
            mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, paginas);
                
            wrefresh(query);
            wrefresh(resultado);
                
            key = wgetch(resultado);
            if ((key == KEY_LEFT) && pagina > 0) {
                pagina--;
                wrefresh(resultado);
            }
            else if ((key == KEY_RIGHT) && pagina < paginas - 1) {
                pagina++;
                wrefresh(resultado);
            }
            
        }

        endwin();
    }
}



/* ------------------------------------- QUERY 7 ----------------------------------------------- */


void print_q7(int is_Interativo, void* todosAeroportos, int tamanho, int top, int num, int type)
{
    if(is_Interativo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", num);
        FILE *resultado = fopen(file_name, "w");

        for(int j = 0; j < top && j < tamanho && getAeroportosMediana_dir(todosAeroportos, j) >= 0; j++)
        {
            if(type == 0)
            {
                void* aeroporto = getAeroporto(todosAeroportos, j);
                fprintf(resultado, "%s;%d\n", getAeroportoName(aeroporto), getAeroportosMediana_dir(todosAeroportos, j));
            }
            else if(type == 1)
            {
                void* aeroporto = getAeroporto(todosAeroportos, j);
                fprintf(resultado, "--- %d ---\n", j+1);
                fprintf(resultado, "name: %s\n",  getAeroportoName(aeroporto));
                fprintf(resultado, "median: %d\n", getAeroportosMediana_dir(todosAeroportos, j));
                
                if (j < top - 1 && j < tamanho - 1 && getAeroportosMediana_dir(todosAeroportos, j+1) >= 0)
                {
                    fprintf(resultado, "\n");
                }
            }
        }

        free(file_name);
        fclose(resultado);
    }
    else 
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        mvwprintw(query, 4, 10, "7 %d", top);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        keypad(resultado, TRUE);
        wrefresh(query);

        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
        
        int total = 0;
        for (int i = 0; i < tamanho; i++) {
            if (getAeroportosMediana_dir(todosAeroportos, i) >= 0) total++;
        }

        int utilizavel = 0;
        if (top > total) utilizavel = total;
        else utilizavel = top;

        int paginas = (utilizavel) / 25;
        if (type == 0) { // 25 é o numero de linhas q cabem do type 0 por pagina
            paginas = (utilizavel) / 25;
            paginas++;
        } else { // cada type 1 gasta 3 linhas,  logo * 3
            paginas = utilizavel / 6;
            if (utilizavel % 6 != 0) paginas++;
        }

        int startY = 1;
        int pagina = 0;
        int key;
        while (key != 'q') 
        {
            int inicial;
            int add;
            if (type == 0) {
                inicial = pagina * 25;
                add = 25;
            } else {
                inicial = pagina * 6;
                add = 6;
            }

            wclear(resultado);  // Limpa a janela resultado
            box(resultado, 0, 0);

            startY = 1;
            for(int j = inicial; j < top && j < tamanho && getAeroportosMediana_dir(todosAeroportos, j) >= 0 && j < inicial + add; j++) {   
                if(type == 0)
                {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    mvwprintw(resultado, startY++, 2, "%s;%d", getAeroportoName(aeroporto), getAeroportosMediana_dir(todosAeroportos, j));
                }
                else if (type == 1) {
                    void* aeroporto = getAeroporto(todosAeroportos, j);
                    mvwprintw(resultado, startY++, 2, "--- %d ---", j+1);
                    mvwprintw(resultado, startY++, 2, "name: %s",  getAeroportoName(aeroporto));
                    mvwprintw(resultado, startY++, 2, "median: %d", getAeroportosMediana_dir(todosAeroportos, j));
                    startY++;
                }
            }
            
            mvwprintw(query, 34, 90, "                   ");
            mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, paginas);
                
            wrefresh(query);
            wrefresh(resultado);
                
            key = wgetch(resultado);
            if ((key == KEY_LEFT) && pagina > 0) {
                pagina--;
                wrefresh(resultado);
            }
            else if ((key == KEY_RIGHT) && pagina < paginas - 1) {
                pagina++;
                wrefresh(resultado);
            }
            
        }
        
        endwin();
    }
}



/* ------------------------------------- QUERY 8 ----------------------------------------------- */

void print_q8(int is_Interarivo, int encontrou, char* hotel, int revenue, char* begin_date1, char* end_date1, int i, int type)
{
    if(is_Interarivo == 0)
    { 
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", i);
        FILE *resultado = fopen(file_name, "w");

        if(encontrou == 1)
        {   
            if (type == 0) {
                fprintf(resultado, "%d\n", revenue);
            }
            else if (type == 1) {
                fprintf(resultado, "--- 1 ---\n");
                fprintf(resultado, "revenue: %d\n", revenue);
            }
        }

        free(file_name);
        fclose(resultado);
    }
    else 
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        mvwprintw(query, 4, 10, "8 %s %s %s", hotel, begin_date1, end_date1);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        keypad(resultado, TRUE);

        mvwprintw(query, 34, 90, "Página 1 de 1");
        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(query);

        if(encontrou == 1)
        {   
            if (type == 0) {
            mvwprintw(resultado, 1, 2, "%d", revenue);
            }
            else if (type == 1) {
                mvwprintw(resultado, 1, 2, "--- 1 ---");
                mvwprintw(resultado, 2, 2, "revenue: %d", revenue);
            }

            wrefresh(resultado);
            int key = 0;
            while (key != 'q') {
                key = wgetch(resultado);
            }
        }

        endwin();
    }
}



/* ------------------------------------- QUERY 9 ----------------------------------------------- */

void print_q9(int is_Interativo, char* prefix, void* allUsers, int tamanho, int i, int type)
{
    int vez = 0;
    if(is_Interativo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", i);
        FILE *resultado = fopen(file_name, "w");

        for (int j = 0; j < tamanho; j++) {
            void* currentUser = getUserArray(allUsers, j);
            if (comecaComPrefix(getUserName(currentUser), prefix) && !isInactive(getUserStatus(currentUser))) {
                if(type == 0)
                {
                    fprintf(resultado, "%s;%s\n", getUserId(currentUser), getUserName(currentUser));
                }
                else if(type == 1)
                {

                    if (vez == 0) fprintf(resultado, "--- %d ---\n", vez + 1);
                    else fprintf(resultado, "\n--- %d ---\n", vez + 1);
                    fprintf(resultado, "id: %s\n", getUserId(currentUser));
                    fprintf(resultado, "name: %s\n", getUserName(currentUser));

                    vez++;
                }
            }
        }

        free(file_name);
        fclose(resultado);
    }
    else
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        mvwprintw(query, 4, 10, "9 %s", prefix);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        keypad(resultado, TRUE);
        wrefresh(query);

        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");

        int inicio = -1;
        int total = 0;
        for (int i = 0; i < tamanho; i++) {
            void* currentUser = getUserArray(allUsers, i);
            if (comecaComPrefix(getUserName(currentUser), prefix) && !isInactive(getUserStatus(currentUser))) {
                if (inicio == -1) inicio = i;
                total++;
            }
        }

        int utilizavel = total;
        int paginas;
        if (type == 0) {
            paginas = (utilizavel) / 25;
            paginas++;
        } else {
            paginas = (utilizavel) / 6;
            if ((utilizavel) % 6 != 0) paginas++;
        }

        int startY = 1;
        int pagina = 0;
        int key;
        while (key != 'q') 
        {
            int inicial;
            int add;
            if (type == 0) {
                inicial = inicio + (pagina * 25);
                add = 25;
            } else {
                inicial = inicio + (pagina * 6);
                add = 6;
            }

            wclear(resultado);  // Limpa a janela resultado
            box(resultado, 0, 0);

            startY = 1;
            for (int j = inicial; j < inicial + add && j < inicio + total; j++) 
            {
                void* currentUser = getUserArray(allUsers, j);
                
                if(type == 0)
                {
                    mvwprintw(resultado, startY++, 2, "%s;%s", getUserId(currentUser), getUserName(currentUser));
    
                }
                else if(type == 1)
                {
                    mvwprintw(resultado, startY++, 2, "--- %d ---", j-inicio + 1);
                    mvwprintw(resultado, startY++, 2, "id: %s", getUserId(currentUser));
                    mvwprintw(resultado, startY++, 2, "name: %s", getUserName(currentUser));
                    startY++;
    
                    vez++;
                }
            }

            mvwprintw(query, 34, 90, "                   ");
            mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, paginas);
                
            wrefresh(query);
            wrefresh(resultado);
                
            key = wgetch(resultado);
            if ((key == KEY_LEFT) && pagina > 0) {
                pagina--;
                wrefresh(resultado);
            }
            else if ((key == KEY_RIGHT) && pagina < paginas - 1) {
                pagina++;
                wrefresh(resultado);
            }
        }
        endwin();
    }
}



/* ------------------------------------- QUERY 10 ----------------------------------------------- */

void print_q10(int is_Interativo, void* U_stats, void* R_stats, void* P_stats, void* V_stats, char* ano_c, char* mes_c, int i, int type)
{
    char* anos[14] = {"2010","2011","2012","2013","2014","2015","2016","2017","2018","2019","2020","2021","2022","2023"};

    if(is_Interativo == 0)
    {
        char *file_name = malloc(50);
        sprintf(file_name, "./Resultados/command%d_output.txt", i);
        FILE *resultado = fopen(file_name, "w");

        int ver = 0;
        if(ano_c == NULL)
        {
            if(type == 0)
            {
                for(int p = 0; p < 14; p++)
                {
                    fprintf(resultado, "%s;%d;%d;%d;%d;%d\n", anos[p],getTotalAno(U_stats, anos[p]), getTotalAno(V_stats, anos[p]), getTotalAno(P_stats, anos[p]), getTotalAnoUnico(P_stats, anos[p]), getTotalAno(R_stats, anos[p]));
                }
            }
            else if(type == 1)
            {
                for(int p = 0; p < 14; p++)
                {
                    if(ver == 0) 
                    {
                        fprintf(resultado, "--- %d ---\n", p+1);
                        ver = 1;
                    }
                    else fprintf(resultado, "\n--- %d ---\n", p+1);
                    fprintf(resultado, "year: %s\n", anos[p]);
                    fprintf(resultado, "users: %d\n", getTotalAno(U_stats, anos[p]));
                    fprintf(resultado, "flights: %d\n", getTotalAno(V_stats, anos[p]));
                    fprintf(resultado, "passengers: %d\n", getTotalAno(P_stats, anos[p]));
                    fprintf(resultado, "unique_passengers: %d\n", getTotalAnoUnico(P_stats, anos[p]));
                    fprintf(resultado, "reservations: %d\n", getTotalAno(R_stats, anos[p]));
                }
            }
        }
        else if(mes_c == NULL)
        {
            if(type == 0)
            {
                for(int p = 0; p < 12; p++)
                {
                    int users = getTotalMes(U_stats, ano_c, p+1);
                    int voos = getTotalMes(V_stats, ano_c, p+1);
                    int passageiros = getTotalMes(P_stats, ano_c, p+1);
                    int pUnicos = getTotalMesUnico(P_stats, ano_c, p+1);
                    int reservas = getTotalMes(R_stats, ano_c, p+1);

                    if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0)
                        fprintf(resultado, "%d;%d;%d;%d;%d;%d\n", p+1, users, voos, passageiros, pUnicos, reservas);
                }
            }
            else if(type == 1)
            {
                int c = 1;
                for(int p = 0; p < 12; p++)
                {
                    int users = getTotalMes(U_stats, ano_c, p+1);
                    int voos = getTotalMes(V_stats, ano_c, p+1);
                    int passageiros = getTotalMes(P_stats, ano_c, p+1);
                    int pUnicos = getTotalMesUnico(P_stats, ano_c, p+1);
                    int reservas = getTotalMes(R_stats, ano_c, p+1);

                    if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0)
                    {
                        if(ver == 0) 
                        {
                            fprintf(resultado, "--- %d ---\n", c);
                            ver = 1;
                        }
                        else fprintf(resultado, "\n--- %d ---\n", c);
                        fprintf(resultado, "month: %d\n", p+1);
                        fprintf(resultado, "users: %d\n", users);
                        fprintf(resultado, "flights: %d\n", voos);
                        fprintf(resultado, "passengers: %d\n", passageiros);
                        fprintf(resultado, "unique_passengers: %d\n", pUnicos);
                        fprintf(resultado, "reservations: %d\n", reservas);

                        c++;
                    }
                }
            }
        }
        else
        {
            int mes = atoi(mes_c);
                
            if(type == 0)
            {    
                for(int p = 0; p < 31; p++)
                {
                    int users = getTotalDias(U_stats, ano_c, mes, p+1);
                    int voos = getTotalDias(V_stats, ano_c, mes, p+1);
                    int passageiros = getTotalDias(P_stats, ano_c, mes, p+1);
                    int pUnicos = getTotalDiasUnicos(P_stats, ano_c, mes, p+1);
                    int reservas = getTotalDias(R_stats, ano_c, mes, p+1);

                    if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0)
                        fprintf(resultado, "%d;%d;%d;%d;%d;%d\n", p+1, users, voos, passageiros, pUnicos, reservas);
                }
            }
            else if(type == 1)
            {
                int c = 1;
                int mes = atoi(mes_c);

                for(int p = 0; p < 31; p++)
                {
                    int users = getTotalDias(U_stats, ano_c, mes, p+1);
                    int voos = getTotalDias(V_stats, ano_c, mes, p+1);
                    int passageiros = getTotalDias(P_stats, ano_c, mes, p+1);
                    int pUnicos = getTotalDiasUnicos(P_stats, ano_c, mes, p+1);
                    int reservas = getTotalDias(R_stats, ano_c, mes, p+1);

                    if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0)
                    {
                        if(ver == 0) 
                        {
                            fprintf(resultado, "--- %d ---\n", c);
                            ver = 1;
                        }
                        else fprintf(resultado, "\n--- %d ---\n", c);
                        fprintf(resultado, "day: %d\n", p+1);
                        fprintf(resultado, "users: %d\n", users);
                        fprintf(resultado, "flights: %d\n", voos);
                        fprintf(resultado, "passengers: %d\n", passageiros);
                        fprintf(resultado, "unique_passengers: %d\n", pUnicos);
                        fprintf(resultado, "reservations: %d\n", reservas);

                        c++;
                    }
                }

            }
        }

        free(file_name);
        fclose(resultado);
    }
    else
    {
        initscr();
        noecho();
        curs_set(0);
        cbreak();
        flushinp();

        WINDOW *query = newwin(40, 131, 0, 0);
        box(query, 0, 0);
        wclear(query);

        mvwprintw(query, 3, 10, "Input:");
        if(ano_c == NULL) mvwprintw(query, 4, 10, "10");
        else if(mes_c == NULL) mvwprintw(query, 4, 10, "10 %s", ano_c);
        else mvwprintw(query, 4, 10, "10 %s %s", ano_c, mes_c);
        mvwprintw(query, 6, 10, "Resultado:");
        WINDOW *resultado = newwin(27, 100, 7, 10);
        box(resultado, 0, 0);
        keypad(resultado, TRUE);
        wrefresh(query);

        mvwprintw(query, 36, 30, "Utilize as setas do teclado para navegar entre as páginas.");
        mvwprintw(query, 37, 38, "Pressione a tecla 'q' para voltar atrás.");

        if(ano_c == NULL)
        {
            int total = 14;
            int paginas;
            if (type == 0) {
                paginas = total / 25;
                paginas++;
            } else {
                paginas = total / 3;
                if (total % 3 != 0) paginas++;
            }

            int startY = 1;
            int pagina = 0;
            int key = 0;
            while(key != 'q')
            {
                int inicial;
                int add;
                if (type == 0) {
                    inicial = pagina * 25;
                    add = 25;
                } else {
                    inicial = pagina * 3;
                    add = 3;
                }

                wclear(resultado);  // Limpa a janela resultado
                box(resultado, 0, 0);

                startY = 1;
                if(type == 0)
                {   
                    for(int p = 0; p < 14; p++)
                    {
                        mvwprintw(resultado, startY++, 2, "%s;%d;%d;%d;%d;%d", anos[p],getTotalAno(U_stats, anos[p]), getTotalAno(V_stats, anos[p]), getTotalAno(P_stats, anos[p]), getTotalAnoUnico(P_stats, anos[p]), getTotalAno(R_stats, anos[p]));
                    }
                }
                else if(type == 1)
                {
                    for(int p = inicial; p < 14 && p < inicial + add; p++)
                    {
                        mvwprintw(resultado, startY++, 2, "--- %d ---", p+1);
                        mvwprintw(resultado, startY++, 2, "year: %s", anos[p]);
                        mvwprintw(resultado, startY++, 2, "users: %d", getTotalAno(U_stats, anos[p]));
                        mvwprintw(resultado, startY++, 2, "flights: %d", getTotalAno(V_stats, anos[p]));
                        mvwprintw(resultado, startY++, 2, "passengers: %d", getTotalAno(P_stats, anos[p]));
                        mvwprintw(resultado, startY++, 2, "unique_passengers: %d", getTotalAnoUnico(P_stats, anos[p]));
                        mvwprintw(resultado, startY++, 2, "reservations: %d", getTotalAno(R_stats, anos[p]));
                        startY++;
                    }
                }
                
                mvwprintw(query, 34, 90, "                   ");
                mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, paginas);

                wrefresh(query);
                wrefresh(resultado);

                key = wgetch(resultado);
                if ((key == KEY_LEFT) && pagina > 0) {
                    pagina--;
                    wrefresh(resultado);
                }
                else if ((key == KEY_RIGHT) && pagina < paginas - 1) {
                    pagina++;
                    wrefresh(resultado);
                }
            }
        }
        else if(mes_c == NULL)
        {
            int total = 0;
            for (int i = 0; i < 12; i++) {
                int users = getTotalMes(U_stats, ano_c, i+1);
                int voos = getTotalMes(V_stats, ano_c, i+1);
                int passageiros = getTotalMes(P_stats, ano_c, i+1);
                int pUnicos = getTotalMesUnico(P_stats, ano_c, i+1);
                int reservas = getTotalMes(R_stats, ano_c, i+1);
                if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0) {
                    total++;
                }
            }

            int paginas;
            if(type == 0)
            {
                paginas = total / 25;
                paginas++;
            }
            else if(type == 1)
            {
                paginas = total / 3;
                if (total % 3 != 0) paginas++;
            }

            int startY = 1;
            int pagina = 0;
            int key;

            int next[paginas+1];
            int iter[paginas];
            for (int i = 0; i < paginas; i++) {
                next[i] = 0;
                iter[i] = 0;
            }
            next[paginas+1] = 0;

            int counter = 0;
            while (key != 'q')
            {
                startY = 1;
                wclear(resultado);
                box(resultado, 0, 0);   
                int inicial;
                    

                if(type == 0)
                {
                    for(int p = 0; p < 12; p++)
                    {
                        int users = getTotalMes(U_stats, ano_c, p+1);
                        int voos = getTotalMes(V_stats, ano_c, p+1);
                        int passageiros = getTotalMes(P_stats, ano_c, p+1);
                        int pUnicos = getTotalMesUnico(P_stats, ano_c, p+1);
                        int reservas = getTotalMes(R_stats, ano_c, p+1);

                        if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0) {
                            mvwprintw(resultado, startY++, 2, "%d;%d;%d;%d;%d;%d", p+1, users, voos, passageiros, pUnicos, reservas);
                        }
                    }
                }
                else if(type == 1)
                {
                    inicial = (pagina * 3) + next[pagina];

                    int per_pagina = 0;
                    for(int p = inicial; p < 12 && per_pagina < 3; p++)
                    {
                        int users = getTotalMes(U_stats, ano_c, p+1);
                        int voos = getTotalMes(V_stats, ano_c, p+1);
                        int passageiros = getTotalMes(P_stats, ano_c, p+1);
                        int pUnicos = getTotalMesUnico(P_stats, ano_c, p+1);
                        int reservas = getTotalMes(R_stats, ano_c, p+1);

                        if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0)
                        {
                            per_pagina++;
                            mvwprintw(resultado, startY++, 2, "--- %d ---", counter + per_pagina);
                            mvwprintw(resultado, startY++, 2, "month: %d", p+1);
                            mvwprintw(resultado, startY++, 2, "users: %d", users);
                            mvwprintw(resultado, startY++, 2, "flights: %d", voos);
                            mvwprintw(resultado, startY++, 2, "passengers: %d", passageiros);
                            mvwprintw(resultado, startY++, 2, "unique_passengers: %d", pUnicos);
                            mvwprintw(resultado, startY++, 2, "reservations: %d", reservas);
                            startY++;
                        } else {
                            if (iter[pagina] == 0) {
                                for (int i = pagina+1; i < paginas+1; i++){
                                    next[i]++;
                                }
                            }
                        }
                    }
                    iter[pagina] = 1;
                }

                mvwprintw(query, 34, 90, "                   ");
                mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, paginas);

                wrefresh(query);
                wrefresh(resultado);

                key = wgetch(resultado);
                if ((key == KEY_LEFT) && pagina > 0) {
                    pagina--;
                    counter -= 3;
                    wrefresh(query);
                }
                else if ((key == KEY_RIGHT) && pagina < paginas - 1) {
                    pagina++;
                    counter += 3;
                    wrefresh(query);
                }
            }   
        }
        else
        {
            int mes = atoi(mes_c);
                
            int total = 0;
            for (int i = 0; i < 31; i++) {
                int users = getTotalDias(U_stats, ano_c, mes, i+1);
                int voos = getTotalDias(V_stats, ano_c, mes, i+1);
                int passageiros = getTotalDias(P_stats, ano_c, mes, i+1);
                int pUnicos = getTotalDiasUnicos(P_stats, ano_c, mes, i+1);
                int reservas = getTotalDias(R_stats, ano_c, mes, i+1);
                if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0) {
                    total++;
                }
            }

            int paginas;
            if(type == 0)
            {
                paginas = total / 25;
                paginas++;
            }
            else if(type == 1)
            {
                paginas = total / 3;
                if (total % 3 != 0) paginas++;
            }

            int startY = 1;
            int pagina = 0;
            int key;

            int next[paginas+1];
            int iter[paginas];
            for (int i = 0; i < paginas; i++) {
                next[i] = 0;
                iter[i] = 0;
            }
            next[paginas+1] = 0;

            int counter = 0;
            while (key != 'q')
            {
                startY = 1;
                wclear(resultado);
                box(resultado, 0, 0);   
                int inicial;
                    

                if(type == 0)
                {
                    inicial = (pagina * 25) + next[pagina];

                    int per_pagina = 0;
                    for(int p = inicial; p < 31 && per_pagina < 25 ; p++)
                    {
                        int users = getTotalDias(U_stats, ano_c, mes, p+1);
                        int voos = getTotalDias(V_stats, ano_c, mes, p+1);
                        int passageiros = getTotalDias(P_stats, ano_c, mes, p+1);
                        int pUnicos = getTotalDiasUnicos(P_stats, ano_c, mes, p+1);
                        int reservas = getTotalDias(R_stats, ano_c, mes, p+1);

                        if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0) {
                            per_pagina++;
                            mvwprintw(resultado, startY++, 2, "%d;%d;%d;%d;%d;%d", p+1, users, voos, passageiros, pUnicos, reservas);
                        } else {
                            if (iter[pagina] == 0) {
                                for (int i = pagina+1; i < paginas+1; i++){
                                    next[i]++;
                                }
                            }
                        }
                    }
                    iter[pagina] = 1;
                }
                else if(type == 1)
                {
                    inicial = (pagina * 3) + next[pagina];

                    int per_pagina = 0;
                    for(int p = inicial; p < 31 && per_pagina < 3; p++)
                    {
                        int users = getTotalDias(U_stats, ano_c, mes, p+1);
                        int voos = getTotalDias(V_stats, ano_c, mes, p+1);
                        int passageiros = getTotalDias(P_stats, ano_c, mes, p+1);
                        int pUnicos = getTotalDiasUnicos(P_stats, ano_c, mes, p+1);
                        int reservas = getTotalDias(R_stats, ano_c, mes, p+1);

                        if(users != 0 || voos != 0 || passageiros != 0 || pUnicos != 0 || reservas != 0)
                        {
                            per_pagina++;
                            mvwprintw(resultado, startY++, 2, "--- %d ---", counter + per_pagina);
                            mvwprintw(resultado, startY++, 2, "day: %d", p+1);
                            mvwprintw(resultado, startY++, 2, "users: %d", users);
                            mvwprintw(resultado, startY++, 2, "flights: %d", voos);
                            mvwprintw(resultado, startY++, 2, "passengers: %d", passageiros);
                            mvwprintw(resultado, startY++, 2, "unique_passengers: %d", pUnicos);
                            mvwprintw(resultado, startY++, 2, "reservations: %d", reservas);
                            startY++;
                        } else {
                            if (iter[pagina] == 0) {
                                for (int i = pagina+1; i < paginas+1; i++){
                                    next[i]++;
                                }
                            }
                        }
                    }
                    iter[pagina] = 1;
                }

                mvwprintw(query, 34, 90, "                   ");
                mvwprintw(query, 34, 90, "Página %d de %d", pagina + 1, paginas);

                wrefresh(query);
                wrefresh(resultado);

                key = wgetch(resultado);
                if ((key == KEY_LEFT) && pagina > 0) {
                    pagina--;
                    counter -= 3;
                    wrefresh(query);
                }
                else if ((key == KEY_RIGHT) && pagina < paginas - 1) {
                    pagina++;
                    counter += 3;
                    wrefresh(query);
                }
            }
        }
        endwin();
    }
}