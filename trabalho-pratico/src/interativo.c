#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "../include/parser_interativo.h"

//1º menu do programa, com a parte de inserir o path
int menu_inicial () 
{
    initscr();
    noecho();
    curs_set(0);
    start_color();
    flushinp();
    init_color(5, 900, 900, 0);
	  init_pair(1, 5, 0);
	  init_pair(2, 5, 0);

    WINDOW *opcoes = newwin(40, 131, 0, 0);
    box(opcoes, 0, 0);

    wattron(opcoes, COLOR_PAIR(1));

    wclear(opcoes);
    mvwprintw(opcoes, 4, 27,"  __  __           _         ___       _                 _   _            ");
    mvwprintw(opcoes, 5, 27," |  \\/  | ___   __| | ___   |_ _|_ __ | |_ ___ _ __ __ _| |_(_)_   _____  ");
    mvwprintw(opcoes, 6, 27," | |\\/| |/ _ \\ / _` |/ _ \\   | || '_ \\| __/ _ \\ '__/ _` | __| \\ \\ / / _ \\ ");
    mvwprintw(opcoes, 7, 27," | |  | | (_) | (_| | (_) |  | || | | | ||  __/ | | (_| | |_| |\\ V / (_) |");
    mvwprintw(opcoes, 8, 27," |_|  |_|\\___/ \\__,_|\\___/  |___|_| |_|\\__\\___|_|  \\__,_|\\__|_| \\_/ \\___/ ");
                                                                          
    wrefresh(opcoes);
    wattroff(opcoes, COLOR_PAIR(1));

    mvwprintw(opcoes, 18, 55, "Inserir path");
    mvwprintw(opcoes, 20, 59, "Sair");

    

    keypad(opcoes, TRUE);

    int key;
    int current = 0;

    while (key != '\n') {
      
      for (int i = 0; i < 2; i++) {
        if (i == current) {
            wattron(opcoes, A_BLINK | A_BOLD);
            wattron(opcoes, COLOR_PAIR(1));
        }
        switch (i) {
            case 0:
            mvwprintw(opcoes, 18, 55, "Inserir path");
            break;

            case 1:
            mvwprintw(opcoes, 20, 59, "Sair");
            break;

            default: 
            mvwprintw(opcoes, 18, 55, "Inserir path");
            mvwprintw(opcoes, 20, 59, "Sair");
            break;
        }
        wattroff(opcoes, A_BLINK | A_BOLD);
        wattroff(opcoes, COLOR_PAIR(1));
      }
      wrefresh(opcoes);

      key = wgetch(opcoes);

      if (key == KEY_UP) {
        current--;
        if (current < 0) current = 1;
      }
      else if (key == KEY_DOWN) {
        current++;
        if (current > 1) current = 0;
      }


    }

    endwin();
    echo();
    curs_set(1);
    return current;
}

//menu de inserir o path que faz a verificação se é válido ou não
char* menu_inserirpath () {
    initscr();
    curs_set(0);
    start_color();
    init_color(5, 900, 900, 0);
	  init_pair(1, 5, 0);
	  init_pair(2, 5, 0);
    char* path = malloc(sizeof(char)*1000);

    WINDOW *opcoes = newwin(40, 131, 0, 0);
    box(opcoes, 0, 0);

    wattron(opcoes, COLOR_PAIR(1));

    wclear(opcoes);
    mvwprintw(opcoes, 4, 27,"  __  __           _         ___       _                 _   _            ");
    mvwprintw(opcoes, 5, 27," |  \\/  | ___   __| | ___   |_ _|_ __ | |_ ___ _ __ __ _| |_(_)_   _____  ");
    mvwprintw(opcoes, 6, 27," | |\\/| |/ _ \\ / _` |/ _ \\   | || '_ \\| __/ _ \\ '__/ _` | __| \\ \\ / / _ \\ ");
    mvwprintw(opcoes, 7, 27," | |  | | (_) | (_| | (_) |  | || | | | ||  __/ | | (_| | |_| |\\ V / (_) |");
    mvwprintw(opcoes, 8, 27," |_|  |_|\\___/ \\__,_|\\___/  |___|_| |_|\\__\\___|_|  \\__,_|\\__|_| \\_/ \\___/ ");
                                                                          
    wrefresh(opcoes);
    wattroff(opcoes, COLOR_PAIR(1));

    for (int i = 0; 1; i++) {
      mvwprintw(opcoes, 18, 20, "Inserir a path dos datasets: ");
      wrefresh(opcoes);

      if (i>0) mvwprintw(opcoes, 19, 20, "Path errada, volte a inserir.");

      wmove(opcoes, 18, 49);
      wclrtoeol(opcoes);
      wrefresh(opcoes);

      mvwgetstr(opcoes, 18,49,path);

      char filepath_u[1000];
        snprintf(filepath_u, sizeof(filepath_u), "%s/users.csv", path);
        FILE *users = fopen(filepath_u, "r");
        if (users != NULL) {
            break;
        }
    }

    

    keypad(opcoes, TRUE);

    endwin();
    echo();
    curs_set(1);
    return path;
}

