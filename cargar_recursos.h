/**
 * @file cargar_recursos.h
 * @author Dotta, Elian Agustin
 * @brief Modularización de la carga de recursos del juego, como imágenes, audio y framebuffers.
 * @version 1.0
 * @date 2026-03-05
 */

#ifndef CARGAR_RECURSOS_H_INCLUDED
#define CARGAR_RECURSOS_H_INCLUDED

#include "errores.h"

// Declaración adelantada de tJuego para evitar dependencias circulares
typedef struct tJuego tJuego;

/**
 * @brief Carga todas las imágenes necesarias para el juego.
 * @param juego Puntero al contexto del juego donde se almacenarán las texturas cargadas.
 * @return TODO_OK si la carga fue exitosa, ERR_ARCHIVO u otro código de error si falló.
 */
tError cargar_imagenes(tJuego *juego);

/**
 * @brief Crea los framebuffers utilizados por los diferentes estados del juego.
 * @param juego Puntero al contexto del juego donde se almacenarán los framebuffers.
 * @return TODO_OK si la creación fue exitosa, ERR_MEMORIA u otro código de error si falló.
 */
tError crear_framebuffers(tJuego *juego);

/**
 * @brief Carga todos los recursos de audio del juego (efectos y música).
 * @param juego Puntero al contexto del juego donde se almacenarán los sonidos.
 * @return TODO_OK si la carga fue exitosa, ERR_ARCHIVO u otro código de error si falló.
 */
tError cargar_audio(tJuego *juego);

#endif // CARGAR_RECURSOS_H_INCLUDED