#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "../include/parser.h"
#include "../include/voos.h"
#include "../include/utilizadores.h"
#include "../include/datas.h"
#include "../include/queries.h"
#include "../include/reservas.h"
#include "../include/passageiros.h"
#include "../include/utilidades.h"
#include "../include/interativo.h"
#include "../include/parser_interativo.h"
#include "../include/output.h"
#include "../include/utilizadores_dados.h"
#include "../include/voos_dados.h"
#include "../include/reservas_dados.h"
#include <time.h>
#include <sys/resource.h>
#include <ncurses.h>
#define MAX_LINHA 1000



int main (int argc, char* argv[]) 
{
    if (argc < 2) {

        //setlocale(LC_COLLATE, "en_US.UTF-8");

        initscr();
        curs_set(0);
        start_color();

        int opcao = menu_inicial();
        if (opcao == 1) return 0;
        char* input = menu_inserirpath();
        
        WINDOW *statusWin = newwin(5, 50, 10, 40); // Ajuste as dimensões e a posição conforme necessário
        box(statusWin, 0, 0);
        refresh();

        loading(statusWin, "A carregar estatísticas...");
        void* usersStats = initStats();
        void* flightStats = initStats();
        void* reservStats = initStats();
        void* passStats = initStats();
        napms(1);

        loading(statusWin, "A carregar usuários...");
        char filepath_u[1000];
        snprintf(filepath_u, sizeof(filepath_u), "%s/users.csv", input);
        FILE *users = fopen(filepath_u, "r");
        void *users_dados = parser(users, verificaUser, organizaUser, NULL, "users_errors.csv", "id;name;email;phone_number;birth_date;sex;passport;country_code;address;account_creation;pay_method;account_status", usersStats);
        fclose(users);
        napms(1);

        loading(statusWin, "A carregar voos...");
        char filepath_v[1000];
        snprintf(filepath_v, sizeof(filepath_v), "%s/flights.csv", input);
        FILE* voos = fopen(filepath_v, "r");
        void *voos_dados = parser(voos, verificaVoo, organizaVoo, NULL, "flights_errors.csv", "id;airline;plane_model;total_seats;origin;destination;schedule_departure_date;schedule_arrival_date;real_departure_date;real_arrival_date;pilot;copilot;notes", flightStats);
        fclose(voos);
        napms(1);

        loading(statusWin, "A carregar reservas...");
        char filepath_r[1000] ;
        snprintf(filepath_r, sizeof(filepath_r), "%s/reservations.csv", input);
        FILE *reservas = fopen(filepath_r, "r");
        void *reservas_dados = parser(reservas, verificaReserva, organizaReserva, users_dados, "reservations_errors.csv", "id;user_id;hotel_id;hotel_name;hotel_stars;city_tax;address;begin_date;end_date;price_per_night;includes_breakfast;room_details;rating;comment", reservStats);
        fclose(reservas);
        napms(1);

        loading(statusWin, "A carregar passageiros...");
        char filepath_p[1000];
        snprintf(filepath_p, sizeof(filepath_p), "%s/passengers.csv", input);
        FILE *passageiros = fopen(filepath_p, "r");

        parser_passageiros(passageiros, verificaPassageiros, users_dados, voos_dados, passStats);
        fclose(passageiros);
        napms(1);

        loading(statusWin, "A carregar funções auxiliares...");
        insereReservas_User(users_dados, reservas_dados); //insere as reservas em q o user faz parte

        void *hoteisDadosMedia = insereMediaHoteis(reservas_dados); //percorre todas as reservas validas e insere os ratings numa struct

        void *aeroportos_dados = criaSaveAeroportos(voos_dados);
        calcularMedianaDelays(aeroportos_dados);

        ordenaNomes(users_dados);
        napms(1);

        
        endwin();
        while (1) {
            refresh();
            int k = menu_principal();
            refresh();
            if(k == 0)
            {
                menu_queries(voos_dados, users_dados, reservas_dados, hoteisDadosMedia, aeroportos_dados, usersStats, flightStats, reservStats, passStats);
            }
            if (k == 1)
            {
                while (1)
                {
                    refresh();
                    int current = exemplos_queries();
                    if(current == 10) break;
                }                
            } 
            if (k == 70) {endwin(); return 0;} 
        }
    }
    else
    {
        //setlocale(LC_COLLATE, "en_US.UTF-8");

        if (argc != 3) {
            printf("INVALID INPUT.\n");
            return 1;
        };

        void* usersStats = initStats();
        void* flightStats = initStats();
        void* reservStats = initStats();
        void* passStats = initStats();

        char filepath_u[1000];
        snprintf(filepath_u, sizeof(filepath_u), "%s/users.csv", argv[1]);
        FILE *users = fopen(filepath_u, "r");
        if (users == NULL) {
            printf("ERROR: CAN'T OPEN FILE\n");
            return 1;
        }
        void *users_dados = parser(users, verificaUser, organizaUser, NULL, "users_errors.csv", "id;name;email;phone_number;birth_date;sex;passport;country_code;address;account_creation;pay_method;account_status", usersStats);
        fclose(users);

        char filepath_v[1000];
        snprintf(filepath_v, sizeof(filepath_v), "%s/flights.csv", argv[1]);
        FILE* voos = fopen(filepath_v, "r");
        if (voos == NULL) {
            printf("ERROR: CAN'T OPEN FILE\n");
            return 1;
        }
        void *voos_dados = parser(voos, verificaVoo, organizaVoo, NULL, "flights_errors.csv", "id;airline;plane_model;total_seats;origin;destination;schedule_departure_date;schedule_arrival_date;real_departure_date;real_arrival_date;pilot;copilot;notes", flightStats);
        fclose(voos);

        char filepath_r[1000] ;
        snprintf(filepath_r, sizeof(filepath_r), "%s/reservations.csv", argv[1]);
        FILE *reservas = fopen(filepath_r, "r");
        if (reservas == NULL) {
            printf("ERROR: CAN'T OPEN FILE\n");
            return 1;
        }
        void *reservas_dados = parser(reservas, verificaReserva, organizaReserva, users_dados, "reservations_errors.csv", "id;user_id;hotel_id;hotel_name;hotel_stars;city_tax;address;begin_date;end_date;price_per_night;includes_breakfast;room_details;rating;comment", reservStats);
        fclose(reservas);

        char filepath_p[1000];
        snprintf(filepath_p, sizeof(filepath_p), "%s/passengers.csv", argv[1]);
        FILE *passageiros = fopen(filepath_p, "r");
        if (passageiros == NULL) {
            printf("ERROR: CAN'T OPEN FILE\n");
            return 1;
        }
        parser_passageiros(passageiros, verificaPassageiros, users_dados, voos_dados, passStats);
        fclose(passageiros);

        insereReservas_User(users_dados, reservas_dados); //insere as reservas em q o user faz parte

        void *hoteisDadosMedia = insereMediaHoteis(reservas_dados); //percorre todas as reservas validas e insere os ratings numa struct

        void *aeroportos_dados = criaSaveAeroportos(voos_dados);
        calcularMedianaDelays(aeroportos_dados);

        ordenaNomes(users_dados);

        FILE *input = fopen(argv[2], "r");
        if (input == NULL) {
            printf("ERROR: CAN'T OPEN FILE\n");
            return 1;
        }
        parse_query(input, voos_dados, users_dados, reservas_dados, hoteisDadosMedia, aeroportos_dados, usersStats, flightStats, reservStats, passStats);
        fclose(input);
        
        // liberta a memória alocada das structs
        free_hoteis_data(hoteisDadosMedia);
        free_reservations_data(reservas_dados); 
        free_users_data(users_dados); 
        free_aeroportos_data(aeroportos_dados);
        free_voos_data(voos_dados);
        free_stats_hash(usersStats);
        free_stats_hash(flightStats);
        free_stats_hash(reservStats);
        free_stats_hash(passStats);
    }

    return 0;
}