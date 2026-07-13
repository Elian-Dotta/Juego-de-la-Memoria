#ifndef TDAVECTOR_H
#define TDAVECTOR_H

#include <stdbool.h>
#include <stddef.h>


#include "errores.h"

#define TAM 10
#define CAP_INI 8

#define NO_HALLADO -1
//#define TODO_OK 0
//#define LLENO 1
#define DUPLICADO 1


#define FACTOR_INCR 1.5
#define FACTOR_DECR 0.5
#define FACTOR_OCUP 0.25

#define AUMENTAR 1
#define DISMINUIR 2

#define BURBUJEO 1
#define SELECCION 2
#define SELECCION_DOBLE 3
#define INSERCION 4
#define QSORT 5

typedef struct
{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}Vector;

typedef struct
{
    void* act;
    void* ult;
    Vector* vec;
    bool finIter;
}VectorIterador;

typedef int (*Cmp) (const void* a, const void* b);
typedef void (*Imprimir) (const void* a);
typedef void (*Accion) (void* a, void* b);

tError vectorCrear (Vector* v, size_t tamElem);
tError vectorCrearDeArchivo (Vector* v, size_t tamElem, const char* nomArch);
tError vectorGrabar(Vector* v, const char* nomArch);
tError vectorInsertarAlInicio (Vector* v, const void* elem, Cmp cmp);
tError vectorInsertarAlFinal (Vector* v, const void* elem);
tError vectorInsertarEnPos (Vector* v, const void* elem, int pos, Cmp cmp);
tError vectorOrdInsertar (Vector* v, void* elem, Cmp cmp);
tError vectorOrdBuscar (const Vector* v, void* elem, Cmp cmp);
tError vectorDesordBuscar (const Vector* v, const void* elem, Cmp cmp);
tError vectorObtenerDePos(const Vector* v, void* elem, int pos);
tError vectorEliminarDePos (Vector* v, int pos);
tError vectorOrdEliminar (Vector* v, void* elem, Cmp cmp);
void vectorOrdenar (Vector* v, int metodo, Cmp cmp);
void vectorMostrar (const Vector* v, Imprimir imprimir);
void vectorMezclar (Vector* v);
void vectorRecorrer (Vector*  v, Accion accion, void* contexto);
void vectorVaciar (Vector* v);
void vectorDestruir (Vector* v);

/*Vector iterador*/

void vectorIteradorCrear (VectorIterador* it, const Vector* v);
void* vectorIteradorPrimero (VectorIterador* it);
void* vectorIteradorSiguiente (VectorIterador* it);
void* vectorIteradorAnterior (VectorIterador* it);
bool vectorIteradorFin (VectorIterador* it);

#endif // TDAVECTOR_H
