#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include "config.h"
#include <stdint.h>

typedef enum {
    Jugador_1,
    Jugador_2,
    CANT_JUGADORES,
}eJugadores;

typedef struct{
    unsigned int id_jugador;
    char nombre[LARGO_NOMBRE];
    unsigned int puntaje;
    unsigned int cont_racha;
}Jugador;

typedef struct{
    int puntaje;
    int indiceTurno;
}contextoPuntaje;

void jugadorInicializar(Jugador* p, int id, const char* nombre);

void cambiarTurno(uint32_t *indice, int cant, int cantJugadores);

void actualizarRachaPuntaje(void* a, void* b);

#endif // JUGADOR_H_INCLUDED
