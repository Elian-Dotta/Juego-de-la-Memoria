/**
 * @file config.h
 * @author Dotta, Elian Agustin
 * @brief Manejo de configuracion del juego
 *
 * @version 1.0
 * @date 2026-03-05
 */
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/** @brief Tamaño de la grilla en el eje X */
#define TAM_GRID_X 16
/** @brief Tamaño de la grilla en el eje Y */
#define TAM_GRID_Y 9

#include "errores.h"
#include "TDAvectorGen.h"

#define window_w 1336
#define window_h (window_w / 16 * 9)
#define center_w (window_w / 2)
#define center_h (window_h / 2)
#define TILE (window_w / TAM_GRID_X)

#define CANT_MAX_JUGADORES 2
#define LOOP_DELAY 16 // Ms
#define TAM_BUFFER 256
#define MAX_FILAS 6
#define MAX_COLUMNAS 6
#define CONFIG_RUTA "config.txt"
#define LARGO_NOMBRE 4

/**
 * @brief Enumeracion de los sets de cartas soportados.
 *
 */
typedef enum {
    SET_BILLETES,
    SET_ESCUDOS,
    SET_COSTUMBRES,
    CANT_SET,
}eOptionsSet;


/**
 * @brief Enumeracion de posicionamiento de texturas dentro del rectangulo donde se van a dibujar.
 *
 * @note Mueve el inicio del rectangulo donde se va a dibujar la textura.
 * POS_CENTER lleva el centro de la textura a la coordenada enviada en las funciones que requieren este tipo como parametro, y mueve la esquina del rectangulo final.
 *
 */
typedef enum
{
    POS_LUCORNER = 0,
    POS_CENTER,
    POS_LDCORNER,
    POS_RUCORNER,
    POS_RDCORNER,
}ePos;


/**
 * @brief Macro para calcular la posicion en pixeles a partir de la posicion en grilla.
 *
 */
#define posX(grid_x) ((grid_x) * (TILE) + TILE / 2)
#define posY(grid_y) ((grid_y) * (TILE) + TILE / 2)


/**
 * @brief Estructura para almacenar la configuracion actual del juego.
 *
 * @note Se carga al iniciar el juego desde un archivo y se actualiza y guarda al modificar la configuracion desde el menu de opciones.
 */
typedef struct tConfig
{
    int filasGrilla;                        // Cantidad de filas de la grilla actual del juego
    int columnasGrilla;                     // Cantidad de columnas de la grilla actual del juego
    int numeroSet;                          // Numero del set utilizado actualmente en el juego
    int cant_Jugadores;                     // Cantidad de jugadores actual del juego
    int cant_Cartas;                        // Cantidad de cartas actual del juego
    int grilla[MAX_FILAS][MAX_COLUMNAS];    // Informacion de la forma de la grilla actual del juego
    char n1[LARGO_NOMBRE];                  // Nombre del jugador 1
    char n2[LARGO_NOMBRE];                  // Nombre del jugador 2
}tConfig;

/**
 * @brief Inicializa la configuracion del juego con valores por defecto.
 *
 * @param cfg Puntero a la estructura de configuracion a inicializar.
 */
void config_default(tConfig *cfg);


/**
 * @brief Carga la configuracion del juego desde un archivo con formato especifico.
 *
 * @param cfg Puntero a la estructura de configuracion a cargar.
 * @param ruta Ruta del archivo de configuracion
 *
 * @return tError 0 o el codigo de error correspondiente al fallo.
 */
tError config_cargar(tConfig *cfg, const char *ruta);


/**
 * @brief Actualiza la configuracion del juego con los valores recibidos por parametro.
 *
 * @param cfg Puntero a la estructura de configuracion a actualizar.
 * @param grilla Matriz de enteros con el layout de la grilla actual del juego.
 * @param set_carta Indice del set de cartas seleccionado actualmente.
 * @param cantJugadores Numero de jugadores en el juego.
 * @param n1 Nombre del primer jugador.
 * @param n2 Nombre del segundo jugador.
 *
 * @return tError 0 o el codigo de error correspondiente al fallo.
 */
tError config_actualizar(tConfig *cfg, int grilla[MAX_FILAS][MAX_COLUMNAS],int set_carta, int cantJugadores, const char* n1, const char* n2);

/**
 * @brief Guarda la configuracion del juego en un archivo.
 *
 * @param cfg Puntero a la estructura de configuracion a guardar.
 * @param ruta Ruta del archivo de configuracion.
 *
 * @return tError 0 o el codigo de error correspondiente al fallo.
 */
tError config_guardar(tConfig *cfg, const char *ruta);

#endif // CONFIG_H_INCLUDED
