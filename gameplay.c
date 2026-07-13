#include "gameplay.h"

#include <stdlib.h>
#include <time.h>
#include "juego.h"
#include "graficos.h"
#include "menu.h"


static void procesar_teclas(tJuego *j);
static void procesar_mouse(tJuego *j);
static SDL_Texture* _hud_callback_actualizar_puntaje(SDL_Renderer *renderer, void *dato, void *datoNuevo, void *extra);
static SDL_Texture* _hud_callback_actualizar_indicadorTurno(SDL_Renderer *renderer, void *dato, void *datoNuevo, void *extra);
void dibujar_bandera(SDL_Renderer *renderer);

void abrirJuego(tJuego* j)
{
    if(j->estado != &estadoPausa)
    {

        vectorCrear(&j->jugadores, sizeof(Jugador));

        int i;

        Jugador jugador;

        printf("P1: %s P2: %s\n", j->config.n1, j->config.n2);

        jugadorInicializar(&jugador, Jugador_1, j->config.n1);
        vectorInsertarAlFinal(&j->jugadores, &jugador);
        texto_crear_textura(j->renderer,
                            &j->textoGamePlayerName[TXT_GAME_P1NAME],
                            j->fuenteTTF,
                            j->config.n1,
                            COLOR_GOLD_BRIGHT,
                            posX(14.55),
                            posY(0)
        );

        j->hudGame[HUD_GAME_P1_POINTS] = hud_inicializar(j->renderer,
                                                         posX(13.1),
                                                         posY(-0.02),
                                                         &(int32_t){0},
                                                         sizeof(int32_t),
                                                         j->fuenteTTFChico,
                                                         true,
                                                         _hud_callback_actualizar_puntaje,
                                                         NULL
        );

        hud_actualizar_dato(j->hudGame[HUD_GAME_P1_POINTS], &(int32_t){0});

        if(j->config.cant_Jugadores > 1)
        {
            jugadorInicializar(&jugador, Jugador_2, j->config.n2);
            vectorInsertarAlFinal(&j->jugadores, &jugador);
            texto_crear_textura(j->renderer,
                                &j->textoGamePlayerName[TXT_GAME_P2NAME],
                                j->fuenteTTF,
                                j->config.n2,
                                COLOR_GOLD_BRIGHT,
                                posX(1),
                                posY(8)
            );
            j->hudGame[HUD_GAME_P2_POINTS] = hud_inicializar(j->renderer,
                                                         posX(2.5),
                                                         posY(8.04),
                                                         &(int32_t){0},
                                                         sizeof(int32_t),
                                                         j->fuenteTTFChico,
                                                         true,
                                                         _hud_callback_actualizar_puntaje,
                                                         NULL
            );
            hud_actualizar_dato(j->hudGame[HUD_GAME_P2_POINTS], &(int32_t){0});
        }



        vectorCrear(&j->cartas, sizeof(Carta));

        Carta carta;

        //SDL_Texture** setActual = j->config.conjunto.numeroSet == SET_BILLETES? j->SET1Texture : j->SET2Texture;
        SDL_Texture** setActual;

        switch(j->config.numeroSet)
        {
            case SET_BILLETES:
                setActual = j->SET1Texture;
                break;

            case SET_ESCUDOS:
                setActual = j->SET2Texture;
                break;

            case SET_COSTUMBRES:
                setActual = j->SET3Texture;
                break;
        }


        for (i = 0; i < j->config.cant_Cartas / 2; i++)
        {

            crear_carta(&carta, i * 2, i, setActual[i], setActual[CartaReverso]);
            vectorInsertarAlFinal(&j->cartas, &carta);
            crear_carta(&carta, i * 2 + 1, i, setActual[i], setActual[CartaReverso]);
            vectorInsertarAlFinal(&j->cartas, &carta);


        }

        vectorMezclar(&j->cartas);

        calcular_layout_cartas(setActual[CartaReverso],
                                j->config.filasGrilla,
                                j->config.columnasGrilla,
                                posY(1.5f), //150,
                                posY(7.5f),
                                posX(1.0f),//350,
                                posX(12.0f),
                                0.1f,
                                &j->layout
        );

        contextoActPosicion context = {
            .indice = 0,
            .cantColumnas = j->config.columnasGrilla,
            .margenX = j->layout.margenX,
            .margenY = j->layout.margenY,
            .IniX = j->layout.startX,
            .IniY = j->layout.startY,
            .cartaW = j->layout.cartaW,
            .cartaH = j->layout.cartaH,
        };

        for(int i = 0; i < j->config.filasGrilla; i++)
        {
            for(int t = 0; t < j->config.columnasGrilla; t++)
            {
                context.grilla[i][t] = j->config.grilla[i][t];
            }
        }

        vectorRecorrer(&j->cartas, actualizar_posicion_carta, &context);

        j->indiceTurno = Jugador_1;

        j->hudIndicadorTurno = hud_inicializar(j->renderer,
                                               posX(7.5),
                                               posY(0.75),
                                               &j->indiceTurno,
                                               sizeof(j->indiceTurno),
                                               j->textoGamePlayerName,
                                               false,
                                               _hud_callback_actualizar_indicadorTurno,
                                               NULL
        );

        hud_actualizar_dato(j->hudIndicadorTurno, &j->indiceTurno);

        crear_texto_game(j);
        crear_botones_game(j);

        j->contCoincidencias = 0;
        j->contTurno = 0;

        j->estadoMemoria = WAIT_FIRST;
    }


    printf("Abriendo juego\n");
}

