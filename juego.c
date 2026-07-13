/**
 * @file juego.c
 * @brief Implementacion del modulo 'juego'
 */

#include "juego.h"
#include "imagenes.h"
#include "graficos.h"
#include "menu.h"
#include "gameplay.h"
#include "button.h"
#include "opciones.h"
#include "inputTxt.h"
#include "jugador.h"
#include "cargar_recursos.h"
#include "sonidos.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>



/************************
    FUNCIONES PUBLICAS
*************************/


tError juego_inicializar(tJuego *juego)
{
    // Inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != TODO_OK) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return ERR_SDL;
    }

    tError err;
    char nombreVentana[TAM_BUFFER];
    sprintf(nombreVentana, "Juego de la Memoria Patriotico");
    juego->anchoVentana = window_w;
    juego->altoVentana = window_h;

    // Crea la ventana del juego
    juego->ventana = SDL_CreateWindow(nombreVentana,
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           juego->anchoVentana,
                                           juego->altoVentana,
                                           SDL_WINDOW_SHOWN);
    if (!juego->ventana) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return ERR_SDL;
    }

    // Crea el renderer para la ventana
    juego->renderer = SDL_CreateRenderer(juego->ventana, -1, SDL_RENDERER_ACCELERATED);
    if (!juego->renderer) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return ERR_SDL;
    }

    SDL_SetRenderDrawBlendMode(juego->renderer, SDL_BLENDMODE_BLEND); // Habilita la transparencia en el renderer

    // Inicializa SDL_ttf
    if((err = texto_inicializar()) != TODO_OK)
    {
        return err;
    }

    // Carga las fuentes del utilizadas en el juego
    juego->fuenteTTF = texto_cargar_fuente("fnt/Milonga-Regular.ttf", 24);
    juego->fuenteTTFTitulo = texto_cargar_fuente("fnt/Milonga-Regular.ttf", 60);
    juego->fuenteTTFChico = texto_cargar_fuente("fnt/Milonga-Regular.ttf", 16);

    // Inicializa SDL_image
    if((err = imagenes_inicializar()) != (IMAGEN_BMP | IMAGEN_JPG | IMAGEN_PNG))
        return err;

    // Carga las imagenes del juego de la carpeta "img"
    if((err = cargar_imagenes(juego)))
        return err;

    // Crea los framebuffers del juego
    if((err = crear_framebuffers(juego)))
        return err;

    // Carga la configuracion del juego desde "config.txt"
    if(config_cargar(&juego->config, CONFIG_RUTA))
    {
        // Si falla carga la configuracion de juego por default
        config_default(&juego->config);
        if((err = config_guardar(&juego->config, CONFIG_RUTA))) // Guarda la configuracion default en el archivo
            return err;
    }

    /*
    printf("%d\n", juego->config.cant_Cartas);
    for(int i = 0; i < MAX_FILAS; i++)
    {
        for(int t = 0; t < MAX_COLUMNAS; t++)
        {
            printf("%d", juego->config.grilla[i][t]);

        }
        putchar('\n');
    }
    */

    int i;

    // Inicializa SDL_Mixer
    tFormatosSnd formatosSnd = sonidos_inicializar();
    if (formatosSnd != SONIDO_ERR) {
        juego->audioInicializado = 1;

        cargar_audio(juego); // Carga el audio del juego de la carpeta "snd"

        for(i = 0; i < CANT_SND; i++)
        {
            if(!juego->sndJuego[i])
            {
                fprintf(stderr, "Assets de audio no encontrados\n");
                juego->audioInicializado = 0;
                sonidos_finalizar();
            }
        }
    } else
    {
        juego->audioInicializado = 0;
    }

    // Comienza a reproducir la musica del juego
    sonidos_reproducir(juego->sndJuego[SND_MUSICA], -1);

    // Crea el vector de jugadores y los inicializa
    vectorCrear(&juego->jugadores, sizeof(Jugador));

    Jugador player;

    for(i = 0; i < juego->config.cant_Jugadores; i++)
    {
        jugadorInicializar(&player, i, devolverEntrada(&juego->input[i]));
        vectorInsertarAlFinal(&juego->jugadores, &player);
    }

    // Define el icono de la ventana
    SDL_SetWindowIcon(juego->ventana, juego->icono);

    // Inicializa el estado del juego en el menu principal
    juego->estado = &estadoMenuPrin;
    juego->estadoPendiente = NULL;
    // Llama a la funcion de apertura del estado actual para cargar los recursos correspondientes a este estado y preparar el juego para este
    juego->estado->abrir(juego);

    // Inicializa la flag para el loop principal del juego
    juego->corriendo = 1;
    return err;
}

