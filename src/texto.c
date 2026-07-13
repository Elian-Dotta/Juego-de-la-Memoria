#include "texto.h"
#include <stdio.h>


tError texto_inicializar(void)
{
    if (TTF_Init() != 0) {
        fprintf(stderr, "No se pudo inicializar SDL_ttf: %s\n", TTF_GetError());
        return ERR_SDL_TTF;
    }

    return TODO_OK;
}

TTF_Font* texto_cargar_fuente(const char *path, uint32_t tam)
{
    TTF_Font *fuente = TTF_OpenFont(path, tam);
    if (!fuente) {
        fprintf(stderr, "No se pudo cargar la fuente \"%s:\" %s\n", path, TTF_GetError());
    }

    return fuente;
}

void texto_destruir_fuente(TTF_Font *fuente)
{
    TTF_CloseFont(fuente);
}

tError texto_crear_textura(SDL_Renderer *renderer, tTexto *texto, TTF_Font *fuente, const char* cadena, SDL_Color color, int32_t posX, int32_t posY)
{
    SDL_Surface * superficie = TTF_RenderUTF8_Blended(fuente, cadena, color); // Permite utilizar caracteres UTF8
    if (!superficie) {
        fprintf(stderr, "%s\n", TTF_GetError());
        return ERR_SDL_IMAGE;
    }

    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, superficie);

    SDL_FreeSurface(superficie); // Una vez creada la textura, la superficie ya no es necesaria

    if (!textura) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return ERR_TEXTURA;
    }

    SDL_SetTextureBlendMode(textura, SDL_BLENDMODE_BLEND);

    texto->texto = textura;
    texto->x = posX;
    texto->y = posY;

    return TODO_OK;
}



void texto_dibujar_textura(SDL_Renderer *renderer, tTexto *texto, SDL_Rect *origen, float escalaHor, float escalaVer, double angulo, uint8_t flipHor, uint8_t flipVer, ePos pos)
{
    SDL_Rect destino = {
        texto->x,
        texto->y,
        0,
        0
    };

    SDL_QueryTexture(texto->texto, NULL, NULL, &destino.w, &destino.h);
    destino.w = (int32_t)(destino.w * escalaHor);
    destino.h = (int32_t)(destino.h * escalaVer);

    SDL_Point centro = {
        (int32_t)(destino.w / 2),
        (int32_t)(destino.h / 2),
    };

    switch(pos)
    {
        case POS_LUCORNER:
                break;
        case POS_CENTER:
                //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                //SDL_RenderDrawLine(renderer, 0, destino.y, window_w, destino.y);
                //SDL_RenderDrawLine(renderer, destino.x, 0, destino.x, window_h);
                destino.x = destino.x - destino.w / 2;
                destino.y = destino.y - destino.h / 2;
                break;
        case POS_LDCORNER:
                destino.y = destino.y - destino.h;
                break;
        case POS_RUCORNER:
                destino.x = destino.x - destino.w;
                break;
        case POS_RDCORNER:
                destino.y = destino.y - destino.h;
                destino.x = destino.x - destino.w;
                break;

    }

    SDL_RendererFlip flip = (flipHor ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) |
                            (flipVer ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);


    SDL_RenderCopyEx(renderer, texto->texto, origen, &destino, angulo, &centro, flip);
}


tError texto_destruir_textura(tTexto *txt)
{
    if(txt->texto)
    {
        SDL_DestroyTexture(txt->texto);
    }

    txt->x = 0;
    txt->y = 0;
    return TODO_OK;
}

void texto_finalizar(void)
{
    TTF_Quit();
}