//menu principal do programa, já depois de inserir o path corretamente
int menu_principal () {
  initscr();
    noecho();
    curs_set(0);
    start_color();
    flushinp();
    init_color(5, 900, 900, 0);
	  init_pair(1, 5, 0);
	  init_pair(2, 5, 0);

    WINDOW *opcoes = newwin(40, 131, 0, 0);
    box(opcoes, 0, 0);

    wattron(opcoes, COLOR_PAIR(1));

    wclear(opcoes);
    mvwprintw(opcoes, 4, 27,"  __  __           _         ___       _                 _   _            ");
    mvwprintw(opcoes, 5, 27," |  \\/  | ___   __| | ___   |_ _|_ __ | |_ ___ _ __ __ _| |_(_)_   _____  ");
    mvwprintw(opcoes, 6, 27," | |\\/| |/ _ \\ / _` |/ _ \\   | || '_ \\| __/ _ \\ '__/ _` | __| \\ \\ / / _ \\ ");
    mvwprintw(opcoes, 7, 27," | |  | | (_) | (_| | (_) |  | || | | | ||  __/ | | (_| | |_| |\\ V / (_) |");
    mvwprintw(opcoes, 8, 27," |_|  |_|\\___/ \\__,_|\\___/  |___|_| |_|\\__\\___|_|  \\__,_|\\__|_| \\_/ \\___/ ");
                                                                          
    wrefresh(opcoes);
    wattroff(opcoes, COLOR_PAIR(1));

    mvwprintw(opcoes, 18, 55, "Menu queries");
    mvwprintw(opcoes, 20, 53, "Exemplos queries");
    mvwprintw(opcoes, 22, 59, "Sair");

    

    keypad(opcoes, TRUE);

    int key;
    int current = 0;

    while (key != '\n') {
              
      for (int i = 0; i < 3; i++) {
        if (i == current) {
            wattron(opcoes, A_BLINK | A_BOLD);
            wattron(opcoes, COLOR_PAIR(1));
        }
        switch (i) {
            case 0:
            mvwprintw(opcoes, 18, 55, "Menu queries");
            break;

            case 1:
            mvwprintw(opcoes, 20, 53, "Exemplos queries");
            break;

            case 2:
            mvwprintw(opcoes, 22, 59, "Sair");
            break;

            default: 
            mvwprintw(opcoes, 18, 55, "Menu queries");
            mvwprintw(opcoes, 20, 53, "Exemplos queries");
            mvwprintw(opcoes, 22, 59, "Sair");
            break;
        }
        wattroff(opcoes, A_BLINK | A_BOLD);
        wattroff(opcoes, COLOR_PAIR(1));
      }
      wrefresh(opcoes);

      key = wgetch(opcoes);

      if (key == KEY_UP) {
        current--;
        if (current < 0) current = 2;
      }
      else if (key == KEY_DOWN) {
        current++;
        if (current > 2) current = 0;
      }


    }

    endwin();
    echo();
    curs_set(1);
    if (current == 2) return 70; //serve para dar exit ao programa no ciclo
    return current;
}

//tela de loading das estruturas de dados
void loading(WINDOW* janela, char* output) {
  wclear(janela);
  box(janela, 0,0);
  mvwprintw(janela, 1, 1, "%s", output);
  wrefresh(janela);
}

