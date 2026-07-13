#ifndef INPUTTXT_H_INCLUDED
#define INPUTTXT_H_INCLUDED

#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"

typedef struct tJuego tJuego;

typedef struct
{
    char buffer[TAM_BUFFER];
    bool activo;

    int posX;
    int posY;

    float escalaHor;
    float escalaVer;

    TTF_Font* fuente;
    SDL_Color color;

    Button botonEntrada;
}entradaTxt;

void inicializarEntrada(entradaTxt* input, SDL_Texture* textura, TTF_Font* fuente, char* s, SDL_Color color, int32_t posX, int32_t posY, float escalaHorTxt, float escalaVerTxt, float escalaHorBut, float escalaVerBut, int pos);

void activarInput(tJuego *j, entradaTxt* input);

void desactivarInput(tJuego *j);

void iniciarEntrada(entradaTxt* input);

void pararEntrada(entradaTxt* input);

void dibujarBotonEntrada(SDL_Renderer* renderer, entradaTxt* input);

void dibujarEntradaEnPantalla(SDL_Renderer* renderer, entradaTxt* input);

const char* devolverEntrada(entradaTxt* input);

#endif // INPUTTXT_H_INCLUDED
