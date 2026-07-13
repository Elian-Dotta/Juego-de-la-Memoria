#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED
#include "errores.h"
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct sHUD tHUD;



typedef SDL_Texture* (*tActualizarHUD) (SDL_Renderer *renderer, void *dato, void *datoNuevo, void *extra);


typedef void (*tDestruirHUD) (void *extra);




tHUD* hud_inicializar (SDL_Renderer *renderer, int32_t posX, int32_t posY, void *dato, size_t tamDato, void *extra, bool destruirTextura, tActualizarHUD actualizar, tDestruirHUD destruir);


void hud_dibujar (const tHUD *hud);


tError hud_actualizar_dato (tHUD *hud, void *datoNuevo);


void hud_actualizar_posicion_abs (tHUD *hud, int32_t posX, int32_t posY, double angulo);


void hud_actualizar_posicion_rel (tHUD *hud, int32_t posX, int32_t posY, double angulo);


void hud_destruir(tHUD *hud);


#endif // HUD_H_INCLUDED
