#include "inputTxt.h"
#include "button.h"
#include "graficos.h"
#include "string.h"
#include "juego.h"


void inicializarEntrada(entradaTxt* input, SDL_Texture* textura, TTF_Font* fuente, char* s, SDL_Color color, int32_t posX, int32_t posY, float escalaHorTxt, float escalaVerTxt, float escalaHorBut, float escalaVerBut, int pos)
{
    if(strlen(s) > TAM_BUFFER - 1)
    {
        input->buffer[0] = '\0';
    }
    else
    {
        strcpy(input->buffer, s);
    }

    input->activo = false;

    input->posX = posX;
    input->posY = posY;
    input->escalaHor = escalaHorTxt;
    input->escalaVer = escalaVerTxt;

    input->fuente = fuente;
    input->color = color;

    button_crear(&input->botonEntrada,
                        textura,
                        posX,
                        posY,
                        escalaHorBut,
                        escalaVerBut,
                        0,
                        0,
                        0
                        );
}


void activarInput(tJuego *j, entradaTxt* input)
{
    if(j->inputActual && j->inputActual != input)
    {
        pararEntrada(j->inputActual);
    }

    j->inputActual = input;
    iniciarEntrada(input);
}

void desactivarInput(tJuego *j)
{
    if(j->inputActual)
    {
        pararEntrada(j->inputActual);
        j->inputActual = NULL;
    }
}

void iniciarEntrada(entradaTxt* input)
{
    if(!input->activo)
    {
        input->activo = true;
        SDL_StartTextInput();
    }
}

void pararEntrada(entradaTxt* input)
{
    if(input->activo)
    {
        input->activo = false;
        SDL_StopTextInput();
    }
}

void dibujarBotonEntrada(SDL_Renderer* renderer, entradaTxt* input)
{
    button_dibujar_textura(renderer, &input->botonEntrada);
}

void dibujarEntradaEnPantalla(SDL_Renderer* renderer, entradaTxt* input)
{

    char bufferDibujo[TAM_BUFFER + 2];
    strcpy(bufferDibujo, input->buffer);
    size_t len = strlen(bufferDibujo);

    SDL_Color colorActivo = input->color;

    if(input->activo)
    {
        bufferDibujo[len] = '|';
        bufferDibujo[len + 1] = '\0';
        colorActivo = BLANCO;
    }

    SDL_Surface *superficie = TTF_RenderUTF8_Blended(input->fuente, bufferDibujo, colorActivo);

    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, superficie);

    SDL_FreeSurface(superficie);

    SDL_SetTextureBlendMode(textura, SDL_BLENDMODE_BLEND);

    SDL_Rect destino = {
            input->posX,
            input->posY,
            0,
            0,
    };

    SDL_QueryTexture(textura, 0, 0, &destino.w, &destino.h);

    destino.w = (int32_t)(destino.w * input->escalaHor);
    destino.h = (int32_t)(destino.h * input->escalaVer);

    destino.x = destino.x - destino.w / 2;
    destino.y = destino.y - destino.h / 2;

    SDL_RenderCopy(renderer, textura, NULL, &destino);

    SDL_DestroyTexture(textura);
}

const char* devolverEntrada(entradaTxt* input)
{
    return input->buffer;
}