void eventosJuego(tJuego* j)
{
    if (j->event.type == SDL_MOUSEMOTION || j->event.type == SDL_MOUSEBUTTONDOWN || j->event.type == SDL_MOUSEBUTTONUP)
    {
            procesar_mouse(j);
    }
    if (j->event.type == SDL_KEYDOWN)
    {
            procesar_teclas(j);
    }

}

void actualizarJuego(tJuego* j)
{
    if(j->gameButtons[GAME_BUTTON_PAUSE].clickeado)
    {
        j->estadoPendiente = &estadoPausa;
    }

    if(j->gameButtons[GAME_BUTTON_UNMIX].clickeado)
    {
        vectorOrdenar(&j->cartas, QSORT, cmpCarta);

        contextoActPosicion context = {
            .indice = 0,
            .cantColumnas = j->config.columnasGrilla,
            .margenX = j->layout.margenX,
            .margenY = j->layout.margenY,
            .IniX = j->layout.startX,
            .IniY = j->layout.startY,
            .cartaW = j->layout.cartaW,
            .cartaH = j->layout.cartaH,
        };

        for(int i = 0; i < j->config.filasGrilla; i++)
        {
            for(int t = 0; t < j->config.columnasGrilla; t++)
            {
                context.grilla[i][t] = j->config.grilla[i][t];
            }
        }

        vectorRecorrer(&j->cartas, actualizar_posicion_carta, &context);
    }

    if(j->gameButtons[GAME_BUTTON_MIX].clickeado)
    {
        vectorMezclar(&j->cartas);

        contextoActPosicion context = {
            .indice = 0,
            .cantColumnas = j->config.columnasGrilla,
            .margenX = j->layout.margenX,
            .margenY = j->layout.margenY,
            .IniX = j->layout.startX,
            .IniY = j->layout.startY,
            .cartaW = j->layout.cartaW,
            .cartaH = j->layout.cartaH,
        };

        for(int i = 0; i < j->config.filasGrilla; i++)
        {
            for(int t = 0; t < j->config.columnasGrilla; t++)
            {
                context.grilla[i][t] = j->config.grilla[i][t];
            }
        }

        vectorRecorrer(&j->cartas, actualizar_posicion_carta, &context);
    }

        if(j->gameButtons[GAME_BUTTON_ROL].clickeado)
    {
        carta_rol(&j->cartas);

        contextoActPosicion context = {
            .indice = 0,
            .cantColumnas = j->config.columnasGrilla,
            .margenX = j->layout.margenX,
            .margenY = j->layout.margenY,
            .IniX = j->layout.startX,
            .IniY = j->layout.startY,
            .cartaW = j->layout.cartaW,
            .cartaH = j->layout.cartaH,
        };

        for(int i = 0; i < j->config.filasGrilla; i++)
        {
            for(int t = 0; t < j->config.columnasGrilla; t++)
            {
                context.grilla[i][t] = j->config.grilla[i][t];
            }
        }

        vectorRecorrer(&j->cartas, actualizar_posicion_carta, &context);

    }
    if(j->gameButtons[GAME_BUTTON_ROR].clickeado)
    {
        carta_ror(&j->cartas);

        contextoActPosicion context = {
            .indice = 0,
            .cantColumnas = j->config.columnasGrilla,
            .margenX = j->layout.margenX,
            .margenY = j->layout.margenY,
            .IniX = j->layout.startX,
            .IniY = j->layout.startY,
            .cartaW = j->layout.cartaW,
            .cartaH = j->layout.cartaH,
        };

        for(int i = 0; i < j->config.filasGrilla; i++)
        {
            for(int t = 0; t < j->config.columnasGrilla; t++)
            {
                context.grilla[i][t] = j->config.grilla[i][t];
            }
        }

        vectorRecorrer(&j->cartas, actualizar_posicion_carta, &context);
    }

    if(j->estadoMemoria == FINISHED && j->gameButtons[GAME_BUTTON_MENUPRIN].clickeado)
    {
        j->estadoPendiente = &estadoMenuPrin;
    }


    if(j->estadoMemoria == CHECK)
    {
        if(SDL_GetTicks() - j->revealTime > 1000)
        {
            contextoPuntaje contexto = {
                            5 + rand() % 6,
                            j->indiceTurno
            };

            vectorRecorrer(&j->cartas, actualizarCarta, j);

            if(j->firstCard == j->secondCard)
            {


                vectorRecorrer(&j->jugadores, actualizarRachaPuntaje, &contexto);
                hud_actualizar_dato(j->hudGame[j->indiceTurno], &contexto.puntaje);
                sonidos_reproducir(j->config.numeroSet == SET_BILLETES? j->sndJuego[SND_ACIERTO_BILLETE] : j->sndJuego[SND_ACIERTO_GENERICO], 1);
            }
            else
            {
                contexto.puntaje = 0;

                vectorRecorrer(&j->jugadores, actualizarRachaPuntaje, &contexto);
                cambiarTurno(&j->indiceTurno, 1, j->config.cant_Jugadores);
                hud_actualizar_dato(j->hudIndicadorTurno, &j->indiceTurno);
                sonidos_reproducir(j->sndJuego[SND_INCORRECTO], 1);
            }

            j->firstCard = j->secondCard = -1;


            j->estadoMemoria = WAIT_FIRST;
        }
    }

    int i;

    for(i = 0; i < CANT_GAME_BUTTON; i++)
    {
        actualizarBoton(&j->gameButtons[i]);
    }

    if(j->contCoincidencias == j->config.cant_Cartas)
    {
        j->estadoMemoria = FINISHED;
        j->contCoincidencias = 0;
        sonidos_reproducir(j->sndJuego[SND_VICTORIA], 1);
    }




    return;
}

