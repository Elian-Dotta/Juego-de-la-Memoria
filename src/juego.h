/**
 * @file juego.h
 * @author Dotta, Elian Agustin
 * @brief Este modulo define la estructura central del juego 'tJuego'
 *  integra los demas modulos y delega la funcionalidad a los diferentes estados del juego
 *  cambiando el accionar dependiendo a que estado se apunte
 *
 * @version 1.0
 * @date 2026-03-06
 */

#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "config.h"
#include "errores.h"
#include "estado.h"
#include "texto.h"
#include "hud.h"
#include "imagenes.h"
#include "button.h"
#include "carta.h"
#include "opciones.h"
#include "jugador.h"
#include "inputTxt.h"
#include "sonidos.h"
#include "gameplay.h"
#include "menu.h"

/**
 * @brief Enumeracion con los sonidos del juego
 */
typedef enum {
    SND_VICTORIA,
    SND_ACIERTO_BILLETE,
    SND_ACIERTO_GENERICO,
    SND_INCORRECTO,
    SND_CAMBIAR_CARTA,
    SND_BOTON,
    SND_MUSICA,
    CANT_SND,
}eSonido;

/**
 * @brief Enumeracion de los fondos del array de texturas 'background'
 */
typedef enum {
    MENU_BACKGROUND,
    GAME_BACKGROUND,
    OPTIONS_BACKGROUND,
    PAUSE_BACKGROUND,
    CANT_BACKGROUND,
}eBackground;

/**
 * @brief Enumeracion de texturas de botones del array de texturas 'button'
 */
typedef enum {
    MENU_BUTTON,
    OPTIONS_ARROW_BUTTON,
    OPTIONS_SQUARE_BUTTON,
    CANT_BUTTON,
}eButton;

/**
 * @brief Estructura central del juego, contiene todas las variables necesarias para su funcionamiento, asi como el contexto del mismo.
 *
 * @note Esta estructura se pasa como parametro a las funciones de los diferentes estados del juego para que puedan modificar el contexto del mismo y generar la logica deseada
 *
 */
