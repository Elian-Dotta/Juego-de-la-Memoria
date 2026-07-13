/**
 * @file opciones.c
 * @author Dotta, Elian Agustin
 * @brief Implementacion del estado de opciones del juego.
 * @version 1.0
 * @date 2026-03-06
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "opciones.h"

#include <stdlib.h>
#include <time.h>
#include "juego.h"
#include "graficos.h"
#include "menu.h"
#include "errores.h"


////FUNCIONES PRIVADAS////

/// @brief Callback para actualizar el set de opciones en el HUD.
/// @param renderer Puntero al renderer del juego
/// @param dato Dato actual del HUD, en este caso un entero que indica el indice del set seleccionado actualmente
/// @param datoNuevo Dato nuevo para actualizar el HUD, en este caso un entero que indica el cambio a aplicar al indice del set seleccionado (ej: -1 para ir al set anterior, 1 para ir al siguiente)
/// @param extra Informacion extra, en este caso un puntero al array de textos de opciones para acceder a las texturas de los sets
/// @return
SDL_Texture* _hud_callback_actualizar_setOpt(SDL_Renderer *renderer, void *dato, void *datoNuevo, void *extra);

SDL_Texture* _hud_callback_actualizar_setOpt(SDL_Renderer *renderer, void *dato, void *datoNuevo, void *extra)
{
    int32_t nuevoIndice = *(int32_t*)dato + *(int32_t*)datoNuevo;

    if(nuevoIndice < 0)
    {
        nuevoIndice = CANT_OPTIONS_SET_TXT - 1;
    }
    if(nuevoIndice >= CANT_OPTIONS_SET_TXT)
    {
        nuevoIndice = 0;
    }

    (void)renderer;

    memcpy(dato, &nuevoIndice, sizeof(int32_t));

    tTexto *t = extra;

    SDL_Texture *nuevaTextura = t[nuevoIndice].texto;

    return nuevaTextura;
}


/**
 * @brief Procesa los eventos del mouse para el estado de opciones.
 *
 * @param j Puntero al juego.
 */
void procesarMouse(tJuego* j);

//FUNCIONES PUBLICAS////

void abrirOpciones(tJuego* j)
{
    crear_texto_opt(j);
    crear_botones_opt(j);
    crear_entradas_opt(j);
    crear_hud_opt(j);

    printf("Abriendo opciones\n");
}

void eventosOpciones(tJuego* j)
{
    if(j->event.type == SDL_MOUSEMOTION || j->event.type == SDL_MOUSEBUTTONDOWN || j->event.type == SDL_MOUSEBUTTONUP)
    {
        procesarMouse(j);
    }

    /*------------------------INPUT-ACTIVO-------------------------*/
    if(!j->inputActual)
    {
        return;
    }

    /*----------------------------TEXTO----------------------------*/
    if(j->event.type == SDL_TEXTINPUT)
    {
        if((strlen(j->inputActual->buffer) + strlen(j->event.text.text)) < LARGO_NOMBRE)
        {
            strcat(j->inputActual->buffer, j->event.text.text);
        }
    }

    /*----------------------TECLAS-ESPECIALES----------------------*/
    if (j->event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = j->event.key.keysym.sym;

        switch(key)
        {
            case SDLK_BACKSPACE:
                size_t len = strlen(j->inputActual->buffer);
                if(len > 0)
                {
                    j->inputActual->buffer[len - 1] = '\0';
                }
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                printf("Texto correctamente ingresado: %s\n", j->inputActual->buffer);
                desactivarInput(j);
                break;
            case SDLK_ESCAPE:
                j->inputActual->buffer[0] = '\0';
                desactivarInput(j);
                printf("Cancelando entrada de texto\n");
                break;
            default:
                return;
        }
    }

    return;
}