tError juego_procesar_eventos(tJuego *juego)
{
    tError err = TODO_OK;

    // Busca si hay un evento
    while(SDL_PollEvent(&juego->event))
    {
        // Detiene el loop del juego si es SDL_QUIT
        if(juego->event.type == SDL_QUIT)
        {
            juego->corriendo = 0;
        }
        else
        {
            // Deriva el tratamiento del evento al estado actual del juego
            juego->estado->evento(juego);
        }
    }

    return err;
}

void juego_actualizar(tJuego *juego)
{
    // Deriva la actualizacion del juego al estado actual del juego
    juego->estado->actualizar(juego);

}

void juego_renderizar(tJuego *juego)
{
    // Deriva el renderizado del juego al estado actual del juego
    juego->estado->renderizar(juego);

}

void juego_aplicar_cambio_estado(tJuego *juego)
{
    // Da pie al nuevo estado de juego, cerrando el anterior y llamando a la apertura del pendiente
    juego->estado->cerrar(juego);
    juego->estadoPendiente->abrir(juego);
    // Asigna como estado actual al estado pendiente e señala al pendiente como vacio
    juego->estado = juego->estadoPendiente;
    juego->estadoPendiente = NULL;
}


void juego_destruir(tJuego *juego)
{
    juego->estado->cerrar(juego); // Cierra el estado actual del juego para liberar los recursos correspondientes a este


    // Destruye todas las texturas cargadas en juego_inicializar y cierra los subsistemas de SDL
    int i;
    for(i = 0; i < FB_GAME_CANT; i++)
    {
        if(juego->eFramebuffersGame[i]){
            SDL_DestroyTexture(juego->eFramebuffersGame[i]);
        }
    }

    for(i = 0; i < FB_MENU_CANT; i++)
    {
        if(juego->eFramebuffersMenu[i]){
            SDL_DestroyTexture(juego->eFramebuffersMenu[i]);
        }
    }

    for(i = 0; i < FB_OPTIONS_CANT; i++)
    {
        if(juego->eFramebuffersOptions[i]){
            SDL_DestroyTexture(juego->eFramebuffersOptions[i]);
        }
    }

    for(i = 0; i < FB_PAUSE_CANT; i++)
    {
        if(juego->eFramebuffersPause[i]){
            SDL_DestroyTexture(juego->eFramebuffersPause[i]);
        }
    }

    for(i = 0; i < CANT_BACKGROUND; i++)
    {
        if(juego->background[i]){
            SDL_DestroyTexture(juego->background[i]);
        }
    }

    for(i = 0; i < CANT_BUTTON; i++)
    {
        if(juego->button[i]){
            SDL_DestroyTexture(juego->button[i]);
        }
    }

    if(juego->buttong)
    {
        SDL_DestroyTexture(juego->buttong);
    }

    if(juego->optionsRectangle)
    {
        SDL_DestroyTexture(juego->optionsRectangle);
    }


    for(i = 0; i < CANT_GAME_IND; i++)
    {
        if(juego->indicadorPlayer[i]){
            SDL_DestroyTexture(juego->indicadorPlayer[i]);
        }
    }

    for(i = 0; i < CARTAS_CANT; i++)
    {
        if(juego->SET1Texture[i]){
            SDL_DestroyTexture(juego->SET1Texture[i]);
        }
    }

    for(i = 0; i < CARTAS_CANT; i++)
    {
        if(juego->SET2Texture[i]){
            SDL_DestroyTexture(juego->SET2Texture[i]);
        }
    }

    for(i = 0; i < CARTAS_CANT; i++)
    {
        if(juego->SET3Texture[i]){
            SDL_DestroyTexture(juego->SET3Texture[i]);
        }
    }

    SDL_FreeSurface(juego->icono);

    if(juego->fuenteTTFChico){
        texto_destruir_fuente(juego->fuenteTTFChico);
    }

    if(juego->fuenteTTF){
        texto_destruir_fuente(juego->fuenteTTF);
    }

    if(juego->fuenteTTFTitulo){
        texto_destruir_fuente(juego->fuenteTTFTitulo);
    }

    if (juego->audioInicializado)
    {
        for(i = 0; i < CANT_SND; i++)
        {
            sonidos_destruir(juego->sndJuego[i]);
        }
        sonidos_finalizar();
    }

    texto_finalizar();

    imagenes_finalizar();

    if(juego->renderer){
        SDL_DestroyRenderer(juego->renderer);
    }

    if(juego->ventana){
        SDL_DestroyWindow(juego->ventana);
    }

}