//tela de cada query do menu de exemplos, responsavel por mostrar os exemplos
void print_exemplos (int type) {
    initscr();
    noecho();
    curs_set(0);
    start_color();
    flushinp();
    init_color(5, 900, 900, 0);
	  init_pair(1, 5, 0);
	  init_pair(2, 5, 0);

    WINDOW *query = newwin(40, 131, 0, 0);
    box(query, 0, 0);

    wattron(query, COLOR_PAIR(1));

    wclear(query);                                                                          
    wrefresh(query);
    refresh();
    wattroff(query, COLOR_PAIR(1));

    int key;
    while (key != 'q') 
    {
      switch (type) 
      {
        case 0: //query 1
        mvwprintw(query, 3, 10, "Exemplo de um utilizador:");
        mvwprintw(query, 4, 10, "Input: 1(F) JéssiTavares910");
        WINDOW* ex1 = newwin(13, 115, 5, 10);
        box(ex1, 0, 0);
        mvwprintw(ex1, 1, 2, "Formato normal:");
        mvwprintw(ex1, 5, 2, "Jéssica Tavares;F;63;PT;ZE466021;11;5;2365.200");
        mvwprintw(ex1, 1, 70, "Formato FIELD(F):");
        mvwprintw(ex1, 3, 70, "--- 1 ---");
        mvwprintw(ex1, 4, 70, "name: Jéssica Tavares");
        mvwprintw(ex1, 5, 70, "sex: F");
        mvwprintw(ex1, 6, 70, "age: 63");
        mvwprintw(ex1, 7, 70, "country_code: PT");
        mvwprintw(ex1, 8, 70, "passport: ZE466021");
        mvwprintw(ex1, 9, 70, "number_of_flights: 11");
        mvwprintw(ex1, 10, 70, "number_of_reservations: 5");
        mvwprintw(ex1, 11, 70, "total_spent: 2365.200");

        mvwprintw(query, 20, 10, "Exemplo de uma reserva:");
        mvwprintw(query, 21, 10, "Input: 1(F) Book0000000048");
        WINDOW* ex2 = newwin(13, 115, 22, 10);
        box(ex2, 0, 0);
        mvwprintw(ex2, 1, 2, "Formato normal:");
        mvwprintw(ex2, 5, 2, "HTL904;Chiado Plaza Hotel;3;2023/09/01;2023/09/03;False;2;306.000");
        mvwprintw(ex2, 1, 70, "Formato FIELD(F):");
        mvwprintw(ex2, 3, 70, "--- 1 ---");
        mvwprintw(ex2, 4, 70, "hotel_id: HTL904");
        mvwprintw(ex2, 5, 70, "hotel_name: Chiado Plaza Hotel");
        mvwprintw(ex2, 6, 70, "hotel_stars: 3");
        mvwprintw(ex2, 7, 70, "begin_date: 2023/09/01");
        mvwprintw(ex2, 8, 70, "end_date: 2023/09/03");
        mvwprintw(ex2, 9, 70, "includes_breakfast: False");
        mvwprintw(ex2, 10, 70, "nights: 2");
        mvwprintw(ex2, 11, 70, "total_price: 306.000");

        mvwprintw(query, 36, 10, "O input '1(F) 0000000946' também seria possível e apresentaria iformações relativas ao voo 0000000946.");

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex1);
        wrefresh(ex2);
        break;

        case 1: //query 2
        mvwprintw(query, 3, 10, "Exemplo de voos:");
        mvwprintw(query, 4, 10, "Input: 2(F) JéssiTavares910 flights");
        WINDOW* ex3 = newwin(12, 115, 5, 10);
        box(ex3, 0, 0);
        mvwprintw(ex3, 1, 2, "Formato normal:");
        mvwprintw(ex3, 3, 2, "0000000922;2023/07/19");
        mvwprintw(ex3, 4, 2, "0000000796;2023/06/11");
        mvwprintw(ex3, 5, 2, "0000000163;2023/02/10");
        mvwprintw(ex3, 6, 2, "0000000582;2022/12/20");
        mvwprintw(ex3, 7, 2, "0000000115;2022/10/21");
        mvwprintw(ex3, 8, 2, "0000000447;2022/10/11");
        mvwprintw(ex3, 9, 2, "...");
        mvwprintw(ex3, 1, 65, "Formato FIELD(F):");
        mvwprintw(ex3, 3, 65, "--- 1 ---");
        mvwprintw(ex3, 4, 65, "id: 0000000922");
        mvwprintw(ex3, 5, 65, "date: 2023/07/19");
        mvwprintw(ex3, 7, 65, "--- 2 ---");
        mvwprintw(ex3, 8, 65, "id: 0000000796");
        mvwprintw(ex3, 9, 65, "date: 2023/06/11");
        mvwprintw(ex3, 10, 65, "...");

        mvwprintw(query, 18, 10, "Exemplo de voos e reservas:");
        mvwprintw(query, 19, 10, "Input: 1(F) Book0000000048");
        WINDOW* ex4 = newwin(14, 115, 20, 10);
        box(ex4, 0, 0);
        mvwprintw(ex4, 1, 2, "Formato normal:");
        mvwprintw(ex4, 3, 2, "0000000922;2023/07/19;flight");
        mvwprintw(ex4, 4, 2, "Book0000037981;2023/07/19;reservation");
        mvwprintw(ex4, 5, 2, "0000000796;2023/06/11;flight");
        mvwprintw(ex4, 6, 2, "0000000163;2023/02/10;flight");
        mvwprintw(ex4, 7, 2, "Book0000006454;2023/02/10;reservation");
        mvwprintw(ex4, 8, 2, "0000000582;2022/12/20;flight");  
        mvwprintw(ex4, 9, 2, "...");      
        mvwprintw(ex4, 1, 65, "Formato FIELD(F):");
        mvwprintw(ex4, 3, 65, "--- 1 ---");
        mvwprintw(ex4, 4, 65, "id: 0000000922");
        mvwprintw(ex4, 5, 65, "date: 2023/07/19");
        mvwprintw(ex4, 6, 65, "type: flight");
        mvwprintw(ex4, 8, 65, "--- 2 ---");
        mvwprintw(ex4, 9, 65, "id: Book0000037981");
        mvwprintw(ex4, 10, 65, "date: 2023/07/19");
        mvwprintw(ex4, 11, 65, "type: reservation");
        mvwprintw(ex4, 12, 65, "...");

        mvwprintw(query, 35, 10, "O input '2(F) JéssiTavares910 reservations' também seria possível e aprentaria todas as reservas do");
        mvwprintw(query, 36, 10, "utilizador JéssiTavares910.");

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex3);
        wrefresh(ex4);
        break;

        case 2: //query 3
        mvwprintw(query, 3, 10, "Exemplo:");
        mvwprintw(query, 4, 10, "Input: 3(F) HTL1001");
        WINDOW* ex5 = newwin(12, 115, 5, 10);
        box(ex5, 0, 0);
        mvwprintw(ex5, 1, 2, "Formato normal:");
        mvwprintw(ex5, 5, 2, "2.487");   
        mvwprintw(ex5, 1, 65, "Formato FIELD(F):");
        mvwprintw(ex5, 4, 65, "--- 1 ---");
        mvwprintw(ex5, 5, 65, "rating: 2.487");

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex5);
        break;

        case 3://query 4
        mvwprintw(query, 3, 10, "Exemplo:");
        mvwprintw(query, 4, 10, "Input: 4(F) HTL1001");
        WINDOW* ex6 = newwin(20, 115, 5, 10);
        box(ex6, 0, 0);
        mvwprintw(ex6, 1, 2, "Formato normal:");
        mvwprintw(ex6, 7, 2, "Book0000030197;2023/07/21;2023/07/24;GabrieGomes;4;673.200");
        mvwprintw(ex6, 8, 2, "Book0000030199;2023/07/21;2023/07/23;MartRibeiro1463;1;448.800");
        mvwprintw(ex6, 9, 2, "Book0000030217;2023/07/21;2023/07/22;AdriRamos278;2;224.400");
        mvwprintw(ex6, 10, 2, "Book0000027509;2023/07/18;2023/07/20;IMatos1421;2;448.800");
        mvwprintw(ex6, 11, 2, "Book0000027516;2023/07/18;2023/07/19;RúbSá1404;2;224.400");
        mvwprintw(ex6, 12, 2, "Book0000027521;2023/07/18;2023/07/22;GaOliveira1587;1;897.600");        
        mvwprintw(ex6, 13, 2, "...");
        mvwprintw(ex6, 1, 70, "Formato FIELD(F):");
        mvwprintw(ex6, 3, 70, " --- 1 ---");
        mvwprintw(ex6, 4, 70, "id: Book0000030197");
        mvwprintw(ex6, 5, 70, "begin_date: 2023/07/21");
        mvwprintw(ex6, 6, 70, "end_date: 2023/07/24");
        mvwprintw(ex6, 7, 70, "user_id: GabrieGomes");
        mvwprintw(ex6, 8, 70, "rating: 4");
        mvwprintw(ex6, 9, 70, "total_price: 673.200");
        mvwprintw(ex6, 11, 70, "--- 2 ---");
        mvwprintw(ex6, 12, 70, "id: Book0000030199");
        mvwprintw(ex6, 13, 70, "begin_date: 2023/07/21");
        mvwprintw(ex6, 14, 70, "end_date: 2023/07/23");
        mvwprintw(ex6, 15, 70, "user_id: MartRibeiro1463");
        mvwprintw(ex6, 16, 70, "rating: 1");
        mvwprintw(ex6, 17, 70, "total_price: 448.800");
        mvwprintw(ex6, 18, 70, "..."); 

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex6);
        break;

        case 4://query 5
        mvwprintw(query, 3, 10, "Exemplo:");
        mvwprintw(query, 4, 10, "Input: 5(F) LIS '2021/01/01 00:00:00' '2022/12/31 23:59:59'");
        WINDOW* ex7 = newwin(18, 115, 5, 10);
        box(ex7, 0, 0);
        mvwprintw(ex7, 1, 2, "Formato normal:");
        mvwprintw(ex7, 5, 2, "0000000017;2022/12/03 17:54:17;MAD;TAP Air Portugal;Airbus A320");
        mvwprintw(ex7, 6, 2, "0000000799;2022/11/13 12:43:46;MAD;TAP Air Portugal;Airbus A320");
        mvwprintw(ex7, 7, 2, "0000000304;2022/11/12 02:18:28;MAD;TAP Air Portugal;Airbus A320");
        mvwprintw(ex7, 8, 2, "0000000702;2022/10/18 07:44:22;MAD;TAP Air Portugal;Airbus A320");
        mvwprintw(ex7, 9, 2, "0000000029;2022/10/11 15:18:59;MAD;TAP Air Portugal;Airbus A320");
        mvwprintw(ex7, 10, 2, "0000000593;2022/09/14 05:27:11;MAD;TAP Air Portugal;Airbus A320");      
        mvwprintw(ex7, 11, 2, "...");
        mvwprintw(ex7, 1, 68, "Formato FIELD(F):");
        mvwprintw(ex7, 3, 68, "--- 1 ---");
        mvwprintw(ex7, 4, 68, "id: 0000000017");
        mvwprintw(ex7, 5, 68, "schedule_departure_date: 2022/12/03 17:54:17");
        mvwprintw(ex7, 6, 68, "destination: MAD");
        mvwprintw(ex7, 7, 68, "airline: TAP Air Portugal");
        mvwprintw(ex7, 8, 68, "plane_model: Airbus A320");
        mvwprintw(ex7, 10, 68, "--- 2 ---");
        mvwprintw(ex7, 11, 68, "id: 0000000799");
        mvwprintw(ex7, 12, 68, "schedule_departure_date: 2022/11/13 12:43:46");
        mvwprintw(ex7, 13, 68, "destination: MAD");
        mvwprintw(ex7, 14, 68, "airline: TAP Air Portugal");
        mvwprintw(ex7, 15, 68, "plane_model: Airbus A320");
        mvwprintw(ex7, 16, 68, "...");

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex7);
        break;

        case 5://query 6        Lista o top N aeroportos com mais passageiros, para um dado ano.
        mvwprintw(query, 3, 10, "Exemplo:");
        mvwprintw(query, 4, 10, "Input: 6(F) 2023 10");
        WINDOW* ex8 = newwin(12, 115, 5, 10);
        box(ex8, 0, 0);
        mvwprintw(ex8, 1, 2, "Formato normal:");
        mvwprintw(ex8, 3, 2, "MAD;3841");
        mvwprintw(ex8, 4, 2, "AMS;2969");
        mvwprintw(ex8, 5, 2, "FCO;2661");
        mvwprintw(ex8, 6, 2, "LIS;2349");
        mvwprintw(ex8, 7, 2, "DUB;2310");    
        mvwprintw(ex8, 8, 2, "...");
        mvwprintw(ex8, 1, 68, "Formato FIELD(F):");
        mvwprintw(ex8, 3, 68, "--- 1 ---");
        mvwprintw(ex8, 4, 68, "name: MAD");
        mvwprintw(ex8, 5, 68, "passengers: 3841");
        mvwprintw(ex8, 7, 68, "--- 2 ---");
        mvwprintw(ex8, 8, 68, "name: AMS");
        mvwprintw(ex8, 9, 68, "passengers: 2969");
        mvwprintw(ex8, 10, 68, "...");

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex8);
        break;

        case 6://query 7        Lista o top N aeroportos com maior mediana de atrasos.
        mvwprintw(query, 3, 10, "Exemplo:");
        mvwprintw(query, 4, 10, "Input: 7(F) 10");
        WINDOW* ex9 = newwin(12, 115, 5, 10);
        box(ex9, 0, 0);
        mvwprintw(ex9, 1, 2, "Formato normal:");
        mvwprintw(ex9, 3, 2, "IST;900");
        mvwprintw(ex9, 4, 2, "LIS;900");
        mvwprintw(ex9, 5, 2, "VIE;900");
        mvwprintw(ex9, 6, 2, "WAW;450");
        mvwprintw(ex9, 7, 2, "ARN;300");    
        mvwprintw(ex9, 8, 2, "...");
        mvwprintw(ex9, 1, 68, "Formato FIELD(F):");
        mvwprintw(ex9, 3, 68, "--- 1 ---");
        mvwprintw(ex9, 4, 68, "name: IST");
        mvwprintw(ex9, 5, 68, "median: 900");
        mvwprintw(ex9, 7, 68, "--- 2 ---");
        mvwprintw(ex9, 8, 68, "name: LIS");
        mvwprintw(ex9, 9, 68, "median: 900");
        mvwprintw(ex9, 10, 68, "...");

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex9);
        break;

        case 7://query 8        Apresenta a receita total de um hotel entre duas datas(inclusive).
        mvwprintw(query, 3, 10, "Exemplo:");
        mvwprintw(query, 4, 10, "Input: 8(F) HTL1001 2023/05/02 2023/05/02");
        WINDOW* ex10 = newwin(12, 115, 5, 10);
        box(ex10, 0, 0);
        mvwprintw(ex10, 1, 2, "Formato normal:");
        mvwprintw(ex10, 5, 2, "2640");   
        mvwprintw(ex10, 1, 65, "Formato FIELD(F):");
        mvwprintw(ex10, 4, 65, "--- 1 ---");
        mvwprintw(ex10, 5, 65, "revenue: 2640");

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex10);
        break;

        case 8://query 9        Lista todos os utilizadores cujo nome começa com um dado prefixo.
        mvwprintw(query, 3, 10, "Exemplo:");
        mvwprintw(query, 4, 10, "Input: 9(F) Julia");
        WINDOW* ex11 = newwin(12, 115, 5, 10);
        box(ex11, 0, 0);
        mvwprintw(ex11, 1, 2, "Formato normal:");
        mvwprintw(ex11, 3, 2, "JulAlves1633;Juliana Alves");
        mvwprintw(ex11, 4, 2, "JAndrade;Juliana Andrade");
        mvwprintw(ex11, 5, 2, "JulAnjos;Juliana Anjos");
        mvwprintw(ex11, 6, 2, "JulAntunes931;Juliana Antunes");
        mvwprintw(ex11, 7, 2, "JBatista;Juliana Batista");    
        mvwprintw(ex11, 8, 2, "...");
        mvwprintw(ex11, 1, 68, "Formato FIELD(F):");
        mvwprintw(ex11, 3, 68, "--- 1 ---");
        mvwprintw(ex11, 4, 68, "id: JulAlves1633");
        mvwprintw(ex11, 5, 68, "name: Juliana Alves");
        mvwprintw(ex11, 7, 68, "--- 2 ---");
        mvwprintw(ex11, 8, 68, "id: JAndrade");
        mvwprintw(ex11, 9, 68, "name: Juliana Andrade");
        mvwprintw(ex11, 10, 68, "...");

        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex11);
        break;

        case 9://query 10       Apresenta várias métricas gerais da aplicação.
        mvwprintw(query, 1, 10, "Exemplo 1:");
        mvwprintw(query, 2, 10, "Input: 10(F)");
        WINDOW* ex12 = newwin(12, 115, 3, 10);
        box(ex12, 0, 0);
        mvwprintw(ex12, 1, 2, "Formato normal:");
        mvwprintw(ex12, 3, 2, "2010;905;0;0;0;0");
        mvwprintw(ex12, 4, 2, "2011;857;0;0;0;0");
        mvwprintw(ex12, 5, 2, "2012;878;0;0;0;0");
        mvwprintw(ex12, 6, 2, "2013;833;0;0;0;0");
        mvwprintw(ex12, 7, 2, "2014;851;0;0;0;0");
        mvwprintw(ex12, 8, 2, "2015;859;0;0;0;0");
        mvwprintw(ex12, 9, 60, "...");
        mvwprintw(ex12, 1, 60, "Formato FIELD(F):");
        mvwprintw(ex12, 3, 60, "--- 1 ---");
        mvwprintw(ex12, 4, 60, "year: 2010");
        mvwprintw(ex12, 5, 60, "users: 905");
        mvwprintw(ex12, 6, 60, "flights: 0");
        mvwprintw(ex12, 7, 60, "passengers: 0");
        mvwprintw(ex12, 8, 60, "unique_passengers: 0");
        mvwprintw(ex12, 9, 60, "reservations: 0");
        mvwprintw(ex12, 10, 60, "...");

        mvwprintw(query, 16, 10, "Exemplo 2:");
        mvwprintw(query, 17, 10, "Input: 10 2023(F)");
        WINDOW* ex13 = newwin(12, 115, 18, 10);
        box(ex13, 0, 0);
        mvwprintw(ex13, 1, 2, "Formato normal:");
        mvwprintw(ex13, 3, 2, "1;0;37;2867;2487;1512");
        mvwprintw(ex13, 4, 2, "2;0;25;1919;1741;1067");
        mvwprintw(ex13, 5, 2, "3;0;30;2247;2011;1154");
        mvwprintw(ex13, 6, 2, "4;0;29;2158;1952;1029");
        mvwprintw(ex13, 7, 2, "5;0;32;2592;2305;1364");
        mvwprintw(ex13, 8, 2, "6;0;39;3009;2590;1501");
        mvwprintw(ex13, 9, 60, "...");
        mvwprintw(ex13, 1, 60, "Formato FIELD(F):");
        mvwprintw(ex13, 3, 60, "--- 1 ---");
        mvwprintw(ex13, 4, 60, "month: 1");
        mvwprintw(ex13, 5, 60, "users: 0");
        mvwprintw(ex13, 6, 60, "flights: 37");
        mvwprintw(ex13, 7, 60, "passengers: 2867");
        mvwprintw(ex13, 8, 60, "unique_passengers: 2487");
        mvwprintw(ex13, 9, 60, "reservations: 1512");
        mvwprintw(ex13, 10, 60, "...");

        mvwprintw(query, 31, 10, "O input '10(F) 2022 03' também seria possível e apresentaria os dados do ano 2022 e do mês 3 agregados por dia.");
      
        mvwprintw(query, 39, 38, "Pressione a tecla 'q' para voltar atrás.");
        wrefresh(ex12);
        wrefresh(ex13); 
        break;

      }
      wrefresh(query);
      key = wgetch(query);
    }  
    endwin();
}

