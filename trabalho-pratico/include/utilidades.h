#ifndef UTILIDADES_H_
#define UTILIDADES_H_

typedef struct mes Mes;

typedef struct ano Ano;

typedef struct stats Stats;

void addStats (char* data, void* stats, char* anoTag, void* users_dados, void* voos_dados);

int getTotalMesUnico (void* stats, char* tag, int mes);

int getTotalDias(void* stats, char* tag, int mes, int dia);

int getTotalDiasUnicos (void* stats, char* tag, int mes, int dia);

int getTotalAnoUnico (void* stats, char* tag);

int getTotalAno (void* stats, char* tag);

int getTotalMes (void* stats, char* tag, int mes);

void* initStats ();

void free_stats_hash (void* tabela);

#endif /* UTILIDADES_H_ */
