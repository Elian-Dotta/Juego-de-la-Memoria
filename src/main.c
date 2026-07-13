/**
 * @file main.c
 * @author Dotta, Elian Agustin
 * 
 * 
 * @version 1.0
 * @date 2026-03-05
 */

#include <stdio.h>
#include <stdlib.h>
#include "juego.h"
#include "errores.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));
    tError err;
    tJuego juego;

    if ((err = juego_inicializar(&juego)) != TODO_OK) {
        fprintf(stderr, "Error: %s\n", errores_obtener_detalle(err));
        return err;
    }

    // Loop principal
    while (juego.corriendo)
    {
        // Procesamiento de eventos (teclado, mouse, eventos de ventana)
        if ((err = juego_procesar_eventos(&juego)) != TODO_OK) {
            fprintf(stderr, "Error: %s\n", errores_obtener_detalle(err));
        }

        // Actualizacion del estado del juego y sus elementos
        juego_actualizar(&juego);

        // Renderizado de la escena
        juego_renderizar(&juego);

        // Esta pausa evita que el procesador sea utilizado al 100%. 16ms limita el renderizado a 60 fps (1000 / 16 = ~62,5fps)
        SDL_Delay(LOOP_DELAY);

        // Aplica un cambio de estado si hay uno pendiente
        if(juego.estadoPendiente)
        {
            juego_aplicar_cambio_estado(&juego);
        }
    }

    // Limpieza y liberacion de la memoria
    juego_destruir(&juego);

    printf("CERRANDO JUEGO\n");

    return err;
}
