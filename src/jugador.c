#include "jugador.h"
#include <string.h>

void jugadorInicializar(Jugador* p, int id, const char* nombre)
{
    strcpy(p->nombre, nombre);

    p->id_jugador = id;
    p->puntaje = 0;
    p->cont_racha = 0;
}

void cambiarTurno(uint32_t *indice, int cant, int cantJugadores)
{
    if(cantJugadores == 1)
    {
        return;
    }
    
    uint32_t nuevoIndice = *indice + cant;

    if(nuevoIndice < 0)
    {
        nuevoIndice = cantJugadores - 1;
    }
    if(nuevoIndice >= cantJugadores)
    {
        nuevoIndice = 0;
    }

    *indice = nuevoIndice;
}

void actualizarRachaPuntaje(void* a, void* b)
{
    Jugador *p = a;
    contextoPuntaje* cont = b;
    if (p->id_jugador == cont->indiceTurno)
    {
        if(cont->puntaje == 0)
        {
            p->cont_racha = 0;
        }
        else
        {   
            p->cont_racha++;
            p->puntaje += cont->puntaje * p->cont_racha;
        }
    }

}