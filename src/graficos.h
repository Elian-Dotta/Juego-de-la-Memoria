#ifndef GRAFICOS_H_INCLUDED
#define GRAFICOS_H_INCLUDED
#include <SDL2/SDL.h>

#include "config.h"

#define COLOR(r,g,b,a) (SDL_Color){r, g, b, a}

#define BLANCO           COLOR(255,255,255,255)
#define XBLANCO          COLOR(255,255,255,0)
#define NEGRO            COLOR(0,0,0,255)
#define GRIS             COLOR(128,128,128,255)

#define ROJO             COLOR(255,0,0,255)
#define ROJO_OSCURO      COLOR(200,0,0,255)

#define VERDE            COLOR(0,255,0,255)
#define AZUL             COLOR(0,0,255,255)

#define AMARILLO         COLOR(255,255,0,255)

#define COLOR_GOLD_NORMAL COLOR(212,175,55,255)
#define COLOR_GOLD_BRIGHT COLOR(255,215,80,255)
#define COLOR_GOLD_WARM   COLOR(255,180,0,255)
#define COLOR_GOLD_DARK   COLOR(180,140,30,255)

#define COLOR_VERDE        COLOR(120,255,120,255)
#define COLOR_VERDE_BRIGHT  COLOR(180,255,180,255)
#define COLOR_AMARILLO      COLOR(255,220,0,255)
#define COLOR_AMARILLO_BRIGHT    COLOR(255,240,80,255)

#define COLOR_AZUL        COLOR(50,150,255,255)
#define COLOR_ROJO        COLOR(255,100,100,255)
#define COLOR_DORADO      COLOR(255,215,0,255)

#define COLOR_ARG_CELESTE COLOR(100,160,200,255)
#define COLOR_ARG_BLANCO  COLOR(235,235,235,255)
#define COLOR_ARG_AMARILLO COLOR(210,170,60,255)

#define BNORMAL            BLANCO
#define BHOVERED           ROJO_OSCURO
#define BAPRETADO          ROJO_OSCURO

#define BTOGGLED           COLOR_VERDE
#define BTOGGLED_HOVERED   COLOR_VERDE_BRIGHT

#define SDL_APLICAR_COLOR(tex, color) SDL_SetTextureColorMod(tex, (color).r, (color).g, (color).b)

void graficos_dibujar_textura(SDL_Renderer *renderer, SDL_Texture *textura, SDL_Rect *origen, int32_t posX, int32_t posY, float escalaHor, float escalaVer, double angulo, uint8_t flipHor, uint8_t flipVer, ePos pos);

void graficos_borrar_pantalla(SDL_Renderer *renderer, const SDL_Color *color);

SDL_Texture* graficos_crear_framebuffer(SDL_Renderer *renderer, uint32_t anchoVentana, uint32_t altoVentana);

void graficos_cambiar_framebuffer(SDL_Renderer *renderer, SDL_Texture *target);

void graficos_dibujar_grilla(SDL_Renderer *renderer);

void graficos_renderizar(SDL_Renderer *renderer, SDL_Texture* const *framebuffers, uint32_t cantFramebuffers);


#endif // GRAFICOS_H_INCLUDED
