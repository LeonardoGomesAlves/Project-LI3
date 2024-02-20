#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/voos.h"
#include "../include/utilizadores.h"
#include "../include/datas.h"
#include "../include/queries.h"
#include "../include/reservas.h"
#include "../include/passageiros.h"
#include "../include/utilidades.h"
#include "../include/utilizadores_dados.h"
#include "../include/voos_dados.h"
#include "../include/reservas_dados.h"
#include "../include/output.h"
#include <time.h>
#include <sys/resource.h>


int compare_output(const char *expected_file, const char *actual_file) 
{
    FILE *expected_fp = fopen(expected_file, "r");
    FILE *result_fp = fopen(actual_file, "r");

    if (expected_fp == NULL || result_fp == NULL) {
        printf("ERROR: CAN'T OPEN FILES: %s, %s\n", expected_file, actual_file);
        return 0;
    }

    char expected_line[1000];
    char result_line[1000];
    int line_number = 1;
    int test_passed = 1; // asume que o teste passou

    if (fgets(expected_line, sizeof(expected_line), expected_fp) != NULL &&
        fgets(result_line, sizeof(result_line), result_fp) == NULL) 
    {
        test_passed = 0;
        printf("Teste falhou na linha %d\n", line_number);
        printf("Os ficheiros têm número de linhas diferentes.\n");
        printf("-----------------------------------------------\n");
        fclose(expected_fp);
        fclose(result_fp);
        return test_passed;
    }

    while (fgets(expected_line, sizeof(expected_line), expected_fp) != NULL &&
           fgets(result_line, sizeof(result_line), result_fp) != NULL) 
    {
        expected_line[strcspn(expected_line, "\n")] = '\0';
        result_line[strcspn(result_line, "\n")] = '\0';

        // Se a resposta esperada for NULL, então a resposta real também deve ser NULL
        if ((expected_line[0] == '\0' || strcmp(expected_line, "NULL") == 0) && result_line[0] != '\0') 
        {
            test_passed = 0; 
            printf("Teste falhou na linha %d\n", line_number);
            printf("-----------------------------------------------\n");
            break;
        }

        // Comparar linha com o resultado real
        if (strcmp(result_line, expected_line) != 0) 
        {
            test_passed = 0; 
            printf("Teste falhou na linha %d\n", line_number);
            printf("-----------------------------------------------\n");
            break;
        }

        line_number++;
    }

    // Verificar se ambos os arquivos atingiram o final simultaneamente
    if (fgets(expected_line, sizeof(expected_line), expected_fp) != NULL ||
        fgets(result_line, sizeof(result_line), result_fp) != NULL) 
    {
        test_passed = 0; 
        printf("Teste falhou na linha %d\n", line_number);
        printf("Os ficheiros têm número de linhas diferentes.\n");
        printf("-----------------------------------------------\n");
    }

    fclose(expected_fp);
    fclose(result_fp);

    return test_passed;
}





int run_tests(char *output_path, int percorre) 
{
    char expeted_output[1000];
    snprintf(expeted_output, sizeof(expeted_output), "%s/command%d_output.txt", output_path, percorre);

    char result[1000];
    snprintf(result, sizeof(result), "Resultados/command%d_output.txt", percorre);

    // Comparar resultados
    int passed = compare_output(expeted_output, result);
    if (passed == 1) 
    {
        printf("Test passed\n");
        printf("-----------------------------------------------\n");
        return 1;
    } 

    return 0;
}


int parse_query_testes(char *output_path, FILE* input, void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void* todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats)  // tenho de lhe dar os saves
{
    int corretos = 0;
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
        clock_t begin = clock();

        escolhe_querie(0, querie, flags, percorre, allFlights, allUsers, allReservations, todosHoteis, todosAeroportos, userStats, flightStats, reservStats, passStats);

        clock_t end = clock();
        double query_time = (double)(end - begin)/CLOCKS_PER_SEC;
        printf("\nQuery %d executed in: %.6f sec\n", percorre, query_time);
        corretos += run_tests(output_path, percorre);

        for (int j = 0; j < i - 1; j++) {
            free(flags[j]);
        }

        percorre++;
        free(temp_buff);
        free(querie);
        free(flags);

    }

    return corretos;
}





