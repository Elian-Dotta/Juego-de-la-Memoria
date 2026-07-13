#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED


#include "estado.h"
#include "errores.h"


typedef enum {
    FB_GAME_BACKGROUND,
    FB_GAME_CARTA,
    FB_GAME_BUTTONS,
    FB_GAME_INDICADOR,
    FB_GAME_HUD,
    FB_GAME_PLAYERNAME,
    FB_GAME_TXT,
    FB_GAME_GANADOR,
    //FB_GAME_GRILLA,
    FB_GAME_CANT,
}eFramebuffersGame;

typedef enum{
    GAME_BUTTON_PAUSE,
    GAME_BUTTON_UNMIX,
    GAME_BUTTON_MIX,
    GAME_BUTTON_ROL,
    GAME_BUTTON_ROR,
    GAME_BUTTON_MENUPRIN,
    CANT_GAME_BUTTON,
}eGameButton;

typedef enum {
    TXT_GAME_ORDENAR,
    TXT_GAME_MEZCLAR,
    TXT_GAME_GANASTE,
    TXT_GAME_MENUPRIN,
    CANT_GAME_TXT,
}eGameTxt;

typedef enum {
    TXT_GAME_P1NAME,
    TXT_GAME_P2NAME,
    CANT_GAME_NAME_TXT,
}eGameNameTxt;

typedef enum {
    HUD_GAME_P1_POINTS,
    HUD_GAME_P2_POINTS,
    CANT_GAME_HUD,
}eGameHud;

typedef enum {
    GAME_IND_P1,
    GAME_IND_P2,
    CANT_GAME_IND,
}eGameIndicadores;

#define WAIT_FIRST 0
#define WAIT_SECOND 1
#define CHECK 2
#define FINISHED 3

extern Estado estadoJuego;

void abrirJuego(tJuego* juego);
void eventosJuego(tJuego* juego);
void actualizarJuego(tJuego* juego);
void renderizarJuego(tJuego* juego);
void cerrarJuego(tJuego* juego);

tError crear_texto_game(tJuego* juego);
void crear_botones_game(tJuego* juego);

#endif // GAMEPLAY_H_INCLUDED