void renderizarJuego(tJuego* j)
{
    int i;
    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_BACKGROUND]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    graficos_dibujar_textura(j->renderer, j->background[GAME_BACKGROUND], NULL, 0, 0, 1, 1, 0.0f, 0, 0, POS_LUCORNER);

    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_BUTTONS]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    for(i = 0; i < (CANT_GAME_BUTTON - 1); i++)
    {
        button_dibujar_textura(j->renderer, &j->gameButtons[i]);
    }



    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_CARTA]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    vectorRecorrer(&j->cartas, imprimirCarta, j->renderer);

    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_INDICADOR]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    graficos_dibujar_textura(j->renderer, j->indicadorPlayer[GAME_IND_P1], NULL, posX(14), posY(0), 1, 1, 0, 0 ,0, POS_CENTER);

    if(j->config.cant_Jugadores > 1)
    {

        switch(j->indiceTurno)
        {
            case Jugador_1: SDL_APLICAR_COLOR(j->optionsRectangle, COLOR_AZUL); break;
            case Jugador_2: SDL_APLICAR_COLOR(j->optionsRectangle, COLOR_ROJO); break;
        }
        graficos_dibujar_textura(j->renderer, j->optionsRectangle, NULL, posX(7.5), posY(0.75), 0.7f, 0.4f, 0, 0, 0, POS_CENTER);
        SDL_APLICAR_COLOR(j->optionsRectangle, BNORMAL);

        hud_dibujar(j->hudIndicadorTurno);

        for(i = 1; i < j->config.cant_Jugadores; i++)
        {
            graficos_dibujar_textura(j->renderer, j->indicadorPlayer[i], NULL, posX(1.54) * i , posY(8), 1, 1, 0, 0 ,0, POS_CENTER);
        }
    }


    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_HUD]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    hud_dibujar(j->hudGame[HUD_GAME_P1_POINTS]);

    if(j->config.cant_Jugadores > 1)
    {
        for (i = 1; i < j->config.cant_Jugadores; i++)
        {
            hud_dibujar(j->hudGame[i]);
        }
    }

    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_PLAYERNAME]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    texto_dibujar_textura(j->renderer, &j->textoGamePlayerName[Jugador_1], NULL, 1, 0.95f, 0, 0, 0, POS_CENTER);

    if(j->config.cant_Jugadores > 1)
    {
        for(i = 1; i < j->config.cant_Jugadores; i++)
        {
            texto_dibujar_textura(j->renderer, &j->textoGamePlayerName[i], NULL, 1, 0.95f, 0, 0, 0, POS_CENTER);
        }
    }

    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_TXT]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    texto_dibujar_textura(j->renderer, &j->textoGame[TXT_GAME_ORDENAR], NULL, 1.0f, 1.0f, 0, 0, 0, POS_CENTER);
    texto_dibujar_textura(j->renderer, &j->textoGame[TXT_GAME_MEZCLAR], NULL, 1.0f, 1.0f, 0, 0, 0, POS_CENTER);

    if(j->estadoMemoria == FINISHED)
    {
        graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_GANADOR]);
        graficos_borrar_pantalla(j->renderer, &XBLANCO);
        dibujar_bandera(j->renderer);
        texto_dibujar_textura(j->renderer, &j->textoGame[TXT_GAME_GANASTE], NULL, 2, 2, 0, 0, 0, POS_CENTER);
        button_dibujar_textura(j->renderer, &j->gameButtons[GAME_BUTTON_MENUPRIN]);
        texto_dibujar_textura(j->renderer, &j->textoGame[TXT_GAME_MENUPRIN], NULL, 1, 1, 0, 0, 0, POS_CENTER);
    }

    // graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[FB_GAME_GRILLA]);
    // graficos_borrar_pantalla(j->renderer, &XBLANCO);
    // graficos_dibujar_grilla(j->renderer);



    graficos_renderizar(j->renderer, j->eFramebuffersGame, FB_GAME_CANT);

}

