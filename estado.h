/**
 * @file estado.h
 * @author Dotta, Elian Agustin
 * @brief Estructura para definir los estados del juego
 * 
 * @version 1.0
 * @date 2026-03-05
 * 
 */
#ifndef ESTADO_H_INCLUDED
#define ESTADO_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

// Declaracion adelantada de tJuego para evitar dependencias circulares entre los estados y el juego
typedef struct tJuego tJuego;

/**
 * @brief Estructura que define un estado del juego, con punteros a funciones para manejar cada etapa del ciclo de vida de cada estado (abrir, procesar eventos, actualizar, renderizar y cerrar).
 * 
 */
typedef struct
{
    void (*abrir) (tJuego *juego); // Puntero a funcion para abrir el estado, cargar los recursos necesarios para este estado y preparar el juego para este
    void (*evento) (tJuego *juego); // Puntero a funcion para procesar los eventos del estado, como el teclado, mouse y eventos de ventana
    void (*actualizar) (tJuego *juego); // Puntero a funcion para actualizar el estado del juego y sus elementos, como la logica del juego, animaciones, etc
    void (*renderizar) (tJuego *juego); // Puntero a funcion para renderizar el estado del juego, dibujando los elementos en pantalla
    void (*cerrar) (tJuego *juego); // Puntero a funcion para cerrar el estado, liberando los recursos utilizados por este estado
} Estado;


#endif // ESTADO_H_INCLUDED