typedef struct tJuego{
    SDL_Window *ventana;                                // Puntero a la ventana
    SDL_Renderer *renderer;                             // Puntero al renderizador de SDL
    SDL_Texture *eFramebuffersMenu[FB_MENU_CANT];       // Array de framebuffers/capas para la composicion del menu principal
    SDL_Texture *eFramebuffersGame[FB_GAME_CANT];       // Array de framebuffers/capas para la composicion del juego
    SDL_Texture *eFramebuffersOptions[FB_OPTIONS_CANT]; // Array de framebuffers/capas para la composicion del menu opciones
    SDL_Texture *eFramebuffersPause[FB_PAUSE_CANT];     // Array de framebuffers/capas para la composicion del menu de pausa
    SDL_Texture *background[CANT_BACKGROUND];           // Array de texturas utilizadas como fondos de todos los menus
    SDL_Texture *button[CANT_BUTTON];                   // Array de texturas de botones de uso general
    SDL_Texture *buttong;                               // Textura de boton de pausa del juego
    SDL_Texture *optionsRectangle;                      // Textura de un rectangulo utilizado en opciones
    SDL_Texture *indicadorPlayer[CANT_GAME_IND];        // Array de texturas utilizadas como indicador de los jugadores en el juego
    SDL_Texture *SET1Texture[CARTAS_CANT];              // Array de texturas en GPU de las imagenes de las cartas del set 1
    SDL_Texture *SET2Texture[CARTAS_CANT];              // Array de texturas en GPU de las imagenes de las cartas del set 1
    SDL_Texture *SET3Texture[CARTAS_CANT];              // Array de texturas en GPU de las imagenes de las cartas del set 1
    SDL_Surface *icono;                                 // Superficie en RAM utilizada como icono de la ventana
    tTexto textoMenu[CANT_MENU_TXT];                    // Array de textos generados con TTF para textos del menu principal
    tTexto textoPause[CANT_PAUSE_TXT];                  // Array de textos generados con TTF para textos del menu pausa
    tTexto textoGame[CANT_GAME_TXT];                    // Array de textos generados con TTF para textos del juego
    tTexto textoGamePlayerName[CANT_GAME_NAME_TXT];     // Array de textos generados con TTF para nombres de jugadores en el juego
    tTexto textoOptionsSet[CANT_OPTIONS_SET_TXT];       // Array de textos generados con TTF para textos del set en el menu opciones
    tTexto textoOptions[CANT_OPTIONS_TXT];              // Array de textos generados con TTF para textos del menu opciones
    Button menuButtons[CANT_MENU_BUTTON];               // Array de botones del menu principal
    Button pauseButtons[CANT_PAUSE_BUTTON];             // Array de botones del menu pausa
    Button gameButtons[CANT_GAME_BUTTON];               // Array de botones del juego
    Button optionButtons[CANT_OPTIONS_BUTTON];          // Array de botones del menu opciones
    Button optGrid[MAX_FILAS][MAX_COLUMNAS];            // Array de botones de la grilla del menu opciones
    TTF_Font *fuenteTTFChico;                           // Fuente cargada para la generacion de textos de tamaño chico
    TTF_Font *fuenteTTF;                                // Fuente cargada para la generacion de textos de tamaño normal
    TTF_Font *fuenteTTFTitulo;                          // Fuente cargada para la generacion de textos de tamaño grande
    tSonido *sndJuego[CANT_SND];                        // Array de sonidos cargados desde el disco
    uint32_t anchoVentana;                              // Dimension horizontal de la ventana
    uint32_t altoVentana;                               // Dimension vertical  de la ventana
    tHUD *hudSetOptions;                                // Instancia del HUD para seleccionar el set deseado en el menu opciones
    tHUD *hudGame[CANT_GAME_HUD];                       // Array de instancias del HUD para contador de puntos de los jugadores
    tHUD *hudIndicadorTurno;                            // Instancia del HUD para indicar el turno del jugador en el juego
    Vector cartas;                                      // Vector con memoria dinamica para almacenar las cartas del juego
    Vector jugadores;                                   // Vector con memoria dinamica para almacenar puntaje de los jugadores
    Layout layout;                                      // Almacena las variables para generar el tamaño de la grilla de las cartas
    eCartas firstCard;                                  // Flag de la primer carta seleccionada
    eCartas secondCard;                                 // Flag de la segunda carta seleccionada
    uint32_t revealTime;                                // Guarda el tiempo en el que se selecciono la segunda carta para generar un delay al determinar si es correcta o no
    uint32_t contCoincidencias;                         // Contador interno de coincidencias para determinar si el juego se gano o no
    uint32_t contTurno;                                 // Contador interno de la cantidad de turnos jugados
    uint32_t indiceTurno;                               // Indice para saber a que jugador le pertenece el turno actual
    entradaTxt input[CANT_JUGADORES];                   // Array de instancias de una entrada de texto para ingresar el nombre de los jugadores en el menu opciones
    entradaTxt *inputActual;                            // Puntero la instancia de la entrada de texto activa en el momento
    tConfig config;                                     // Estructura para almacenar la configuracion del juego activa
    SDL_Event event;                                    // Variable donde almacenar el evento que cada estado va a procesar a su manera
    SDL_Point mouse;                                    // Variable donde se almacena el punto donde se encuentra el mouse
    uint8_t audioInicializado;                          // Flag de control de SDL_Mixer
    int estadoMemoria;                                  // Bandera que avisa en que estado se encuentra el juego de cartas WAIT_FIRST: espera primer carta WAIT_SECOND: espera segunda carta CHECK: al revelar la segunda checkea si hubo coincidencia FINISHED: da el juego por terminado
    Estado *estado;                                     // Estado actual del juego
    Estado *estadoPendiente;                            // Bandera para avisar cambio de estado pendiente
    uint8_t corriendo;                                  // Bandera de control de estado del game loop.
} tJuego;


/**
 * @brief Inicializa todos los subsistemas y carga los recursos iniciales en el contexto del juego.
 *
 * @param juego Puntero a la estructura tJuego a inicializar.
 *
 * @return tError TODO_OK o el codigo de error correspondiente al fallo.
 */
tError juego_inicializar(tJuego *juego);


/**
 * @brief Captura y deriva el procesamiento de las entradas del usuario al estado de juego actual.
 *
 * @param juego Puntero al contexto del juego.
 *
 * @return tError TODO_OK o el codigo de error correspondiente al fallo.
 */
tError juego_procesar_eventos(tJuego *juego);

/**
 * @brief Deriva la actualizacion de la logica del juego al estado de juego actual.
 *
 * @param juego Puntero al contexto del juego.
 */
void juego_actualizar(tJuego *juego);

/**
 * @brief Deriva el orden de llamado a las funciones de los modulos correspondientes al renderizado al estado del juego actual.
 *
 * @param juego Puntero al contexto del juego.
 */
void juego_renderizar(tJuego *juego);

/**
 * @brief Libera la totalidad de los recursos del juego.
 *
 * @param juego Puntero al contexto a destruir.
 */
void juego_destruir(tJuego *juego);

/**
 * @brief Aplica un cambio de estado de juego pendiente.
 *
 * @param juego Puntero al contexto del juego.
 */
void juego_aplicar_cambio_estado(tJuego *juego);


#endif // JUEGO_H_INCLUDED
