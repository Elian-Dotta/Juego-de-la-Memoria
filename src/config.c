/**
 * @file config.c
 * @author Dotta, Elian Agustin
 *
 * @brief Implementacion del modulo config.
 *
 * @version 1.0
 * @date 2026-03-05
 *
 */
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Lee la grilla desde un archivo con formato especifico y la almacena en una matriz de enteros.
 * 
 * @param arch Archivo abierto en modo lectura de texto, posicionado en la linea siguiente a la linea "Grilla=".
 * @param grilla Matriz de enteros donde se almacenará la grilla leída.
 * @param cant_cartas Puntero al entero donde se almacenará la cantidad de cartas.
 * @return tError 
 */
static tError leer_grilla(FILE* arch, int grilla[MAX_FILAS][MAX_COLUMNAS], int *cant_cartas);


void config_default(tConfig *cfg)
{
    // Por defecto, la grilla es de 6x6 con todas las cartas activas
    cfg->filasGrilla = MAX_FILAS;
    cfg->columnasGrilla = MAX_COLUMNAS;
    int i, j;
    for(i = 0; i < MAX_FILAS; i++)
    {
        for(j = 0; j < MAX_COLUMNAS; j++)
        {
            cfg->grilla[i][j] = 1;
        }
    }
    cfg->cant_Cartas = cfg->filasGrilla * cfg->columnasGrilla;

    // El set de cartas por defecto es el de billetes
    cfg->numeroSet = SET_BILLETES;

    // Por defecto, el juego es para 1 jugador
    cfg->cant_Jugadores = 1;

    strcpy(cfg->n1, "P1");
    strcpy(cfg->n2, "P2");
}

tError config_cargar(tConfig *cfg, const char *ruta)
{
    // Abro el archivo de configuracion en modo lectura de texto
    FILE* config = fopen(ruta, "rt");
    if(!config)
    {
        return ERR_ARCHIVO;
    }

    // Inicializo la configuracion en cero para luego ir actualizandola con los valores del archivo
    cfg->filasGrilla = 0;
    cfg->columnasGrilla = 0;
    cfg->cant_Cartas = 0;
    for(int i = 0; i < MAX_FILAS; i++)
    {
        for(int j = 0; j < MAX_COLUMNAS; j++)
        {
            cfg->grilla[i][j] = 0;
        }

    }
    cfg->n1[0] = '\0';
    cfg->n2[0] = '\0';

    // Variables para almacenar temporalmente los valores leidos del archivo
    int cantJugadores = 0;
    int cantCartas = 0;
    int grilla[MAX_FILAS][MAX_COLUMNAS];
    int setCartas = 0;
    for (int i = 0; i < MAX_FILAS; i++)
    {
        for(int j = 0; j < MAX_COLUMNAS; j++)
        {
            grilla[i][j] = 0;
        }
    }
    char n1[LARGO_NOMBRE] = {'\0'};
    char n2[LARGO_NOMBRE] = {'\0'};


    // Buffer para almacenar cada linea
    char linea[TAM_BUFFER];

    while(fgets(linea, sizeof(linea), config))
    {
        // \0 al salto de linea
        linea[strcspn(linea, "\n")] = '\0';

        // ignoro lineas vacias o comentarios o la grilla
        if(linea[0] == '\0' || linea[0] == '#' || linea[0] == '-')
            continue;

        // obtengo la posicion del igual
        size_t pos = strcspn(linea, "=");

        // intercambio el igual por un \0 para delimitar la clave y el valor
        if(linea[pos] ==  '\0')
            continue;


        char *clave = linea;
        char *valor = linea + pos + 1;


        // corrigo espacios despues de la clave y antes del valor
        while(pos > 0 && linea[pos - 1] == ' ')
        {
            linea[pos - 1] = '\0';
            pos--;
        }
        while(*valor == ' ')
        {
            valor++;
        }

        size_t len = strlen(valor);
        if(len > 0)
        {
            char *ptr = valor + len - 1;
            while(ptr >= valor && *ptr == ' ')
            {
                *ptr = '\0';
                ptr--;
            }
        }

        if(!strcmp(clave, "Grilla"))
        {
            leer_grilla(config, grilla, &cantCartas);
        }
        else if(!strcmp(clave, "Set de cartas"))
        {
            setCartas = atoi(valor);
        }
        else if(!strcmp(clave, "Cantidad de jugadores"))
        {
            cantJugadores = atoi(valor);
        }
        else if(!strcmp(clave, "Jugador 1"))
        {
            strcpy(n1, valor);
        }
        else if(!strcmp(clave, "Jugador 2"))
        {
            strcpy(n2, valor);
        }

    }

	fclose(config);

    // Valido los valores leidos y actualizo la configuracion con estos valores si son validos, o devuelvo error si no lo son
    if(config_actualizar(cfg, grilla, setCartas, cantJugadores, n1, n2))
    {
        return ERR_CONFIG_INVALIDA;
    }


    return TODO_OK;
}