void actualizarOpciones(tJuego* j)
{
    int i;
    int set = j->config.numeroSet;
    int cant_Jugadores = j->config.cant_Jugadores;
    int grilla[MAX_FILAS][MAX_COLUMNAS];

    int c, f;

    for(f = 0; f < MAX_FILAS; f++)
    {
        for(c = 0; c < MAX_COLUMNAS; c++)
        {
            if(j->optGrid[f][c].clickeado)
            {
                j->optGrid[f][c].toggle = !j->optGrid[f][c].toggle;
            }

            actualizarBoton(&j->optGrid[f][c]);

            grilla[f][c] = j->optGrid[f][c].toggle;

        }
    }

    if(j->optionButtons[OPTIONS_BUTTON_LASET].clickeado)
    {
        hud_actualizar_dato(j->hudSetOptions, &(int32_t) {-1});
        set--;
        if(set < 0)
        {
            set = CANT_OPTIONS_SET_TXT - 1;
        }
    }
    if(j->optionButtons[OPTIONS_BUTTON_RASET].clickeado)
    {
        hud_actualizar_dato(j->hudSetOptions, &(int32_t) {1});
        set++;
        if(set >= CANT_OPTIONS_SET_TXT)
        {
            set = 0;
        }
    }

    if(j->optionButtons[OPTIONS_BUTTON_1P].clickeado)
        cant_Jugadores = 1;



    if(j->optionButtons[OPTIONS_BUTTON_2P].clickeado)
        cant_Jugadores = 2;


    switch(cant_Jugadores)
    {
        case 1: j->optionButtons[OPTIONS_BUTTON_2P].toggle = false;
                j->optionButtons[OPTIONS_BUTTON_1P].toggle = true;
                break;
        case 2: j->optionButtons[OPTIONS_BUTTON_2P].toggle = true;
                j->optionButtons[OPTIONS_BUTTON_1P].toggle = false;
                break;
    }

    if(j->optionButtons[OPTIONS_BUTTON_MENU].clickeado && (j->config.n2[0] != '\0' || cant_Jugadores == 1) && j->config.n1[0] != '\0')
        j->estadoPendiente = &estadoMenuPrin;


    if(j->inputActual)
    {
        int activoIndex = j->inputActual - j->input;

        if(activoIndex >= cant_Jugadores)
        {
            desactivarInput(j);
        }
    }

    for(i = 0; i < cant_Jugadores; i++)
    {
        if(j->input[i].botonEntrada.clickeado)
            activarInput(j, &j->input[i]);
    }


    for(i = 0; i < CANT_OPTIONS_BUTTON; i++)
    {
        actualizarBoton(&j->optionButtons[i]);
    }

    for(i = 0; i < cant_Jugadores; i++)
    {
        actualizarBoton(&j->input[i].botonEntrada);
    }


    if(config_actualizar(&j->config, grilla,set, cant_Jugadores, devolverEntrada(&j->input[Jugador_1]), devolverEntrada(&j->input[Jugador_2])))
    {
        config_default(&j->config);
        printf("%s\n", errores_obtener_detalle(ERR_CONFIG_INVALIDA));

    }
    config_guardar(&j->config, CONFIG_RUTA);

    return;
}

