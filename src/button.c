/**
 * @file button.c
 * @author Dotta, Elian Agustin
 * @brief  Implementacion del modulo 'button'.
 * @version 1.0
 * @date 2026-03-05
 * 
 * 
 */

#include "button.h"
#include "graficos.h"

tError button_crear(Button *b, SDL_Texture *textura, int32_t posX, int32_t posY, float escalaHor, float escalaVer, double angulo, uint8_t flipHor, uint8_t flipVer)
{
    // Valido que las escalas no sean menor o igual a cero.
    // Valido que el angulo no se salga del intervalo 0 < x <= 360.
    // Valido que el flip sea mayor a 1.
    // Valido que la posicion en X e Y no se salga de los limites de la ventana.
    // Valido que la textura no este vacia.
    if(!b || escalaHor <= 0 || escalaVer <= 0 || angulo < 0 || angulo >= 360 || flipHor > 1 || flipVer > 1 || posX < 0 || posX >= window_w || posY < 0 || posY >= window_h)
        return ERR_PARAMETRO_INVALIDO;
    if(!textura)
        return ERR_TEXTURA;
    
    b->texture = textura;
    b->x = posX;
    b->y = posY;

    SDL_QueryTexture(textura, NULL, NULL, &b->rectFinal.w, &b->rectFinal.y);

    b->rectFinal.w = (int32_t)(b->rectFinal.w * escalaHor);
    b->rectFinal.h = (int32_t)(b->rectFinal.y * escalaVer);

    // Traslado el inicio del rectangulo para que el centro de la textura se alinie con las posiciones en X e Y ingresadas
    b->rectFinal.x = b->x - (b->rectFinal.w  / 2); // Resto la mitad del ancho de la textura en X
    b->rectFinal.y = b->y - (b->rectFinal.h / 2);  // Resto la mitad del alto de la textura en Y

    b->centro.x = (int32_t) (b->rectFinal.w /2);
    b->centro.y = (int32_t) (b->rectFinal.h / 2);

    b->angulo = angulo;

    b->flip = (flipHor ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) |
            (flipVer ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);

    // Inicializo las flags en false
    b->hovered = false;
    b->esApretado = false;
    b->clickeado = false;
    b->toggle = false;

    return TODO_OK;
}

void actualizarBoton(Button* b)
{
    b->clickeado = false;
}


void reiniciarBoton(Button *b)
{
    b->hovered = false;
    b->esApretado = false;
    b->clickeado = false;
    b->toggle = false;
}

void button_dibujar_textura(SDL_Renderer *renderer, Button *b)
{
    SDL_Color color;

    if(b->toggle)
    {
        if(b->hovered)
            color = BTOGGLED_HOVERED;
        else
            color = BTOGGLED;
    }
    else
    {
        if(b->esApretado)
            color = BAPRETADO;
        else if(b->hovered)
            color = BHOVERED;
        else
            color = BNORMAL;
    }

    SDL_APLICAR_COLOR(b->texture, color);

    SDL_RenderCopyEx(renderer, b->texture, NULL, &b->rectFinal, b->angulo, &b->centro, b->flip);

    SDL_APLICAR_COLOR(b->texture, BNORMAL);
}


void button_destruir(Button *b)
{
    b->x = 0;
    b->y = 0;
    if(b->texture)
    {
        b->texture = NULL;
    }

    b->angulo = 0;
    b->centro.x = 0;
    b->centro.y = 0;

    b->hovered = false;
    b->esApretado = false;
    b->clickeado = false;
    b->toggle = false;

}

void button_destruir_textura(Button *b)
{
    if(b->texture)
    {
        SDL_DestroyTexture(b->texture);
    }
    b->texture = NULL;
}
