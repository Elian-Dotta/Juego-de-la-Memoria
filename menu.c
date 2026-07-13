#include "menu.h"
#include "juego.h"
#include "graficos.h"
#include "gameplay.h"
#include "button.h"



//MENU PRINCIPAL

void abrirMenuPrin(tJuego* j)
{
    crear_botones_menu(j);
    crear_texto_menu(j);


    printf("Abriendo menu principal\n");
}

void eventosMenuPrin(tJuego* j)
{
    int i;
    if (j->event.type == SDL_MOUSEMOTION)
    {
        int mouseX = j->event.motion.x;
        int mouseY = j->event.motion.y;

        SDL_Point mouse = {mouseX, mouseY};

        for(i = 0; i < CANT_MENU_BUTTON; i++)
        {
            j->menuButtons[i].hovered = SDL_PointInRect(&mouse, &j->menuButtons[i].rectFinal);
        }
    }

    if (j->event.type == SDL_MOUSEBUTTONDOWN)
    {
        for(i = 0; i < CANT_MENU_BUTTON; i++)
        {
            if (j->menuButtons[i].hovered && j->event.button.button == SDL_BUTTON_LEFT)
                {
                    j->menuButtons[i].esApretado = 1;
                    sonidos_reproducir(j->sndJuego[SND_BOTON], 1);
                }
        }
    }

    if (j->event.type == SDL_MOUSEBUTTONUP)
    {

        for(i = 0; i < CANT_MENU_BUTTON; i++)
        {
            j->menuButtons[i].esApretado = 0;

        }
    }
}

void actualizarMenuPrin(tJuego* j)
{
    if(j->menuButtons[MENU_BUTTON_JUGAR].esApretado)
    {
        j->estadoPendiente = &estadoJuego;
    }
    if(j->menuButtons[MENU_BUTTON_OPCIONES].esApretado)
    {
        j->estadoPendiente = &estadoOpciones;
    }
    if(j->menuButtons[MENU_BUTTON_SALIR].esApretado)
    {
        j->corriendo = 0;
    }
}

void renderizarMenuPrin(tJuego* j)
{
    int i;
    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersMenu[FB_MENU_BACKGROUND]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    graficos_dibujar_textura(j->renderer, j->background[MENU_BACKGROUND], NULL, 0, 0, 1, 1, 0.0f, 0, 0, 0);


    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersMenu[FB_MENU_BUTTONS]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    for(i = 0; i < CANT_MENU_BUTTON; i++)
    {
        button_dibujar_textura(j->renderer, &j->menuButtons[i]);
    }


    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersMenu[FB_MENU_TXT]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    for(i = 0; i < CANT_MENU_TXT; i++)
    {
        texto_dibujar_textura(j->renderer, &j->textoMenu[i], NULL, 1, 1, 0 ,0 , 0, POS_CENTER);
    }
    /*
    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersMenu[FB_MENU_GRILLA]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    graficos_dibujar_grilla(j->renderer);
    */
    graficos_renderizar(j->renderer, j->eFramebuffersMenu, FB_MENU_CANT);
}

void cerrarMenuPrin(tJuego* j)
{
    int i;
    for(i = 0; i < FB_MENU_CANT; i++)
    {
        graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersMenu[i]);
        graficos_borrar_pantalla(j->renderer, &XBLANCO);
    }

    graficos_renderizar(j->renderer, j->eFramebuffersMenu, FB_MENU_CANT);

    for(i = 0; i < CANT_MENU_BUTTON; i++)
    {
        button_destruir(&j->menuButtons[i]);
    }

    for(i = 0; i < CANT_MENU_TXT; i++)
    {
        texto_destruir_textura(&j->textoMenu[i]);
    }

    printf("Cerrando menu principal\n");
}





//PAUSA

void abrirPausa(tJuego* j)
{
    crear_botones_pausa(j);
    crear_texto_pausa(j);
    printf("Pausando juego...\n");
    return;
}

void eventosPausa(tJuego* j)
{
    int i;
    if (j->event.type == SDL_MOUSEMOTION)
    {
        int mouseX = j->event.motion.x;
        int mouseY = j->event.motion.y;

        j->mouse.x = mouseX;
        j->mouse.y = mouseY;

        for(i = 0; i < CANT_PAUSE_BUTTON; i++)
        {
            j->pauseButtons[i].hovered = SDL_PointInRect(&j->mouse, &j->pauseButtons[i].rectFinal);
        }
    }

    if (j->event.type == SDL_MOUSEBUTTONDOWN)
    {
        for(i = 0; i < CANT_PAUSE_BUTTON; i++)
        {
            if (j->pauseButtons[i].hovered && j->event.button.button == SDL_BUTTON_LEFT)
                {
                    j->pauseButtons[i].esApretado = 1;
                    sonidos_reproducir(j->sndJuego[SND_BOTON], 1);
                }
        }
    }

    if (j->event.type == SDL_MOUSEBUTTONUP)
    {

        for(i = 0; i < CANT_PAUSE_BUTTON; i++)
        {
            j->pauseButtons[i].esApretado = 0;
        }
    }
}

void actualizarPausa(tJuego* j)
{
    if(j->pauseButtons[PAUSE_BUTTON_VOLVER].esApretado)
    {
        j->estadoPendiente = &estadoJuego;
    }
    if(j->pauseButtons[PAUSE_BUTTON_OPCIONES].esApretado)
    {
        j->estadoPendiente = &estadoOpciones;
    }
    if(j->pauseButtons[PAUSE_BUTTON_MENU].esApretado)
    {
        j->estadoPendiente = &estadoMenuPrin;
    }
}

