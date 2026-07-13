/**
 * @file opciones.h
 * @author Dotta, Elian Agustin
 *
 * @brief Definicion del estado de opciones del juego.
 *
 * @version 1.0
 * @date 2026-03-05

 *
 */

#ifndef OPCIONES_H_INCLUDED
#define OPCIONES_H_INCLUDED

#include "estado.h"
#include "errores.h"

/**
 * @brief Enumeracion de los framebuffers del estado de opciones.
 *
 */
typedef enum {
    FB_OPTIONS_BACKGROUND,
    FB_OPTIONS_RECTANGLE,
    FB_OPTIONS_BUTTONS,
    FB_OPTIONS_TXT,
    FB_OPTIONS_HUD,
    FB_OPTIONS_INPUT,
    FB_OPTIONS_CANT,
}eFramebuffersOptions;

/**
 * @brief Enumeracion de los botones del estado de opciones.
 *
 */
typedef enum {
    OPTIONS_BUTTON_LASET,
    OPTIONS_BUTTON_RASET,
    OPTIONS_BUTTON_1P,
    OPTIONS_BUTTON_2P,
    OPTIONS_BUTTON_MENU,
    CANT_OPTIONS_BUTTON
}eOptionsButton;

/**
 * @brief Enumeracion de los textos de set de las opciones. Utilizados dentro del HUD para mostrar el set seleccionado.
 *
 */
typedef enum {
    TXT_OPTIONS_SET_BILLETES,
    TXT_OPTIONS_SET_ESCUDOS,
    TXT_OPTIONS_SET_COSTUMBRES,
    CANT_OPTIONS_SET_TXT,
}eOptionsSetTxt;

/**
 * @brief Enumeracion de los textos del estado de opciones.
 *
 */
typedef enum {
    TXT_OPTIONS_MENUPRIN,
    TXT_OPTIONS_P1,
    TXT_OPTIONS_P2,
    CANT_OPTIONS_TXT,
}eOptionsTxt;

/**
 * @brief Declaracion del estado de opciones. Definido en opciones.c
 *
 */
extern Estado estadoOpciones;

/**
 * @brief Incializa el estado de opciones, creando las estructuras necesarias para este estado.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 */
void abrirOpciones(tJuego* juego);


/**
 * @brief Maneja los eventos del juego segun el estado de opciones.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 */
void eventosOpciones(tJuego* juego);


/**
 * @brief Actualiza el contexto del juego segun el estado de opciones.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 */
void actualizarOpciones(tJuego* juego);


/**
 * @brief Renderiza el menu de opciones.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 */
void renderizarOpciones(tJuego* juego);


/**
 * @brief Destruye el estado de opciones, liberando los recursos utilizados por este estado.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 */
void cerrarOpciones(tJuego* juego);

/**
 * @brief Crea las texturas de los textos del estado de opciones y los almacena en el contexto del juego.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 * @return tError 0 o el codigo de error correspondiente al fallo.
 */
tError crear_texto_opt(tJuego* juego);

/**
 * @brief Crea los botones del estado de opciones y los almacena en el contexto del juego.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 */
void crear_botones_opt(tJuego* juego);

/**
 * @brief Crea las entradas de texto del estado de opciones y las almacena en el contexto del juego.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 * @return tError 0 o el codigo de error correspondiente al fallo.
 */
tError crear_entradas_opt(tJuego *juego);


/**
 * @brief Crea el HUD del estado de opciones y lo almacena en el contexto del juego.
 *
 * @param juego Referencia al juego, para acceder a sus variables y modificar el estado del mismo.
 * @return tError 0 o el codigo de error correspondiente al fallo.
 */
tError crear_hud_opt(tJuego *juego);


#endif // OPCIONES_H_INCLUDED