void cerrarJuego(tJuego* j)
{
    int i;
    for(i = 0; i < FB_GAME_CANT; i++)
    {
        graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[i]);
        graficos_borrar_pantalla(j->renderer, &XBLANCO);
    }

    graficos_renderizar(j->renderer, j->eFramebuffersGame, FB_GAME_CANT);


    if(j->estadoPendiente != &estadoPausa)
    {
        for(i = 0; i < CANT_GAME_BUTTON; i++)
        {
            button_destruir(&j->gameButtons[i]);
        }

        hud_destruir(j->hudIndicadorTurno);

        hud_destruir(j->hudGame[HUD_GAME_P1_POINTS]);

        if(j->config.cant_Jugadores > 1)
        {
            hud_destruir(j->hudGame[HUD_GAME_P2_POINTS]);
        }

        vectorDestruir(&j->cartas);
        vectorDestruir(&j->jugadores);

        j->estadoMemoria = WAIT_FIRST;

        for(i = 0; i < CANT_GAME_TXT; i++)
        {
            texto_destruir_textura(&j->textoGame[i]);
        }

        for(i = 0; i < CANT_GAME_NAME_TXT; i++)
        {
           texto_destruir_textura(&j->textoGamePlayerName[i]);
        }

        printf("Cerrando juego...\n");
    }else
    {
        printf("Pausando juego...\n");
    }

    return;
}


