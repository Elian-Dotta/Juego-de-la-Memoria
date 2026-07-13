#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "estado.h"
#include "errores.h"

typedef enum {
    FB_MENU_BACKGROUND,
    FB_MENU_BUTTONS,
    FB_MENU_TXT,
    //FB_MENU_GRILLA,
    FB_MENU_CANT,
}eFramebuffersMenu;

typedef enum {
    MENU_BUTTON_JUGAR,
    MENU_BUTTON_OPCIONES,
    MENU_BUTTON_SALIR,
    CANT_MENU_BUTTON,
}eMenuButton;

typedef enum {
    TXT_MENU_JUGAR,
    TXT_MENU_OPCIONES,
    TXT_MENU_SALIR,
    CANT_MENU_TXT,
}eMenuTxt;

typedef enum {
    FB_PAUSE_BACKGROUND,
    FB_PAUSE_BUTTONS,
    FB_PAUSE_TXT,
    FB_PAUSE_CANT,
}eFramebuffersPause;

typedef enum {
    PAUSE_BUTTON_VOLVER,
    PAUSE_BUTTON_OPCIONES,
    PAUSE_BUTTON_MENU,
    CANT_PAUSE_BUTTON,
}ePauseButton;

typedef enum {
    TXT_PAUSE_VOLVER,
    TXT_PAUSE_MENU,
    TXT_PAUSE_OPCIONES,
    CANT_PAUSE_TXT,
}ePauseTxt;


extern Estado estadoMenuPrin;

extern Estado estadoPausa;


void abrirMenuPrin(tJuego* juego);
void eventosMenuPrin(tJuego* juego);
void actualizarMenuPrin(tJuego* juego);
void renderizarMenuPrin(tJuego* juego);
void cerrarMenuPrin(tJuego* juego);



void abrirPausa(tJuego* juego);
void eventosPausa(tJuego* juego);
void actualizarPausa(tJuego* juego);
void renderizarPausa(tJuego* juego);
void cerrarPausa(tJuego* juego);

void crear_botones_menu(tJuego* juego);
tError crear_texto_menu(tJuego* juego);

void crear_botones_pausa(tJuego* juego);
tError crear_texto_pausa(tJuego* juego);

#endif // MENU_H_INCLUDED
