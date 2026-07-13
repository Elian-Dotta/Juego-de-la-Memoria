#include "carta.h"
#include "graficos.h"
#include "juego.h"
#include "gameplay.h"

void crear_carta(Carta *c, int id, int tipo, SDL_Texture *Fr, SDL_Texture *Atr)
{
    c->id_carta = id;
    c->tipo = tipo;
    c->frente = Fr;
    c->reverso = Atr;

    c->estado = 0;
    c->hovered = 0;
    c->esApretado = 0;
}

void carta_dibujar_textura(SDL_Renderer* renderer, Carta *carta)
{
    SDL_SetTextureBlendMode(carta->frente, SDL_BLENDMODE_BLEND);
    SDL_SetTextureColorMod(carta->frente, 255,255,255);
    SDL_SetTextureAlphaMod(carta->frente, 255);


    SDL_SetTextureBlendMode(carta->reverso, SDL_BLENDMODE_BLEND);
    SDL_SetTextureColorMod(carta->reverso, 255,255,255);
    SDL_SetTextureAlphaMod(carta->reverso, 255);

    if(carta->estado == OCULTA)
    {
        if(carta->hovered)
        {
            SDL_SetTextureColorMod(carta->reverso, 255, 255, 210);
        }
        else if(carta->esApretado)
        {
            SDL_SetTextureColorMod(carta->reverso, 180, 180, 180);
        }
    }else if(carta->estado == COINCIDENCIA)
    {

        SDL_SetTextureColorMod(carta->frente, 255,240,150);

    }


    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderDrawLine(renderer, 0, carta->rectFinal.y, window_w, carta->rectFinal.y);
    // SDL_RenderDrawLine(renderer, carta->rectFinal.x, 0, carta->rectFinal.x, window_h);
    // SDL_RenderDrawLine(renderer, 0, carta->rectFinal.y + carta->rectFinal.h, window_w, carta->rectFinal.y + carta->rectFinal.h);
    // SDL_RenderDrawLine(renderer, carta->rectFinal.x + carta->rectFinal.w, 0, carta->rectFinal.x + carta->rectFinal.w, window_h);
    // SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    // SDL_RenderDrawRect(renderer, &carta->rectFinal);



    SDL_RenderCopy(renderer,
                    carta->estado == OCULTA? carta->reverso : carta->frente,
                    NULL,
                    &carta->rectFinal);


}

void imprimirCarta(void* a, void* b)
{
    Carta* carta = a;
    SDL_Renderer* renderer = b;

    carta_dibujar_textura(renderer, carta);
}

void actualizar_posicion_carta(void* a, void* b)
{
    Carta *carta = a;
    contextoActPosicion *contexto = b;

    int i = contexto->indice;
    contexto->indice++;

    int idx = -1;
    // int r = i / contexto->cantColumnas;
    // int c = i % contexto->cantColumnas;

    int c = 0, r = 0;

    if(contexto->grilla[r][c] == 1)
        idx = 0;
    while(idx != i)
    {
        c++;
        if(c >= contexto->cantColumnas)
        {
            c = 0;
            r++;
        }
        if(contexto->grilla[r][c] == 1)
            idx++;

    }
    carta->rectFinal.w = contexto->cartaW;
    carta->rectFinal.h = contexto->cartaH;

    carta->rectFinal.x = contexto->IniX + c * (carta->rectFinal.w + contexto->margenX);
    carta->rectFinal.y = contexto->IniY + r * (carta->rectFinal.h + contexto->margenY);
}

void calcular_layout_cartas(
    SDL_Texture* texturaEjemplo,
    int filas,
    int columnas,
    float limSup,
    float limInf,
    float limIzq,
    float limDer,
    float margenFactor,
    Layout *lay
)
{
    // Tamaño real de carta
    int texW, texH;
    SDL_QueryTexture(texturaEjemplo, NULL, NULL, &texW, &texH);

    float ratio = (float)texW / (float)texH;

    lay->cartaH = texH;
    lay->cartaW = texW;

    // ---- ZONA DISPONIBLE ----
    float zonaW = limDer - limIzq;
    float zonaH = limInf - limSup;

    // ---- Calcular tamaño inicial ----

    // Maximo por altura
    float cartaH_alto = zonaH / (filas + (filas - 1) * margenFactor);

    // Maximo por anchura
    float cartaH_ancho = (zonaW / (columnas + (columnas - 1) * margenFactor)) / ratio;

    // Elegir mas restrictivo
    lay->cartaH = (cartaH_alto < cartaH_ancho ? cartaH_alto : cartaH_ancho);

    lay->cartaW = lay->cartaH * ratio;

    float margenY = lay->cartaH * margenFactor;
    float margenX = lay->cartaW * margenFactor;

    float altoTotal  = filas * lay->cartaH + (filas - 1) * margenY;

    float anchoTotal = columnas * lay->cartaW + (columnas - 1) * margenX;

    float startX = limIzq + (zonaW - anchoTotal) / 2.0f;
    float startY = limSup + (zonaH - altoTotal) / 2.0f;

    lay->startX = startX;
    lay->startY = startY;
    lay->margenX = margenX;
    lay->margenY = margenY;

}