static void procesar_mouse(tJuego *j)
{
    int i;
    if(j->event.type == SDL_MOUSEMOTION)
    {
        int mouseX = j->event.motion.x;
        int mouseY = j->event.motion.y;

        j->mouse.x = mouseX;
        j->mouse.y = mouseY;

        for(i = 0; i < CANT_GAME_BUTTON; i++)
        {
            j->gameButtons[i].hovered = SDL_PointInRect(&j->mouse, &j->gameButtons[i].rectFinal);
        }

        vectorRecorrer(&j->cartas, mouseSobreCarta, &j->mouse);

    }

    if(j->event.type == SDL_MOUSEBUTTONDOWN  && j->event.button.button == SDL_BUTTON_LEFT)
    {
        for(i = 0; i < CANT_GAME_BUTTON; i++)
        {
            if(j->gameButtons[i].hovered)
            {
                j->gameButtons[i].esApretado = 1;
            }
        }

        vectorRecorrer(&j->cartas, leftClickEnCarta, j);
    }


    if(j->event.type == SDL_MOUSEBUTTONUP && j->event.button.button == SDL_BUTTON_LEFT)
    {

        for(i = 0; i < CANT_GAME_BUTTON; i++)
        {
            if(j->gameButtons[i].esApretado)
            {
                j->gameButtons[i].clickeado = 1;
                sonidos_reproducir(j->sndJuego[SND_BOTON], 1);
            }

            j->gameButtons[i].esApretado = 0;
        }

        vectorRecorrer(&j->cartas, noClickEnCarta, NULL);
    }
}

static void procesar_teclas(tJuego *j)
{
    switch (j->event.key.keysym.sym)
    {
        case SDLK_ESCAPE:
            j->estadoPendiente = &estadoPausa;
            break;
        case SDLK_p:
            j->estadoPendiente = &estadoPausa;
            break;

    }
}

void dibujar_bandera(SDL_Renderer *renderer)
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    SDL_Rect rect = {
                    posX(-0.5),
                    posY(2),
                    window_w,
                    posY(1.5)
    };

    SDL_SetRenderDrawColor(renderer, COLOR_ARG_CELESTE.r, COLOR_ARG_CELESTE.g, COLOR_ARG_CELESTE.b, 240);
    SDL_RenderFillRect(renderer, &rect);

    rect.y = posY(5);
    SDL_RenderFillRect(renderer, &rect);
    rect.y = posY(3.5);
    SDL_SetRenderDrawColor(renderer, COLOR_ARG_BLANCO.r, COLOR_ARG_BLANCO.g, COLOR_ARG_BLANCO.b, 240);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

tError crear_texto_game(tJuego* juego)
{
    tError err;

    if((err = texto_crear_textura(juego->renderer,
                                &juego->textoGame[TXT_GAME_GANASTE],
                                juego->fuenteTTFTitulo,
                                "GANASTE",
                                COLOR_GOLD_BRIGHT,
                                posX(7.5),
                                posY(4.25)
    )))
    {
        return err;
    }

    if((err = texto_crear_textura(juego->renderer,
                                &juego->textoGame[TXT_GAME_MENUPRIN],
                                juego->fuenteTTF,
                                "IR  AL  MENU  PRINCIPAL",
                                COLOR_GOLD_BRIGHT,
                                posX(7.5),
                                posY(6) -15
    )))
    {
        return err;
    }

    if((err = texto_crear_textura(juego->renderer,
                                &juego->textoGame[TXT_GAME_ORDENAR],
                                juego->fuenteTTFChico,
                                "ORDENAR",
                                COLOR_GOLD_BRIGHT,
                                posX(1.7f),
                                posY(0.5f)
    )))
    {
        return err;
    }

    if((err = texto_crear_textura(juego->renderer,
                                &juego->textoGame[TXT_GAME_MEZCLAR],
                                juego->fuenteTTFChico,
                                "MEZCLAR",
                                COLOR_GOLD_BRIGHT,
                                posX(3.0f),
                                posY(0.5f)
    )))
    {
        return err;
    }


    return TODO_OK;
}