void renderizarOpciones(tJuego* j)
{

    int i, c, f;
    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersOptions[FB_OPTIONS_BACKGROUND]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    graficos_dibujar_textura(j->renderer, j->background[OPTIONS_BACKGROUND], NULL, 0, 0, 1, 1, 0.0f, 0, 0, POS_LUCORNER);


    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersOptions[FB_OPTIONS_RECTANGLE]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    graficos_dibujar_textura(j->renderer, j->optionsRectangle, NULL, posX(7.5), posY(5), 0.82, 0.5, 0, 0, 0, POS_CENTER);

    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersOptions[FB_OPTIONS_BUTTONS]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    for(i = 0; i < CANT_OPTIONS_BUTTON; i++)
    {
        button_dibujar_textura(j->renderer, &j->optionButtons[i]);
    }

    for(f = 0; f < MAX_FILAS; f++)
    {
        for(c = 0; c < MAX_COLUMNAS; c++)
        {
            button_dibujar_textura(j->renderer, &j->optGrid[f][c]);
        }
    }

    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersOptions[FB_OPTIONS_TXT]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    for(i = 0; i < CANT_OPTIONS_TXT; i++)
    {
        texto_dibujar_textura(j->renderer, &j->textoOptions[i], NULL, 1, 1, 0, 0, 0, POS_CENTER);
    }


    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersOptions[FB_OPTIONS_HUD]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    hud_dibujar(j->hudSetOptions);


    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersOptions[FB_OPTIONS_INPUT]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    for(i = 0; i < j->config.cant_Jugadores; i++)
    {
        dibujarBotonEntrada(j->renderer, &j->input[i]);
        dibujarEntradaEnPantalla(j->renderer, &j->input[i]);
    }

    graficos_renderizar(j->renderer, j->eFramebuffersOptions, FB_OPTIONS_CANT);

}

void cerrarOpciones(tJuego* j)
{
    int i;
    for(i = 0; i < FB_OPTIONS_CANT; i++)
    {
        graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersOptions[i]);
        graficos_borrar_pantalla(j->renderer, &XBLANCO);
    }

    graficos_renderizar(j->renderer, j->eFramebuffersOptions, FB_OPTIONS_CANT);

    for(i = 0; i < CANT_OPTIONS_BUTTON; i++)
    {
        button_destruir(&j->optionButtons[i]);
    }

    int c, f;
    for(f = 0; f < MAX_FILAS; f++)
    {
        for(c = 0; c < MAX_COLUMNAS; c++)
        {
            button_destruir(&j->optGrid[f][c]);
        }
    }

    hud_destruir(j->hudSetOptions);

    for(i = 0; i < CANT_OPTIONS_SET_TXT; i++)
    {
        texto_destruir_textura(&j->textoOptionsSet[i]);
    }

    for(i = 0; i < CANT_OPTIONS_TXT; i++)
    {
        texto_destruir_textura(&j->textoOptions[i]);
    }

    printf("Cerrando opciones...\n");
}


void procesarMouse(tJuego* j)
{
    int i, c, f;

    if(j->event.type == SDL_MOUSEMOTION)
    {
        int mouseX = j->event.motion.x;
        int mouseY = j->event.motion.y;

        j->mouse.x = mouseX;
        j->mouse.y = mouseY;

        for(i = 0; i < CANT_OPTIONS_BUTTON; i++)
        {
            j->optionButtons[i].hovered = SDL_PointInRect(&j->mouse, &j->optionButtons[i].rectFinal);
        }

        for(f = 0; f < MAX_FILAS; f++)
        {
            for(c = 0; c < MAX_COLUMNAS; c++)
            {
                j->optGrid[f][c].hovered = SDL_PointInRect(&j->mouse, &j->optGrid[f][c].rectFinal);
            }
        }

        for(i = 0; i < j->config.cant_Jugadores; i++)
        {
            j->input[i].botonEntrada.hovered = SDL_PointInRect(&j->mouse, &j->input[i].botonEntrada.rectFinal);
        }
    }

    if(j->event.type == SDL_MOUSEBUTTONDOWN && j->event.button.button == SDL_BUTTON_LEFT)
    {
        for(i = 0; i < CANT_OPTIONS_BUTTON; i++)
        {
            if(j->optionButtons[i].hovered)
            {
                j->optionButtons[i].esApretado = 1;
            }
        }

        for(f = 0; f < MAX_FILAS; f++)
        {
            for(c = 0; c < MAX_COLUMNAS; c++)
            {
                if(j->optGrid[f][c].hovered)
                {
                    j->optGrid[f][c].esApretado = 1;
                }
            }
        }

        for(i = 0; i < j->config.cant_Jugadores; i++)
        {
            if(j->input[i].botonEntrada.hovered)
            {
                j->input[i].botonEntrada.esApretado = 1;
            }

        }
    }

    if(j->event.type == SDL_MOUSEBUTTONUP && j->event.button.button == SDL_BUTTON_LEFT)
    {
        for(i = 0; i < CANT_OPTIONS_BUTTON; i++)
        {
            if(j->optionButtons[i].esApretado && j->optionButtons[i].hovered)
            {
                j->optionButtons[i].clickeado = 1;
                sonidos_reproducir(j->sndJuego[SND_BOTON], 1);
            }

            j->optionButtons[i].esApretado = 0;
        }

        for(f = 0; f < MAX_FILAS; f++)
        {
            for(c = 0; c < MAX_COLUMNAS; c++)
            {
                if(j->optGrid[f][c].esApretado && j->optGrid[f][c].hovered)
                {
                    j->optGrid[f][c].clickeado = 1;
                    sonidos_reproducir(j->sndJuego[SND_BOTON], 1);
                }

                j->optGrid[f][c].esApretado = 0;
            }
        }

        bool clickEnInput = false;

        for(i = 0; i < j->config.cant_Jugadores; i++)
        {
            if(j->input[i].botonEntrada.esApretado && j->input[i].botonEntrada.hovered)
            {
                j->input[i].botonEntrada.clickeado = 1;
                clickEnInput = true;
                sonidos_reproducir(j->sndJuego[SND_BOTON], 1);

            }

            j->input[i].botonEntrada.esApretado = 0;
        }

        if(!clickEnInput && j->inputActual)
        {
            printf("Texto correctamente ingresado: %s\n", j->inputActual->buffer);
            desactivarInput(j);
        }

    }
}


