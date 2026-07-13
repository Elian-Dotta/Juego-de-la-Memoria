#ifndef SONIDOS_H_INCLUDED
#define SONIDOS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define AMPLITUD_TONO 8192
#define FREC_MUESTREO 44100

typedef enum {
    SONIDO_ERR = -1,
    SONIDO_WAV = 0x00,
    SONIDO_MP3 = 0x01,
    SONIDO_OGG = 0x02,
} tFormatosSnd;


typedef struct sSonido tSonido;


tFormatosSnd sonidos_inicializar(void);


tSonido* sonidos_cargar(const char *path);


tSonido* sonidos_crear_tono(float frecuencia);


void sonidos_reproducir(const tSonido *sonido, int32_t cantVeces);


void sonidos_destruir(tSonido *sonido);


void sonidos_finalizar(void);

#endif // SONIDOS_H_INCLUDED