//menu dos exemplos das queries
int exemplos_queries () {

    initscr();
    noecho();
    curs_set(0);
    start_color();
    flushinp();
    init_color(5, 900, 900, 0);
	  init_pair(1, 5, 0);
	  init_pair(2, 5, 0);

    WINDOW *exemplo = newwin(40, 131, 0, 0);
    box(exemplo, 0, 0);

    wattron(exemplo, COLOR_PAIR(1));

    wclear(exemplo);                                                                          
    wrefresh(exemplo);
    wattroff(exemplo, COLOR_PAIR(1));

    mvwprintw(exemplo, 6, 40, "Lista um resumo de um utilizador, voo ou reserva.");
    mvwprintw(exemplo, 8, 40, "Lista os voos e/ou reservas de um utilizador.");
    mvwprintw(exemplo, 10, 40, "Apresenta a classificação média de um hotel.");
    mvwprintw(exemplo, 12, 40, "Lista as reservas de um hotel, ordenadas por data de início.");
    mvwprintw(exemplo, 14, 40, "Lista os voos com origem num aeroporto, entre duas datas.");
    mvwprintw(exemplo, 16, 40, "Lista o top N aeroportos com mais passageiros, para um dado ano.");
    mvwprintw(exemplo, 18, 40, "Lista o top N aeroportos com maior mediana de atrasos.");
    mvwprintw(exemplo, 20, 40, "Apresenta a receita total de um hotel entre duas datas(inclusive).");
    mvwprintw(exemplo, 22, 40, "Lista todos os utilizadores cujo nome começa com um dado prefixo.");
    mvwprintw(exemplo, 24, 40, "Apresenta várias métricas gerais da aplicação.");
    mvwprintw(exemplo, 27, 50, "Voltar");

    

    keypad(exemplo, TRUE);

    int key;
    int current = 0;
    int i;
      while (key != '\n') {
                  
        for (i = 0; i < 11; i++) {
          if (i == current) {
              wattron(exemplo, A_BLINK | A_BOLD);
              wattron(exemplo, COLOR_PAIR(1));
          }
          switch (i) {
              case 0:
              mvwprintw(exemplo, 6, 15, "Query 1");
              break;

              case 1:
              mvwprintw(exemplo, 8, 15, "Query 2");
              break;

              case 2:
              mvwprintw(exemplo, 10, 15, "Query 3");
              break;

              case 3:
              mvwprintw(exemplo, 12, 15, "Query 4");
              break;

              case 4:
              mvwprintw(exemplo, 14, 15, "Query 5");
              break;

              case 5:
              mvwprintw(exemplo, 16, 15, "Query 6");
              break;

              case 6:
              mvwprintw(exemplo, 18, 15, "Query 7");
              break;

              case 7:
              mvwprintw(exemplo, 20, 15, "Query 8");
              break;

              case 8:
              mvwprintw(exemplo, 22, 15, "Query 9");
              break;

              case 9:
              mvwprintw(exemplo, 24, 15, "Query 10");
              break;

              case 10:
              mvwprintw(exemplo, 27, 50, "Voltar");
              break;


              default: 
              mvwprintw(exemplo, 6, 15, "Query 1");
              mvwprintw(exemplo, 8, 15, "Query 2");
              mvwprintw(exemplo, 10, 15, "Query 3");
              mvwprintw(exemplo, 12, 15, "Query 4");
              mvwprintw(exemplo, 14, 15, "Query 5");
              mvwprintw(exemplo, 16, 15, "Query 6");
              mvwprintw(exemplo, 18, 15, "Query 7");
              mvwprintw(exemplo, 20, 15, "Query 8");
              mvwprintw(exemplo, 22, 15, "Query 9");
              mvwprintw(exemplo, 24, 15, "Query 10");
              mvwprintw(exemplo, 27, 50, "Voltar");
              break;
          }
          wattroff(exemplo, A_BLINK | A_BOLD);
          wattroff(exemplo, COLOR_PAIR(1));
        }
        wrefresh(exemplo);

        key = wgetch(exemplo);

        if (key == KEY_UP) {
          current--;
          if (current < 0) current = 10;
        }
        else if (key == KEY_DOWN) {
          current++;
          if (current > 10) current = 0;
        }


      }
      if (current != 10) {
        printf("%d\n", current);
        print_exemplos(current);
      }

    endwin();
    echo();
    curs_set(1);
    
    
    return current;
}