void crear_botones_opt(tJuego* juego)
{
    button_crear(&juego->optionButtons[OPTIONS_BUTTON_LASET],
                         juego->button[OPTIONS_ARROW_BUTTON],
                         posX(5),
                         posY(5),
                         1,
                         1,
                         0,
                         SDL_FLIP_HORIZONTAL,
                         0
    );

    button_crear(&juego->optionButtons[OPTIONS_BUTTON_RASET],
                         juego->button[OPTIONS_ARROW_BUTTON],
                         posX(10),
                         posY(5),
                         1,
                         1,
                         0,
                         0,
                         0
    );

    button_crear(&juego->optionButtons[OPTIONS_BUTTON_1P],
                         juego->button[OPTIONS_SQUARE_BUTTON],
                         posX(6.75),
                         posY(6.3),
                         0.3,
                         0.3,
                         0,
                         0,
                         0
    );

    button_crear(&juego->optionButtons[OPTIONS_BUTTON_2P],
                         juego->button[OPTIONS_SQUARE_BUTTON],
                         posX(8.25),
                         posY(6.3),
                         0.3,
                         0.3,
                         0,
                         0,
                         0
    );

    button_crear(&juego->optionButtons[OPTIONS_BUTTON_MENU],
                        juego->button[MENU_BUTTON],
                        posX(7.5),
                        posY(7.33),
                        1.4,
                        1.4,
                        0,
                        0,
                        0
    );

    float iniX = posX(5.5);
    float iniY = posY(1.5);
    int rW, rH;

    SDL_QueryTexture(juego->optionsRectangle, NULL, NULL, &rW, &rH);

    rW *= 0.2f;
    rH *= 0.2f;

    int c, f;
    for(f = 0; f < MAX_FILAS; f++)
    {
        for(c = 0; c < MAX_COLUMNAS; c++)
        {
            button_crear(&juego->optGrid[f][c],
                         juego->optionsRectangle,
                         iniX + c * (rW + rW * 0.25f),
                         iniY + f * (rH + rH * 0.05f),
                         0.2f,
                         0.2f,
                         0,
                         0,
                         0
                         );

            if(juego->config.grilla[f][c] == 1)
            {
                juego->optGrid[f][c].toggle = true;
            }
        }
    }


}

