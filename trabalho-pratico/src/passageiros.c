#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/datas.h"
#include "../include/utilizadores.h"
#include "../include/voos.h"
#include "../include/passageiros.h"
#include "../include/utilidades.h"
#include "../include/utilizadores_dados.h"
#include "../include/voos_dados.h"


typedef struct passageiros {
    int flight_id;
    char* user_id;
} Passageiros;

//função que verifica se os passageiros são válidos
void verificaPassageiros(void* save_voos, void* users_dados, char **info, FILE* resultado, void* passStats) 
{  
    if (strlen(info[0]) >= 1 && strlen(info[1]) >= 1  && procuraVoo(save_voos, info[0]) == 1 && procuraUser(users_dados, info[1]) == 1) {
        setUserFlights(users_dados, info[1], save_voos, info[0]);
        addUserFlight(users_dados, info[1]);
        addFlightPassenger(save_voos, info[0]);
        void* voo = getFlight(save_voos, info[0]);
        addStats(getFlightSchDepa(voo), passStats, getFlightSchAno(voo), (getUser(users_dados, info[1])), save_voos);
        
    }
    else
    {
        int t = 2;
        for(int k = 0; k < t; k++)
        {
            if(info[k] == NULL) info[k] = strdup("");
        }
        

        fprintf(resultado, "%s", info[0]);

        for (int k = 1; k < t; k++) {
            fprintf(resultado, ";%s", info[k]);
        }

        fprintf(resultado, "\n");
    }
}

