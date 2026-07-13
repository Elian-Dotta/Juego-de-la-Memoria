/**
 * @file errores.h
 * @author Dotta, Elian Agustin
 * @brief Definicion de los codigos de error del juego 
 * 
 * Basado en material provisto por la catedra.
 * Posteriormente modificado y extendido para 
 * las necesidades de este proyecto.
 * 
 * @version 1.0
 * @date 2026-03-05
 */


#ifndef ERRORES_H_INCLUDED
#define ERRORES_H_INCLUDED

typedef enum {

    // Generico
    TODO_OK = 0, 
    ERR_MEMORIA,
    ERR_ARCHIVO,
    ERR_FRAMEBUFFER,
    ERR_CONFIG_INVALIDA,
    ERR_PARAMETRO_INVALIDO,

    // SDL
    ERR_SDL,
    ERR_SDL_MIXER,
    ERR_SDL_TTF,
    ERR_SDL_IMAGE,
    ERR_TEXTURA,

    // Audio
    ERR_AUDIO_DRIVER,
    ERR_AUDIO_ASSET,

    // Imagen
    ERR_IMAGEN,

    // HUD
    ERR_HUD_INICIALIZAR,
    ERR_HUD_ACTUALIZAR,

} tError;

/**
 * @brief Devuelve una cadena descriptiva asociada a un codigo de error.
 * 
 * @param err Codigo de error a describir
 * 
 * @return const char* Puntero a un cadena constante con el detalle del error.
 * 
 * @note La cadena devuelta no debe ser liberada por el usuario
 */
const char* errores_obtener_detalle(tError err);

#endif // ERRORES_H_INCLUDED