tError crear_entradas_opt(tJuego *juego)
{
    inicializarEntrada(&juego->input[Jugador_1],
                       juego->button[MENU_BUTTON],
                       juego->fuenteTTF,
                       juego->config.n1,
                       COLOR_GOLD_BRIGHT,
                       posX(5),
                       posY(6.3),
                       1,
                       1,
                       0.6f,
                       1,
                       POS_CENTER
    );


    inicializarEntrada(&juego->input[Jugador_2],
                       juego->button[MENU_BUTTON],
                       juego->fuenteTTF,
                       juego->config.n2,
                       COLOR_GOLD_BRIGHT,
                       posX(10),
                       posY(6.3),
                       1,
                       1,
                       0.6f,
                       1,
                       POS_CENTER
    );

    juego->inputActual = NULL;

    return TODO_OK;
}

tError crear_texto_opt(tJuego* juego)
{
    tError err;

    if((err = texto_crear_textura(juego->renderer,
                                &juego->textoOptions[TXT_OPTIONS_MENUPRIN],
                                juego->fuenteTTF,
                                "IR  AL  MENU  PRINCIPAL",
                                COLOR_GOLD_BRIGHT,
                                posX(7.5),
                                posY(7.5) - 15
    )))
    {
        return err;
    }
    if((err = texto_crear_textura(juego->renderer,
                            &juego->textoOptionsSet[TXT_OPTIONS_SET_BILLETES],
                            juego->fuenteTTF,
                            "Set Billetes",
                            COLOR_GOLD_BRIGHT,
                            posX(0),
                            posY(0)

    )))
    {
        return err;
    }
    if((err = texto_crear_textura(juego->renderer,
                            &juego->textoOptionsSet[TXT_OPTIONS_SET_ESCUDOS],
                            juego->fuenteTTF,
                            "Set Escudos",
                            COLOR_GOLD_BRIGHT,
                            posX(0),
                            posY(0)

    )))
    {
        return err;
    }
    if((err = texto_crear_textura(juego->renderer,
                            &juego->textoOptionsSet[TXT_OPTIONS_SET_COSTUMBRES],
                            juego->fuenteTTF,
                            "Set Costumbres",
                            COLOR_GOLD_BRIGHT,
                            posX(0),
                            posY(0)

    )))
    {
        return err;
    }


    if((err = texto_crear_textura(juego->renderer,
                            &juego->textoOptions[TXT_OPTIONS_P1],
                            juego->fuenteTTF,
                            "J 1",
                            COLOR_GOLD_BRIGHT,
                            posX(6.75),
                            posY(6.28)

    )))
    {
        return err;
    }
    if((err = texto_crear_textura(juego->renderer,
                            &juego->textoOptions[TXT_OPTIONS_P2],
                            juego->fuenteTTF,
                            "J 2",
                            COLOR_GOLD_BRIGHT,
                            posX(8.25),
                            posY(6.28)

    )))
    {
        return err;
    }

    return TODO_OK;
}

tError crear_hud_opt(tJuego *juego)
{
    juego->hudSetOptions = hud_inicializar(juego->renderer,
                                                         posX(7.5),
                                                         posY(5),
                                                         &(int32_t){0},
                                                         sizeof(int32_t),
                                                         juego->textoOptionsSet,
                                                         false,
                                                         _hud_callback_actualizar_setOpt,
                                                         NULL
    );
    if(!juego->hudSetOptions)
    {
        return ERR_HUD_INICIALIZAR;
    }
    hud_actualizar_dato(juego->hudSetOptions, &juego->config.numeroSet);
    return TODO_OK;
}

/**
 * @brief Defincion del estado de opciones, con punteros a las funciones que manejan cada etapa del ciclo de vida de este estado.
 *
 */
Estado estadoOpciones = {
    abrirOpciones,
    eventosOpciones,
    actualizarOpciones,
    renderizarOpciones,
    cerrarOpciones,
};
