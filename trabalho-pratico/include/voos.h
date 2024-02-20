#ifndef VOOS_H
#define VOOS_H
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

typedef struct voo Voo;

typedef struct save_voos Save_voos;

typedef struct aeroportos Aeroportos;

typedef struct save_aeroportos Save_Aeroportos;

int isLocationValid(char* loc);

void* encontrarAeroportoArray(void* saveAeroportos, char* origin);

void *criaSaveAeroportos(void* allFlights);

void ordenaVooDecrescente(void* aeroporto);

void calcularMedianaDelays(void* saveAeroportos);

void ordenarPorMedianaDecrescente(void* saveAeroportos);

void ordenarPassageiros2023(void* saveAeroportos);

void ordenarPassageiros2022(void* saveAeroportos);

void ordenarPassageiros2021(void* saveAeroportos);

#endif