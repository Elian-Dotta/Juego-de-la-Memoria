/**
 * @file cargar_recursos.c
 * @author Dotta, Elian Agustin
 * @brief Funciones para cargar los recursos del juego (imagenes, sonidos y framebuffers)
 * Implementacion de del modulo 'cargar_recursos'.
 *
 * @version 1.0
 * @date 2026-03-05
 *
 */
#include "cargar_recursos.h"

#include "juego.h"

#include "config.h"
#include "graficos.h"
#include "hud.h"




tError cargar_imagenes(tJuego *juego)
{
    juego->background[MENU_BACKGROUND] = imagenes_cargar_gpu(juego->renderer, "img/Fondo_Menu.jpg");
    if(!juego->background[MENU_BACKGROUND])
    {
        return ERR_IMAGEN;
    }

    juego->background[GAME_BACKGROUND] = imagenes_cargar_gpu(juego->renderer, "img/Fondo_Juego.jpg");
    if(!juego->background[GAME_BACKGROUND])
    {
        return ERR_IMAGEN;
    }

    juego->background[OPTIONS_BACKGROUND] = imagenes_cargar_gpu(juego->renderer, "img/Fondo_Configuracion.jpg");
    if(!juego->background[OPTIONS_BACKGROUND])
    {
        return ERR_IMAGEN;
    }

    juego->background[PAUSE_BACKGROUND] = imagenes_cargar_gpu(juego->renderer, "img/Fondo_Pausa.jpg");
    if(!juego->background[PAUSE_BACKGROUND])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta1] = imagenes_cargar_gpu(juego->renderer, "img/Billete_1.png");
    if(!juego->SET1Texture[Carta1])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta2] = imagenes_cargar_gpu(juego->renderer, "img/Billete_2.png");
    if(!juego->SET1Texture[Carta2])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta3] = imagenes_cargar_gpu(juego->renderer, "img/Billete_10.png");
    if(!juego->SET1Texture[Carta3])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta4] = imagenes_cargar_gpu(juego->renderer, "img/Billete_20.png");
    if(!juego->SET1Texture[Carta4])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta5] = imagenes_cargar_gpu(juego->renderer, "img/Billete_50.png");
    if(!juego->SET1Texture[Carta5])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta6] = imagenes_cargar_gpu(juego->renderer, "img/Billete_100.png");
    if(!juego->SET1Texture[Carta6])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta7] = imagenes_cargar_gpu(juego->renderer, "img/Billete_200.png");
    if(!juego->SET1Texture[Carta7])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta8] = imagenes_cargar_gpu(juego->renderer, "img/Billete_500.png");
    if(!juego->SET1Texture[Carta8])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta9] = imagenes_cargar_gpu(juego->renderer, "img/Billete_1000.png");
    if(!juego->SET1Texture[Carta9])
    {
        return ERR_IMAGEN;
    }

    juego->SET1Texture[Carta10] = imagenes_cargar_gpu(juego->renderer, "img/Billete_1000000.png");
    if(!juego->SET1Texture[Carta10])
    {
        return ERR_IMAGEN;
    }
    juego->SET1Texture[CartaReverso] = imagenes_cargar_gpu(juego->renderer, "img/Billete_Reverso.png");
    if(!juego->SET1Texture[CartaReverso])
    {
        return ERR_IMAGEN;
    }


    juego->SET2Texture[Carta1] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Catamarca.png");
    if(!juego->SET2Texture[Carta1])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta2] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Cordoba.png");
    if(!juego->SET2Texture[Carta2])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta3] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_La_Pampa.png");
    if(!juego->SET2Texture[Carta3])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta4] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Misiones.png");
    if(!juego->SET2Texture[Carta4])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta5] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Neuquen.png");
    if(!juego->SET2Texture[Carta5])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta6] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Río_Negro.png");
    if(!juego->SET2Texture[Carta6])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta7] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Salta.png");
    if(!juego->SET2Texture[Carta7])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta8] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Santa_Cruz.png");
    if(!juego->SET2Texture[Carta8])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta9] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Santiago_del_Estero.png");
    if(!juego->SET2Texture[Carta9])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[Carta10] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Tierra_del_Fuego.png");
    if(!juego->SET2Texture[Carta10])
    {
        return ERR_IMAGEN;
    }

    juego->SET2Texture[CartaReverso] = imagenes_cargar_gpu(juego->renderer, "img/Escudo_Reverso.png");
    if(!juego->SET2Texture[CartaReverso])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta1] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Asado.png");
    if(!juego->SET3Texture[Carta1])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta2] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Churro.png");
    if(!juego->SET3Texture[Carta2])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta3] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Empanada.png");
    if(!juego->SET3Texture[Carta3])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta4] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Facturas.png");
    if(!juego->SET3Texture[Carta4])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta5] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Fernet.png");
    if(!juego->SET3Texture[Carta5])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta6] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Locro.png");
    if(!juego->SET3Texture[Carta6])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta7] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Mate.png");
    if(!juego->SET3Texture[Carta7])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta8] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Milanesa.png");
    if(!juego->SET3Texture[Carta8])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta9] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Pastelitos.png");
    if(!juego->SET3Texture[Carta9])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[Carta10] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Torta_Frita.png");
    if(!juego->SET3Texture[Carta10])
    {
        return ERR_IMAGEN;
    }

    juego->SET3Texture[CartaReverso] = imagenes_cargar_gpu(juego->renderer, "img/Costumbres_Reverso.png");
    if(!juego->SET3Texture[CartaReverso])
    {
        return ERR_IMAGEN;
    }

    juego->button[MENU_BUTTON] = imagenes_cargar_gpu(juego->renderer, "img/Boton_Menu.png");
    if(!juego->button[MENU_BUTTON])
    {
        return ERR_IMAGEN;
    }

    juego->buttong = imagenes_cargar_gpu(juego->renderer, "img/Boton_pausar_juego.png");
    if(!juego->buttong)
    {
        return ERR_IMAGEN;
    }

    juego->button[OPTIONS_ARROW_BUTTON] = imagenes_cargar_gpu(juego->renderer, "img/Options_Arrow.png");
    if(!juego->button[OPTIONS_ARROW_BUTTON])
    {
        return ERR_IMAGEN;
    }

    juego->optionsRectangle = imagenes_cargar_gpu(juego->renderer, "img/Options_Grid_Set.png");
    if(!juego->optionsRectangle)
    {
        return ERR_IMAGEN;
    }

    juego->button[OPTIONS_SQUARE_BUTTON] = imagenes_cargar_gpu(juego->renderer, "img/Options_Player.png");
    if(!juego->button[OPTIONS_SQUARE_BUTTON])
    {
        return ERR_IMAGEN;
    }

    juego->indicadorPlayer[GAME_IND_P1] = imagenes_cargar_gpu(juego->renderer, "img/Jugador_1.png");
    if(!juego->indicadorPlayer[GAME_IND_P1])
    {
        return ERR_IMAGEN;
    }

    juego->indicadorPlayer[GAME_IND_P2] = imagenes_cargar_gpu(juego->renderer, "img/Jugador_2.png");
    if(!juego->indicadorPlayer[GAME_IND_P2])
    {
        return ERR_IMAGEN;
    }

    juego->icono = imagenes_cargar_ram("img/icon.png");
    if(!juego->icono)
    {
        return ERR_IMAGEN;
    }



    return TODO_OK;

}

