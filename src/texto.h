#ifndef TEXTO_H_INCLUDED
#define TEXTO_H_INCLUDED

#include "errores.h"
#include "config.h"
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct tTexto{
    SDL_Texture *texto;
    int32_t x;
    int32_t y;
}tTexto;


tError texto_inicializar(void);

TTF_Font* texto_cargar_fuente(const char *path, uint32_t tam);

void texto_destruir_fuente(TTF_Font *fuente);

tError texto_crear_textura(SDL_Renderer *renderer, tTexto *texto, TTF_Font *fuente, const char* cadena, SDL_Color color, int32_t posX, int32_t posY);

void texto_dibujar_textura(SDL_Renderer *renderer, tTexto *texto, SDL_Rect *origen, float escalaHor, float escalaVer, double angulo, uint8_t flipHor, uint8_t flipVer, ePos pos);

tError texto_destruir_textura(tTexto *txt);

void texto_finalizar(void);


#endif // TEXTO_H_INCLUDED
