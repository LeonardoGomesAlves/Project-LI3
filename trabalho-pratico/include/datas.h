#ifndef DATAS_H
#define DATAS_H
#include <stdio.h>

#define DATA_ATUAL 2023.836     // 2023/10/01

void dataToArray(char* str, int data[]);

char* dataGetAno(char* str);

int verifica_Data (char* data);

int verifica_Horas(char* data);

int verifica_DuasDatas(char* begin_date, char* end_date, int type);

int isDigitString (char* string);

int isEmpty(char* str);

int calculaIdades(char* str);

int comparaDatas(int data1[3], int data2[3]);

int gastoPorDatas (char* datainicio_Book, char* datafim_Book, char* inicio_d, char* fim_d, int per_night);

int verify_datas(char* begin_date, char* end_date);

int calculaNoites(char* begin_date, char* end_date);

int calculaDelays(char* schedule_date, char* real_date);

void adicionarHorasData (char* data, char* resultado);

void retirarHorasData (char* data, char* resultado);

int datasIguais(char* begin_date, char* end_date, int type);

char* preencheData(char* data, char* resultado);

#endif