void crear_botones_game(tJuego* juego)
{
    button_crear(&juego->gameButtons[GAME_BUTTON_PAUSE],
                            juego->buttong,
                            posX(0.5),
                            posY(0.5),
                            1.0f,
                            0.7f,
                            0,
                            0,
                            0
    );


    button_crear(&juego->gameButtons[GAME_BUTTON_MENUPRIN],
                         juego->button[MENU_BUTTON],
                         posX(7.5),
                         posY(5.83),
                         1.4,
                         1.2,
                         0,
                         0,
                         0
    );

    button_crear(&juego->gameButtons[GAME_BUTTON_UNMIX],
                        juego->button[MENU_BUTTON],
                        posX(1.7f),
                        posY(0.5f),
                        0.37f,
                        1.3f,
                        0,
                        0,
                        0
    );

    button_crear(&juego->gameButtons[GAME_BUTTON_MIX],
                        juego->button[MENU_BUTTON],
                        posX(3.0f),
                        posY(0.5f),
                        0.37f,
                        1.3f,
                        0,
                        0,
                        0
    );

    button_crear(&juego->gameButtons[GAME_BUTTON_ROL],
                         juego->button[OPTIONS_ARROW_BUTTON],
                         posX(4.2f),
                         posY(0.5f),
                         0.5f,
                         1.0f,
                         0,
                         SDL_FLIP_HORIZONTAL,
                         0
    );

    button_crear(&juego->gameButtons[GAME_BUTTON_ROR],
                         juego->button[OPTIONS_ARROW_BUTTON],
                         posX(5.2f),
                         posY(0.5f),
                         0.5f,
                         1.0f,
                         0,
                         0,
                         0
    );



}

Estado estadoJuego = {
    abrirJuego,
    eventosJuego,
    actualizarJuego,
    renderizarJuego,
    cerrarJuego,
};

static SDL_Texture* _hud_callback_actualizar_puntaje(SDL_Renderer *renderer, void *dato, void *datoNuevo, void *extra)
{
    unsigned int nuevoDato = *(int32_t*)dato + *(int32_t*)datoNuevo;
    *(int32_t*)dato = nuevoDato;
    char buffer[TAM_BUFFER];

    snprintf(buffer, sizeof(buffer), "%u", nuevoDato);


    tTexto t;

    texto_crear_textura(renderer, &t, extra, buffer, COLOR_GOLD_BRIGHT, 0, 0);

    SDL_Texture *textura = t.texto;

    return textura;

}

static SDL_Texture* _hud_callback_actualizar_indicadorTurno(SDL_Renderer *renderer, void *dato, void *datoNuevo, void *extra)
{
    // uint32_t nuevoIndice = *(uint32_t*)dato + *(uint32_t*)datoNuevo;
    // uint32_t nuevoIndice = *(uint32_t*)datoNuevo;

    // // if(nuevoIndice < 0)
    // // {
    // //     nuevoIndice = CANT_GAME_NAME_TXT - 1;
    // // }
    // // if(nuevoIndice >= CANT_GAME_NAME_TXT)
    // // {
    // //     nuevoIndice = 0;
    // // }

    // (void)renderer;

    // memcpy(dato, &nuevoIndice, sizeof(int32_t));

    // tTexto *t = extra;

    // SDL_Texture *nuevaTextura = t[nuevoIndice].texto;

    // return nuevaTextura;
    uint32_t nuevoIndice = *(uint32_t*)datoNuevo;
    memcpy(dato, &nuevoIndice, sizeof(int32_t));

    tTexto *nombres = extra;


    return nombres[nuevoIndice].texto;
}

