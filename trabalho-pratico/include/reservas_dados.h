#ifndef RESERVAS_DADOS_H
#define RESERVAS_DADOS_H

typedef struct reservation Reservation;

typedef struct save_reservations Save_reservations;

typedef struct hoteis Hoteis;

typedef struct save_hoteis Save_Hoteis;

void *verificaReserva(char** info, void* userdados, FILE* resultado, void* stats); //encapsulamento, metemos so este modulo ig

void *organizaReserva(void** array);

int procuraHotel (void *tabela, char* tag);

int procuraReservas(void *tabela, char* reserva);

void free_reservations_data(void* data);

void free_hoteis_data(void* save_hoteis);

char* getReservationId (void* reservas_dados, char* reservation);

void* getReservation(void* reservas_dados, char* chave);

char* getBreakfast_Reserv (void* reservas_dados);

char* getReservationHotelId_Reserv (void* reservas_dados);

char* getReservationHotelName_Reserv (void* reservas_dados);

int getReservationHotelStars_Reserv (void* reservas_dados);

char* getReservationBeginDate_Reserv (void* reservas_dados);

char* getReservationEndDate_Reserv (void* reservas_dados);

int getNnoites_Reserv (void* reservas_dados);

void* getHotel (void* hotel_dados, char* tag);

int getHotelTamanho (void* hotel_dados);

void* getHotelReserv (void* hotel_dados, int ind);

char* getReservationId_Reserv (void* reservas_dados);

char* getReservationUserId_Reserv (void* reservas_dados);

char* getReservationRating (void* reservas_dados);

char* getHotelEndDate (void* hotel_dados, int ind);

char* getHotelBeginDate (void* hotel_dados, int ind);

int getHotelPricePerNight (void* hotel_dados, int ind);

void *insereMediaHoteis (void* reservas);

#endif