void mouseSobreCarta(void* a, void* b)
{
    Carta* carta = a;
    SDL_Point* mouse = b;

    carta->hovered = SDL_PointInRect(mouse, &carta->rectFinal);
}

void leftClickEnCarta(void* a, void* b)
{
    Carta* carta = a;
    tJuego* j = b;

    if(j->estadoMemoria != CHECK && carta->estado == OCULTA && carta->hovered && j->event.button.button == SDL_BUTTON_LEFT)
    {
        carta->esApretado = 1;
        carta->estado = REVELADA;
        sonidos_reproducir(j->sndJuego[SND_CAMBIAR_CARTA], 1);

        if(j->estadoMemoria == WAIT_FIRST)
        {
            j->firstCard = carta->tipo;
            j->estadoMemoria = WAIT_SECOND;
        }
        else if(j->estadoMemoria == WAIT_SECOND)
        {
            j->secondCard = carta->tipo;
            j->estadoMemoria = CHECK;
            j->revealTime = SDL_GetTicks();
        }
    }
}

void noClickEnCarta(void* a, void* b)
{
    Carta* carta = a;

    carta->esApretado = 0;
}

void actualizarCarta(void* a, void* b)
{
    Carta* carta = a;
    tJuego* j = b;

    if(carta->tipo == j->firstCard && carta->tipo == j->secondCard)
    {
        carta->estado = COINCIDENCIA;
        j->contCoincidencias++;
    }
    else if(carta->estado != 2)
    {
        carta->estado = OCULTA;
    }


}

int cmpCarta(const void* a, const void* b)
{
    const Carta* c1 = a;
    const Carta* c2 = b;

    // if(c1->tipo == c2->tipo)
    //     return 0;
    // else

    return c1->tipo - c2->tipo;
}

// void cartaDesmezclar(VectorIterador* dest, VectorIterador* cur)
// {
//     Carta *cDest;
//     Carta *cCur;
//     Carta aux = NULL;

//     cDest = vectorIteradorPrimero(dest);
//     aux = malloc(sizeof(*cDest));
//     cCur = vectorIteradorPrimero(cur);
//     cCur = vectorIteradorSiguiente(cur);

//     while(!vectorIteradorFin(dest))
//     {
//         while((*cDest).frente != (*cCur).frente)
//         {
//             cCur = vectorIteradorSiguiente;
//         }


//     }

//     free(aux);
// }

void carta_rol(Vector* cartas)
{
    Carta *cDest;
    Carta *cCur;
    Carta aux;
    VectorIterador dest;
    VectorIterador cur;

    vectorIteradorCrear(&dest, cartas);
    vectorIteradorCrear(&cur, cartas);

    cDest = vectorIteradorPrimero(&dest);
    cCur = vectorIteradorPrimero(&cur);
    cCur = vectorIteradorSiguiente(&cur);

    memcpy(&aux, cDest, sizeof(Carta));

    while(!vectorIteradorFin(&cur))
    {
        memcpy(cDest, cCur, sizeof(Carta));
        cDest = vectorIteradorSiguiente(&dest);
        cCur = vectorIteradorSiguiente(&cur);
    }

    memcpy(cCur, &aux, sizeof(Carta));
}

void carta_ror(Vector* cartas)
{
    Carta *cDest;
    Carta *cCur;
    Carta aux;

    VectorIterador dest;
    VectorIterador cur;

    vectorIteradorCrear(&dest, cartas);
    vectorIteradorCrear(&cur, cartas);

    vectorIteradorPrimero(&dest);

    while(!vectorIteradorFin(&dest))
    {
        cDest = vectorIteradorSiguiente(&dest);
    }

    memcpy(&aux, cDest, sizeof(Carta));

    cCur = vectorIteradorPrimero(&cur);

    while(!vectorIteradorFin(&cur))
    {
        cCur = vectorIteradorSiguiente(&cur);
    }

    cCur = vectorIteradorAnterior(&cur);

    while(!vectorIteradorFin(&cur))
    {
        memcpy(cDest, cCur, sizeof(Carta));
        cCur = vectorIteradorAnterior(&cur);
        cDest = vectorIteradorAnterior(&dest);
    }

    memcpy(cDest, &aux, sizeof(Carta));
}