int main (int argc, char* argv[]) 
{
    struct rusage r_usage;
    clock_t begin, end;
    double cpu_time_used;
    begin = clock();

    if (argc != 4) {
        printf("INVALID INPUT.\n");
        return 1;
    };

    void* usersStats = initStats();
    void* flightStats = initStats();
    void* reservStats = initStats();
    void* passStats = initStats();

    printf("Loading users...\n");
    char filepath_u[1000];
    snprintf(filepath_u, sizeof(filepath_u), "%s/users.csv", argv[1]);
    FILE *users = fopen(filepath_u, "r");
    if (users == NULL) {
        printf("ERROR: CAN'T OPEN FILE\n");
        return 1;
    }
    void *users_dados = parser(users, verificaUser, organizaUser, NULL, "users_errors.csv", "id;name;email;phone_number;birth_date;sex;passport;country_code;address;account_creation;pay_method;account_status", usersStats);
    fclose(users);

    getrusage(RUSAGE_SELF, &r_usage);
    printf("MEMORY USAGE: %ld KB\n", r_usage.ru_maxrss);

    printf("\nLoading flights...\n");
    char filepath_v[1000];
    snprintf(filepath_v, sizeof(filepath_v), "%s/flights.csv", argv[1]);
    FILE* voos = fopen(filepath_v, "r");
    if (voos == NULL) {
        printf("ERROR: CAN'T OPEN FILE\n");
        return 1;
    }
    void *voos_dados = parser(voos, verificaVoo, organizaVoo, NULL, "flights_errors.csv", "id;airline;plane_model;total_seats;origin;destination;schedule_departure_date;schedule_arrival_date;real_departure_date;real_arrival_date;pilot;copilot;notes", flightStats);
    fclose(voos);

    getrusage(RUSAGE_SELF, &r_usage);
    printf("MEMORY USAGE: %ld KB\n", r_usage.ru_maxrss);

    printf("\nLoading reservations...\n");
    char filepath_r[1000] ;
    snprintf(filepath_r, sizeof(filepath_r), "%s/reservations.csv", argv[1]);
    FILE *reservas = fopen(filepath_r, "r");
    if (reservas == NULL) {
        printf("ERROR: CAN'T OPEN FILE\n");
        return 1;
    }
    void *reservas_dados = parser(reservas, verificaReserva, organizaReserva, users_dados, "reservations_errors.csv", "id;user_id;hotel_id;hotel_name;hotel_stars;city_tax;address;begin_date;end_date;price_per_night;includes_breakfast;room_details;rating;comment", reservStats);
    fclose(reservas);

    getrusage(RUSAGE_SELF, &r_usage);
    printf("MEMORY USAGE: %ld KB\n", r_usage.ru_maxrss);

    printf("\nLoading passengers...\n");
    char filepath_p[1000];
    snprintf(filepath_p, sizeof(filepath_p), "%s/passengers.csv", argv[1]);
    FILE *passageiros = fopen(filepath_p, "r");
    if (passageiros == NULL) {
        printf("ERROR: CAN'T OPEN FILE\n");
        return 1;
    }
    parser_passageiros(passageiros, verificaPassageiros, users_dados, voos_dados, passStats);
    fclose(passageiros);

    getrusage(RUSAGE_SELF, &r_usage);
    printf("MEMORY USAGE: %ld KB\n", r_usage.ru_maxrss);

    end = clock();
    cpu_time_used = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("\nLOADING TIME: %.6f\n\n", cpu_time_used);

    insereReservas_User(users_dados, reservas_dados); //insere as reservas em q o user faz parte

    void *hoteisDadosMedia = insereMediaHoteis(reservas_dados); //percorre todas as reservas validas e insere os ratings numa struct

    void *aeroportos_dados = criaSaveAeroportos(voos_dados);
    calcularMedianaDelays(aeroportos_dados);

    ordenaNomes(users_dados);

    printf("\nTESTS:\n");
    FILE *input = fopen(argv[2], "r");
    if (input == NULL) {
        printf("ERROR: CAN'T OPEN FILE\n");
        return 1;
    }
    int testes_corretos = parse_query_testes(argv[3], input, voos_dados, users_dados, reservas_dados, hoteisDadosMedia, aeroportos_dados, usersStats, flightStats, reservStats, passStats);
    fclose(input);

    printf("\nTotal tests passed: %d\n", testes_corretos);
    
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
        
    
    clock_t time = clock();
    printf("\nELAPSED TIME: %.6f\n", (float)time/CLOCKS_PER_SEC);

    getrusage(RUSAGE_SELF, &r_usage);
    printf("TOTAL MEMORY USAGE: %ld KB\n", r_usage.ru_maxrss);

    return 0;
}