//menu das queries, responsavel por pedir ao utilizador o input
void menu_queries(void *allFlights, void *allUsers, void *allReservations, void* todosHoteis, void* todosAeroportos, void* userStats, void* flightStats, void* reservStats, void* passStats)
{
  initscr();
  curs_set(0);
  start_color();
  init_color(5, 900, 900, 0);
	init_pair(1, 5, 0);
	init_pair(2, 5, 0);
  char* query = malloc(sizeof(char)*1000);

  WINDOW *input = newwin(40, 131, 0, 0);
  box(input, 0, 0);

  wattron(input, COLOR_PAIR(1));

  wclear(input);
  mvwprintw(input, 4, 27,"  __  __           _         ___       _                 _   _            ");
  mvwprintw(input, 5, 27," |  \\/  | ___   __| | ___   |_ _|_ __ | |_ ___ _ __ __ _| |_(_)_   _____  ");
  mvwprintw(input, 6, 27," | |\\/| |/ _ \\ / _` |/ _ \\   | || '_ \\| __/ _ \\ '__/ _` | __| \\ \\ / / _ \\ ");
  mvwprintw(input, 7, 27," | |  | | (_) | (_| | (_) |  | || | | | ||  __/ | | (_| | |_| |\\ V / (_) |");
  mvwprintw(input, 8, 27," |_|  |_|\\___/ \\__,_|\\___/  |___|_| |_|\\__\\___|_|  \\__,_|\\__|_| \\_/ \\___/ ");
                                                                          
  wrefresh(input);
  wattroff(input, COLOR_PAIR(1));
  
  wrefresh(input);
  while(1)
  {
    mvwprintw(input, 18, 20, "Insira a query desejada:");
    mvwprintw(input, 36, 40, "Digite 'back' para voltar atrás.");
    wrefresh(input);
    wmove(input, 18, 46);
    wclrtoeol(input);
    wrefresh(input);
    mvwgetstr(input, 18, 46, query);

    keypad(input, TRUE);

    int loop;
    loop = parse_query_Interativo(query, allFlights, allUsers, allReservations, todosHoteis, todosAeroportos, userStats, flightStats, reservStats, passStats);

    print_erros(input, loop);
    
    if(loop == 0) break;
  }

  endwin();
  echo();
  curs_set(1);
    
}