tError cargar_audio(tJuego *juego)
{
    juego->sndJuego[SND_VICTORIA] = sonidos_cargar("snd/Victoria.mp3");
    if(!juego->sndJuego[SND_VICTORIA])
    {
        return ERR_SDL_MIXER;
    }

    juego->sndJuego[SND_ACIERTO_BILLETE] = sonidos_cargar("snd/Acierto_Billete.mp3");
    if(!juego->sndJuego[SND_ACIERTO_BILLETE])
    {
        return ERR_SDL_MIXER;
    }

    juego->sndJuego[SND_ACIERTO_GENERICO] = sonidos_cargar("snd/Acierto_Generico.mp3");
    if(!juego->sndJuego[SND_ACIERTO_GENERICO])
    {
        return ERR_SDL_MIXER;
    }

    juego->sndJuego[SND_INCORRECTO] = sonidos_cargar("snd/Incorrecto.mp3");
    if(!juego->sndJuego[SND_INCORRECTO])
    {
        return ERR_SDL_MIXER;
    }

    juego->sndJuego[SND_CAMBIAR_CARTA] = sonidos_cargar("snd/Cambiar_Carta.mp3");
    if(!juego->sndJuego[SND_CAMBIAR_CARTA])
    {
        return ERR_SDL_MIXER;
    }

    juego->sndJuego[SND_BOTON] = sonidos_cargar("snd/Boton.mp3");
    if(!juego->sndJuego[SND_BOTON])
    {
        return ERR_SDL_MIXER;
    }

    juego->sndJuego[SND_MUSICA] = sonidos_cargar("snd/Musica.ogg");
    if(!juego->sndJuego[SND_MUSICA])
    {
        return ERR_SDL_MIXER;
    }

    return TODO_OK;
}

tError crear_framebuffers(tJuego *juego)
{
    int i;
    for(i = 0; i < FB_GAME_CANT; i++)
    {
            juego->eFramebuffersGame[i] = graficos_crear_framebuffer(juego->renderer, juego->anchoVentana, juego->altoVentana);
            if(!juego->eFramebuffersGame[i])
            {
                return ERR_FRAMEBUFFER;
            }
    }


    for(i = 0; i < FB_MENU_CANT; i++)
    {
            juego->eFramebuffersMenu[i] = graficos_crear_framebuffer(juego->renderer, juego->anchoVentana, juego->altoVentana);
            if(!juego->eFramebuffersMenu[i])
            {
                return ERR_FRAMEBUFFER;
            }
    }

    for(i = 0; i < FB_OPTIONS_CANT; i++)
    {
            juego->eFramebuffersOptions[i] = graficos_crear_framebuffer(juego->renderer, juego->anchoVentana, juego->altoVentana);
            if(!juego->eFramebuffersOptions[i])
            {
                return ERR_FRAMEBUFFER;
            }
    }

    for(i = 0; i < FB_PAUSE_CANT; i++)
    {
            juego->eFramebuffersPause[i] = graficos_crear_framebuffer(juego->renderer, juego->anchoVentana, juego->altoVentana);
            if(!juego->eFramebuffersPause[i])
            {
                return ERR_FRAMEBUFFER;
            }
    }

    return TODO_OK;
}






