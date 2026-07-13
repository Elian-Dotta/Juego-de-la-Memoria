/**
 * @file button.h
 * @author Dotta, Elian Agustin
 * @brief Estructura y funciones para manejar botones en la interfaz gráfica del juego.
 * @version 1.0
 * @date 2026-03-05
 * 
 */

#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "errores.h"
#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

/**
 * @brief Estructura que define un botón en la interfaz gráfica del juego, con su textura, posición, colores para diferentes estados (normal, hover, apretado, toggled), ángulo de rotación, punto de centro para rotación y volteo, rectángulo final para dibujar el botón, y estados booleanos para manejar la interacción del mouse.
 * 
 */
typedef struct
{
    int32_t x;                      // Posición X del centro del botón en la pantalla
    int32_t y;                      // Posición Y del centro del botón en la pantalla

    SDL_Texture *texture;           // Textura del botón

    double angulo;                  // Ángulo de rotación del botón
    SDL_Point centro;               // Punto de centro para la rotación del botón
    SDL_RendererFlip flip;          // Volteo horizontal y vertical del botón

    SDL_Rect rectFinal;             // Rectángulo final para dibujar el botón, calculado a partir de la posición, tamaño de la textura y escala

    bool hovered;                   // Flag para indicar si el mouse está sobre el botón
    bool esApretado;                // Flag para indicar si el botón está apretado
    bool clickeado;                 // Flag para indicar si el botón fue clickeado
    bool toggle;                    // Flag para indicar si el botón está en estado toggle
} Button;


/**
 * @brief Inicializa un botón con la textura, posición, escala, ángulo y volteo especificados.
 * 
 * @param b             Puntero al botón a inicializar
 * @param textura       Puntero a la textura del botón
 * @param posX          Posicion en X donde se dibujará el botón
 * @param posY          Posicion en Y donde se dibujará el botón
 * @param escalaHor     Escala horizontal para el tamaño del botón (1.0 = tamaño original de la textura)
 * @param escalaVer     Escala vertical para el tamaño del botón (1.0 = tamaño original de la textura)
 * @param angulo        Ángulo de rotación del botón en grados
 * @param flipHor       Flag para indicar si el botón se voltea horizontalmente (true o false)
 * @param flipVer       Flag para indicar si el botón se voltea verticalmente (true o false)
 * 
 * @return tError    TODO_OK si la inicialización fue exitosa, ERR_TEXTURA u ERR_PARAMETRO_INVALIDO si hubo un error en los parámetros o la textura.
 */
tError button_crear(Button *b, SDL_Texture *textura, int32_t posX, int32_t posY, float escalaHor, float escalaVer, double angulo, uint8_t flipHor, uint8_t flipVer);

/**
 * @brief Destruye un botón y libera los recursos asociados.
 * 
 * @param b  Puntero al botón a destruir
 * 
 * @note Esta funcion no libera la textura del botón, ya que se asume que las texturas son compartidas y manejadas por otro módulo del juego. Solo resetea los campos del botón y deja de apuntar a la textura.
 */
void button_destruir(Button *b);

/**
 * @brief Destruye la textura a la que apunta el botón.
 * 
 * @param b Puntero al botón a destruir su textura.
 * 
 */
void button_destruir_textura(Button *b);

/**
 * @brief Actualiza un botón clickeado.
 * 
 * @param b Puntero al botón a actualizar.
 * 
 * @note Resetea el campo 'clickeado' con el objetivo que el click tenga duracion de un frame.
 */
void actualizarBoton(Button* b);

/**
 * @brief Reinicia todas las flags de un botón
 * 
 * @param b Puntero al botón a reiniciar
 * 
 * @note Reinicia todas las flags del boton.
 */
void reiniciarBoton(Button *b);

/**
 * @brief Dibuja la textura almacenada del botón utilizando los datos ingresados en su creacion
 * 
 * @param renderer Puntero al renderer del juego
 * @param b        Puntero al botón a dibujar
 * 
 * @note Modifica el color de la textura dependiendo de en que estado se encuentre el botón. (HOVERED, APRETADO, CLICKEADO, TOGGLED, TOGGLED + HOVERED). Luego pinta a la textura de su color original.
 */
void button_dibujar_textura(SDL_Renderer *renderer, Button *b);

#endif // BUTTON_H_INCLUDED