static tError leer_grilla(FILE* arch, int grilla[MAX_FILAS][MAX_COLUMNAS], int *cant_cartas)
{
    char linea[TAM_BUFFER];
    int i = 0;

    while(i < MAX_FILAS)
    {
        if(!fgets(linea, sizeof(linea), arch))
        {
            return ERR_ARCHIVO;
        }

        linea[strcspn(linea, "\n")] = '\0';
        char* cur = linea;
        int j = 0;
        while(*cur != '\0' && j < MAX_COLUMNAS)
        {
            if(*cur == '#')
            {
                grilla[i][j] = 1;
                (*cant_cartas)++;
            }
            else if(*cur == '-')
                grilla[i][j] = 0;
            cur++;
            j++;
        }
        i++;
    }

    return TODO_OK;
}

tError config_actualizar(tConfig *cfg, int grilla[MAX_FILAS][MAX_COLUMNAS] ,int set_carta, int cantJugadores, const char* n1, const char* n2)
{

    int primerFila = -1, primerColumna = -1;
    int ultimaFila = -1, ultimaColumna = -1;
    int cant_cartas = 0;
    int c, f;

    for(f = 0; f < MAX_FILAS; f++)
    {
        for(c = 0; c < MAX_COLUMNAS; c++)
        {
            cfg->grilla[f][c] = grilla[f][c];
            if(cfg->grilla[f][c] == 1)
            {
                cant_cartas++;
                if(primerFila == -1) primerFila = f;
                if(primerColumna == -1 || c < primerColumna) primerColumna = c;
                if(c > ultimaColumna) ultimaColumna = c;
                if(f > ultimaFila) ultimaFila = f;
            }
        }
    }



    cfg->filasGrilla = ultimaFila - primerFila + 1;
    cfg->columnasGrilla = ultimaColumna - primerColumna + 1;

    if(cfg->grilla[0][0] == 0)
    {
        for(f = 0; f < MAX_FILAS; f++)
        {
            for(c = 0; c < MAX_COLUMNAS; c++)
            {
                if(cfg->grilla[f][c] == 1)
                {
                    cfg->grilla[f - primerFila][c - primerColumna] = 1;
                    cfg->grilla[f][c] = 0;
                }
            }
        }
    }

    cfg->cant_Cartas = cant_cartas;
    cfg->numeroSet = set_carta;

    cfg->cant_Jugadores = cantJugadores;

    strcpy(cfg->n1, n1);
    if(cfg->cant_Jugadores > 1)
    {
        strcpy(cfg->n2, n2);
    }
    else
    {
        strcpy(cfg->n2, "\0");
    }

    printf("Filas: %d\n", cfg->filasGrilla);
    printf("Columnas: %d\n", cfg->columnasGrilla);
    printf("Cantidad de cartas: %d\n", cfg->cant_Cartas);
    printf("Set de cartas: %d\n", cfg->numeroSet);
    printf("Cantidad de jugadores: %d\n", cfg->cant_Jugadores);
    printf("Jugador 1: %s\n", cfg->n1);
    printf("Jugador 2: %s\n", cfg->n2);
    printf("Grilla:\n");
    for(f = 0; f < MAX_FILAS; f++)
    {
        for(c = 0; c < MAX_COLUMNAS; c++)
        {
            printf("%d ", cfg->grilla[f][c]);
        }
        printf("\n");
    }

    // Valido los valores leidos y actualizo la configuracion con estos valores si son validos, o devuelvo error si no lo son.
    if(cfg->filasGrilla <= 0 || cfg->filasGrilla > MAX_FILAS ||
       cfg->cant_Cartas <= 0 || cfg->cant_Cartas > MAX_FILAS * MAX_COLUMNAS ||
       cfg->cant_Cartas % 2 != 0 ||
       cfg->columnasGrilla <= 0 || cfg->columnasGrilla > MAX_COLUMNAS ||
       cfg->numeroSet < SET_BILLETES || cfg->numeroSet >= CANT_SET ||
       cfg->cant_Jugadores < 1 || cfg->cant_Jugadores > CANT_MAX_JUGADORES ||
       strlen(cfg->n1) == 0 || strlen(cfg->n1) > LARGO_NOMBRE - 1 || strlen(cfg->n2) > LARGO_NOMBRE - 1
       )
    {
        printf("%s\n", errores_obtener_detalle(ERR_CONFIG_INVALIDA));
        return ERR_CONFIG_INVALIDA;
    }

    return TODO_OK;
}

tError config_guardar(tConfig *cfg, const char *ruta)
{
    FILE* config = fopen(ruta, "wt");
    if(!config)
    {
        return ERR_ARCHIVO;
    }

    char linea[TAM_BUFFER];

    // fprintf(config, "Filas = %d\n", cfg->filasGrilla);
    // fprintf(config, "Columnas = %d\n", cfg->columnasGrilla);
    fprintf(config, "Grilla = \n");
    for(int i = 0; i < MAX_FILAS; i++)
    {
        for(int j = 0; j < MAX_COLUMNAS; j++)
        {
            linea[j] = (cfg->grilla[i][j] == 1) ? '#' : '-';

        }

        linea[MAX_COLUMNAS] = '\0';

        fprintf(config, "%s\n", linea);
    }
    fprintf(config, "Set de cartas = %d\n", cfg->numeroSet);
    fprintf(config, "Cantidad de jugadores = %d\n", cfg->cant_Jugadores);
    fprintf(config, "Jugador 1 = %s\n", cfg->n1);
    fprintf(config, "Jugador 2 = %s\n", cfg->n2);

    fclose(config);
    return TODO_OK;
}
