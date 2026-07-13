#include "graficos.h"
#include "stdio.h"
#include <stdlib.h>

#include "config.h"


void graficos_dibujar_textura(SDL_Renderer *renderer, SDL_Texture *textura, SDL_Rect *origen, int32_t posX, int32_t posY, float escalaHor, float escalaVer, double angulo, uint8_t flipHor, uint8_t flipVer, ePos pos)
{
    SDL_Rect destino = {
        posX,
        posY,
        0,
        0
    };

    SDL_QueryTexture(textura, NULL, NULL, &destino.w, &destino.h);
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


    SDL_RenderCopyEx(renderer, textura, origen, &destino, angulo, &centro, flip);
}

void graficos_borrar_pantalla(SDL_Renderer *renderer, const SDL_Color *color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    SDL_RenderClear(renderer);
}

SDL_Texture* graficos_crear_framebuffer(SDL_Renderer *renderer, uint32_t anchoVentana, uint32_t altoVentana)
{
    SDL_Texture *framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, anchoVentana, altoVentana);
    if (!framebuffer) {
        fprintf(stderr, "Error: No se pudo crear el framebuffer: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetTextureBlendMode(framebuffer, SDL_BLENDMODE_BLEND);

    return framebuffer;
}

void graficos_cambiar_framebuffer(SDL_Renderer *renderer, SDL_Texture *target)
{
    SDL_SetRenderTarget(renderer, target);
}


void graficos_dibujar_grilla(SDL_Renderer *renderer)
{
    for(int i = 0; i < TAM_GRID_Y; i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, posY(i) - TILE / 2, window_w,  posY(i) - TILE / 2);
    }
    for(int i = 0; i < TAM_GRID_X; i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 225, 255);
        SDL_RenderDrawLine(renderer, posX(i) - TILE / 2, 0 , posX(i) - TILE / 2,  window_h);
    }

    for(int i = 0; i < TAM_GRID_Y; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 0, posY(i), window_w,  posY(i));
    }
    for(int i = 0; i < TAM_GRID_X; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, posX(i), 0 , posX(i),  window_h);
    }
}



void graficos_renderizar(SDL_Renderer *renderer, SDL_Texture* const *framebuffers, uint32_t cantFramebuffers)
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (uint32_t i = 0; i < cantFramebuffers; ++i) {
        if (framebuffers[i]) {
            SDL_RenderCopy(renderer, framebuffers[i], NULL, NULL);
        }
    }

    SDL_RenderPresent(renderer);
}
