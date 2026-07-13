#ifndef IMAGENES_H_INCLUDED
#define IMAGENES_H_INCLUDED

#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef enum {
    IMAGEN_BMP  = 0x00,
    IMAGEN_JPG  = 0x01,
    IMAGEN_PNG  = 0x02,
} tFormatosImg;

tFormatosImg imagenes_inicializar(void);


SDL_Texture* imagenes_cargar_gpu(SDL_Renderer *renderer, const char *path);


SDL_Surface* imagenes_cargar_ram(const char *path);

void imagenes_finalizar(void);

#endif // IMAGENES_H_INCLUDED