void renderizarPausa(tJuego* j)
{
    int i;
    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersPause[FB_PAUSE_BACKGROUND]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    graficos_dibujar_textura(j->renderer, j->background[PAUSE_BACKGROUND], NULL, 0, 0, 1, 1, 0.0f, 0, 0, POS_LUCORNER);

    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersPause[FB_PAUSE_BUTTONS]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    for(i = 0; i < CANT_PAUSE_BUTTON; i++)
    {
        button_dibujar_textura(j->renderer, &j->pauseButtons[i]);
    }

    graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersPause[FB_PAUSE_TXT]);
    graficos_borrar_pantalla(j->renderer, &XBLANCO);
    for(i = 0; i < CANT_PAUSE_TXT; i++)
    {
        texto_dibujar_textura(j->renderer, &j->textoPause[i], NULL, 1, 1, 0.0f, 0, 0, POS_CENTER);
    }

    graficos_renderizar(j->renderer, j->eFramebuffersPause, FB_PAUSE_CANT);
}

void cerrarPausa(tJuego* j)
{
    int i;
    if(j->estadoPendiente != &estadoJuego)
    {

        for(i = 0; i < FB_GAME_CANT; i++)
        {
            graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersGame[i]);
            graficos_borrar_pantalla(j->renderer, &XBLANCO);
        }

        graficos_renderizar(j->renderer, j->eFramebuffersGame, FB_GAME_CANT);

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
    }

    for(i = 0; i < FB_PAUSE_CANT; i++)
    {
        graficos_cambiar_framebuffer(j->renderer, j->eFramebuffersPause[i]);
        graficos_borrar_pantalla(j->renderer, &XBLANCO);
    }

    graficos_renderizar(j->renderer, j->eFramebuffersPause, FB_PAUSE_CANT);


    for(i = 0; i < CANT_PAUSE_TXT; i++)
    {
        texto_destruir_textura(&j->textoPause[i]);
    }

    for(i = 0; i < CANT_PAUSE_BUTTON; i++)
    {
        button_destruir(&j->pauseButtons[i]);
    }

    printf("Cerrando pausa...\n");

}

void crear_botones_menu(tJuego* juego)
{
    int i;
    for(i = 0; i < CANT_MENU_BUTTON; i++)
    {
        button_crear(&juego->menuButtons[i],
                            juego->button[MENU_BUTTON],
                            posX(7.5),
                            posY(4.83 + i),
                            1,
                            1,
                            0,
                            0,
                            0
                        );
    }
}

tError crear_texto_menu(tJuego* juego)
{
    tError err;
    if((err = texto_crear_textura(juego->renderer,
                           &juego->textoMenu[TXT_MENU_JUGAR],
                           juego->fuenteTTF,
                           "JUGAR",
                            COLOR_GOLD_BRIGHT,
                            posX(7.5),
                            posY(5) - 15
    )))
    {
        return err;
    }

    if((err = texto_crear_textura(juego->renderer,
                           &juego->textoMenu[TXT_MENU_OPCIONES],
                           juego->fuenteTTF,
                           "OPCIONES",
                            COLOR_GOLD_BRIGHT,
                            posX(7.5),
                            posY(6) - 15

    )))
    {
        return err;
    }

    if((err = texto_crear_textura(juego->renderer,
                           &juego->textoMenu[TXT_MENU_SALIR],
                           juego->fuenteTTF,
                           "SALIR",
                            COLOR_GOLD_BRIGHT,
                            posX(7.5),
                            posY(7) - 15

    )))
    {
        return err;
    }

    return TODO_OK;
}


void crear_botones_pausa(tJuego* juego)
{
    int i;
    for(i = 0; i < CANT_PAUSE_BUTTON; i++)
    {
        button_crear(&juego->pauseButtons[i],
                            juego->button[MENU_BUTTON],
                            posX(7.5),
                            posY(2.83 + i),
                            1.2,
                            1.1,
                            0,
                            0,
                            0
                        );
    }
}

tError crear_texto_pausa(tJuego* juego)
{
    tError err;

    if((err = texto_crear_textura(juego->renderer,
                           &juego->textoPause[TXT_PAUSE_VOLVER],
                           juego->fuenteTTF,
                           "VOLVER  AL  JUEGO",
                            COLOR_GOLD_BRIGHT,
                            posX(7.5),
                            posY(3) - 15
    )))
    {
        return err;
    }
    if((err = texto_crear_textura(juego->renderer,
                           &juego->textoPause[TXT_PAUSE_OPCIONES],
                           juego->fuenteTTF,
                           "OPCIONES",
                            COLOR_GOLD_BRIGHT,
                            posX(7.5),
                            posY(4) - 15
    )))
    {
        return err;
    }
    if((err = texto_crear_textura(juego->renderer,
                           &juego->textoPause[TXT_PAUSE_MENU],
                           juego->fuenteTTF,
                           "MENU  PRINCIPAL",
                            COLOR_GOLD_BRIGHT,
                            posX(7.5),
                            posY(5) - 15
    )))
    {
        return err;
    }

    return TODO_OK;
}

Estado estadoMenuPrin = {
    abrirMenuPrin,
    eventosMenuPrin,
    actualizarMenuPrin,
    renderizarMenuPrin,
    cerrarMenuPrin,
};



Estado estadoPausa = {
    abrirPausa,
    eventosPausa,
    actualizarPausa,
    renderizarPausa,
    cerrarPausa,
};
