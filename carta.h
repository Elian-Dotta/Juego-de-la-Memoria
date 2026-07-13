/**
 * @file carta.h
 * @author Dotta, Elian Agustin
 * @brief 
 * @version 1.0
 * @date 2026-03-05
 *
 * 
 */

#ifndef CARTA_H_INCLUDED
#define CARTA_H_INCLUDED

#include "config.h"
#include <SDL2/SDL.h>

#define OCULTA 0
#define REVELADA 1
#define COINCIDENCIA 2


//* @brief Enumeración que define los diferentes tipos de cartas en el juego, incluyendo 10 tipos de cartas distintas y una carta de reverso.
typedef enum {
    Carta1,
    Carta2,
    Carta3,
    Carta4,
    Carta5,
    Carta6,
    Carta7,
    Carta8,
    Carta9,
    Carta10,
    CartaReverso,
    CARTAS_CANT,
}eCartas;

/**
 * @brief Estructura que representa una carta en el juego de memoria, con su ID, estado (oculta, revelada o coincidencia), tipo de carta, texturas para el frente y reverso, colores para diferentes estados (normal, hover y apretado), rectángulo final para dibujar la carta, posición en la grilla (fila y columna) y flags booleanos para manejar la interacción del mouse (hover y apretado).
 * 
 */
typedef struct {
    int id_carta;
    int estado; // 0 = oculto, 1 = revelado, 2 = coincidencia

    eCartas tipo;

    SDL_Texture *frente;
    SDL_Texture *reverso;
    SDL_Color normal;
    SDL_Color hover;
    SDL_Color apretado;

    SDL_Rect rectFinal;
    int c;
    int r;

    bool hovered;
    bool esApretado;
}Carta;

/**
 * @brief Estructura que define el layout para la disposición de las cartas en la pantalla, incluyendo la posición inicial (startX, startY), tamaño de cada carta (cartaW, cartaH), y los márgenes entre las cartas (margenX, margenY).
 * 
 */
typedef struct {
    float startX;
    float startY;
    float cartaW;
    float cartaH;
    float margenX;
    float margenY;
}Layout;

/**
 * @brief Estructura que define el contexto para actualizar la posición de las cartas en la grilla, incluyendo el índice actual, cantidad de columnas, márgenes entre las cartas, posición inicial (IniX, IniY), tamaño de cada carta (cartaW, cartaH) y una matriz que representa la disposición de las cartas en la grilla (grilla[MAX_FILAS][MAX_COLUMNAS]).
 * 
 */
typedef struct {
    int indice;
    int cantColumnas;
    float margenX;
    float margenY;
    int IniX;
    int IniY;
    float cartaW;
    float cartaH;
    int grilla[MAX_FILAS][MAX_COLUMNAS];
}contextoActPosicion;

//* @brief Función que actualiza la posición de una carta en la grilla utilizando el contexto proporcionado, calculando la fila y columna correspondientes al índice actual y asignando las coordenadas finales para dibujar la carta en pantalla.
void actualizar_posicion_carta(void* a, void* b);   
void imprimirCarta(void* a, void* b);
void mouseSobreCarta(void* a, void* b);
void leftClickEnCarta(void* a, void* b);
void noClickEnCarta(void* a, void* b);
void actualizarCarta(void* a, void* b);
int cmpCarta(const void* a, const void* b);


void crear_carta(Carta *c, int id, int tipo, SDL_Texture *Fr, SDL_Texture *Atr);
void carta_dibujar_textura(SDL_Renderer* renderer, Carta *carta);
void carta_rol(Vector* cartas);
void carta_ror(Vector* cartas);

void calcular_layout_cartas(
    SDL_Texture* texturaEjemplo,
    int filas,
    int columnas,
    float limSup,
    float limInf,
    float limIzq,
    float limDer,
    float margenFactor,
    Layout *lay
);

// void cartaDesmezclar(VectorIterador* dest, VectorIterador* cur);

#endif // CARTA_H_INCLUDED
