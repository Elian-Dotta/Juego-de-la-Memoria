#include "TDAvectorGen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int redimensionarVector(Vector* v, int operacion);
size_t maax(size_t a, size_t b);
void ordenarBurbujeo(Vector* v, Cmp cmp);
void ordenarSeleccion(Vector* v, Cmp cmp);
void ordenarSeleccionDoble (Vector* v, Cmp cmp);
void ordenarInsercion(Vector* v, Cmp cmp);
void intercambiar(void* a, void* b, size_t tamElem);
void* buscarMenor (void* inicio, void* fin, Cmp cmp, size_t tamElem);

tError vectorCrear (Vector* v, size_t tamElem)
{
    v->ce = 0;
    v->vec = malloc(CAP_INI * tamElem);

    if(v->vec == NULL)
    {
        v->cap = 0;
        v->tamElem = 0;
        return ERR_MEMORIA;
    }

    v->cap = CAP_INI;
    v->tamElem = tamElem;
    return TODO_OK;
}

tError vectorCrearDeArchivo (Vector* v, size_t tamElem, const char* nomArch)
{

    FILE* arch = fopen(nomArch, "rb");

    if (!arch)
        return ERR_ARCHIVO;

    fseek(arch, 0, SEEK_END);
    size_t tamArchivo = ftell(arch);

    v->vec = malloc(tamArchivo);

    if (v->vec == NULL)
    {
        fclose(arch);
        v->cap = 0;
        return ERR_MEMORIA;
    }

    v->ce = tamArchivo / tamElem;
    v->cap = v->ce;
    v->tamElem = tamElem;

    rewind(arch); /*Equivalente a fseek desde 0 SEEK_SET */

    fread(v->vec, tamElem, v->ce, arch);
    fclose(arch);

    return TODO_OK;
}

tError vectorGrabar(Vector* v, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "wb");

    if (!arch)
        return ERR_ARCHIVO;

    fwrite(v->vec, v->tamElem, v->ce, arch);

    fclose(arch);

    return TODO_OK;
}

tError vectorInsertarAlInicio (Vector* v, const void* elem, Cmp cmp)
{
    if(v->ce == v->cap)
        if(redimensionarVector(v, AUMENTAR))
            return ERR_MEMORIA;

    if(vectorDesordBuscar(v, elem, cmp) != NO_HALLADO)
        return DUPLICADO;

    void* ult = v->vec + (v->ce - 1 )* v->tamElem;
    void* posIns = v->vec;

    //for(i = ult; i >= v->vec; i -= v->tamElem) //'i' don�t stay in v->vec
    //    *(i + v->tamElem) = *i;

    memmove(posIns + v->tamElem, posIns, (ult - v->vec) + v->tamElem);

    memcpy(posIns, elem, v->tamElem);

    v->ce ++;

    return TODO_OK;
}

tError vectorInsertarAlFinal (Vector* v, const void* elem)
{
    if(v->ce == v->cap)
        if(redimensionarVector(v, AUMENTAR))
            return ERR_MEMORIA;

    void* posIns = v->vec + v->ce * v->tamElem;
    memcpy(posIns, elem, v->tamElem);
    v->ce ++;

    return TODO_OK;
}

tError vectorInsertarEnPos (Vector* v, const void* elem, int pos, Cmp cmp)
{
    if(v->ce == v->cap)
        if(redimensionarVector(v, AUMENTAR))
            return ERR_MEMORIA;

    if(vectorDesordBuscar(v, elem, cmp) != NO_HALLADO)
        return DUPLICADO;

    void* posIns = v->vec + pos * v->tamElem; //I assume 'pos' as 0 to x
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    //for(void* j = ult; j >= i; j -= v->tamElem)
    //    memcpy(j + v->tamElem, j, v->tamElem);

    memmove(posIns + v->tamElem, posIns, (ult - posIns) + v->tamElem);

    memcpy(posIns, elem, v->tamElem);

    v->ce ++;

    return TODO_OK;
}

tError vectorOrdInsertar (Vector* v, void* elem, Cmp cmp)
{
    if(v->ce == v->cap)
    {
        if(redimensionarVector(v, AUMENTAR))
            return ERR_MEMORIA;
    }

    void* i = v->vec;
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    while(i <= ult && cmp(elem, i) > 0)
        i += v->tamElem;

    if(i <= ult && cmp(elem, i) == 0)
        return DUPLICADO;

    //for(i = ult; i >= posIns; i -= v->tamElem)
    //memcpy(i + v->tamElem, i, v->tamElem);

    memmove(i+ v->tamElem, i, (ult - i) + v->tamElem);

    memcpy(i, elem, v->tamElem);

    v->ce ++;

    return TODO_OK;
}

tError vectorOrdBuscar (const Vector* v, void* elem, Cmp cmp)
{
    if(v->ce == 0)
        return NO_HALLADO;

    const void* li = v->vec;
    const void* ls = v->vec + (v->ce - 1) * v->tamElem;
    const void* med;
    int comp;
    int pos = NO_HALLADO;

    while(li <= ls && pos == NO_HALLADO)
    {
        med = li + ((ls - li) / v->tamElem / 2) * v->tamElem;

        if((comp = cmp(med, elem)) == 0)
            pos = (med - v->vec) / v->tamElem; //De 0 a x
        else
        {
            if(comp > 0)
                ls = med - v->tamElem;
            else
                li = med + v->tamElem;
        }
    }

    if(li > ls)
    {
        return NO_HALLADO;
    }

    memcpy(elem, med, v->tamElem);

    return pos;
}

tError vectorDesordBuscar (const Vector* v, const void* elem, Cmp cmp)
{
    void* i = (Vector*) v->vec;
    const void* ult = v->vec + (v->ce - 1) * v->tamElem;
    int comp;

    while(i <= ult && (comp = cmp(i, elem) )!= 0)
        i += v->tamElem;

    if(comp == 0)
    {
        memcpy((void*)elem, i, v->tamElem); /*Guarda el contenido de i en elem en caso de hallarlo*/
        return (i - v->vec)/v->tamElem;
    }
    else
        return NO_HALLADO;
}

tError vectorObtenerDePos(const Vector* v, void* elem, int pos)
{
    if(pos < 0 || pos >= v->ce)
        return NO_HALLADO;

    const void* origen = (const char*)v->vec + pos * v->tamElem;
    memcpy(elem, origen, v->tamElem);
    return TODO_OK;
}

tError vectorEliminarDePos (Vector* v, int pos)
{
    if(pos >= v->ce)
        return NO_HALLADO;

    void* i = v->vec + pos * v->tamElem;
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    //for(; i < ult; i += v->tamElem)
    //  memcpy(i, i + v->tamElem, v->tamElem);

    memmove(i, i + v->tamElem, ult - i);

    v->ce --;

    if(v->ce / v->cap <= FACTOR_OCUP)
        redimensionarVector(v, DISMINUIR);

    return TODO_OK;
}

tError vectorOrdEliminar (Vector* v, void* elem, Cmp cmp)
{
    int pos = vectorOrdBuscar(v, elem, cmp);
    if(pos == NO_HALLADO)
    {
        return NO_HALLADO;
    }
    //vectorEliminarDePos(v, pos);
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* i =  v->vec;

    while(i <= ult && cmp(elem, i) > 0)
        i += v->tamElem;

    memcpy(elem, i, v->tamElem); // Guardo en elem los datos en i por si se requiere

    memmove(i, i + v->tamElem, (ult - i));

    v->ce --;
    return TODO_OK;
}

void vectorOrdenar(Vector* v, int metodo, Cmp cmp)
{
    switch (metodo)
    {
    case BURBUJEO:
        ordenarBurbujeo(v, cmp);
        break;
    case SELECCION:
        ordenarSeleccion (v, cmp);
        break;
    case SELECCION_DOBLE:
        ordenarSeleccionDoble(v, cmp);
        break;
    case INSERCION:
        ordenarInsercion (v, cmp);
        break;
    case QSORT:
        qsort(v->vec, v->ce, v->tamElem, cmp);
        break;
    }
}

void vectorMostrar (const Vector* v, Imprimir imprimir)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        imprimir (i);
    }
    putchar('\n');
}

void vectorRecorrer (Vector*  v, Accion accion, void* contexto)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        accion(i, contexto);
    }
}

void vectorMezclar (Vector* v)
{
    //Algoritmo Fisher-Yates
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    for (void* i = ult; i > v->vec; i -= v->tamElem)
    {
        int ce = (i - v->vec) / v->tamElem;
        void* j = v->vec + (rand() % (ce + 1)) *v->tamElem;

        intercambiar(i, j, v->tamElem);
    }
}

void vectorVaciar (Vector* v)
{
    v->ce = 0;
}

void vectorDestruir (Vector* v)
{
    free(v->vec);
    v->vec = NULL;
    v->ce = 0;
    v->cap = 0;
    v->tamElem = 0;
}

int redimensionarVector(Vector* v, int operacion)
{
    size_t nuevaCap = operacion == AUMENTAR? v->cap * FACTOR_INCR : maax(v->cap * FACTOR_DECR, CAP_INI);

    void* nVec = realloc(v->vec, nuevaCap * v->tamElem); //I understand that realloc dont do anything if cap was already cap

    if(!nVec)
        return ERR_MEMORIA;

    printf("Redimension de %llu a %llu\n", v->cap, nuevaCap);

    v->vec = nVec;
    v->cap = nuevaCap;

    return TODO_OK;
}

size_t maax(size_t a, size_t b)
{
    return a >= b ? a : b;
}

void ordenarBurbujeo (Vector* v, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* limJ = ult - v->tamElem;

    for(int i = 1; i < v->ce ; i ++)
    {
        limJ -= v->tamElem;
        for(void* j = v->vec; j <= limJ; j += v->tamElem)
            if(cmp(j, j + v->tamElem) > 0)
                intercambiar(j, j + v->tamElem, v->tamElem);
    }
}

void ordenarSeleccion(Vector* v, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* menor;
    for(void* i = v->vec; i < ult; i ++)
    {
        menor = buscarMenor(i,ult, cmp, v->tamElem);
        intercambiar(i, menor, v->tamElem);
    }
}

void ordenarSeleccionDoble (Vector* v, Cmp cmp)
{
    void *i1, *i2, *j, *m, *M;

    i1 = v->vec;
    i2 = v->vec + (v ->ce - 1) * v->tamElem;

    while (i1 < i2)
    {
        m = i1;
        M = i2;

        for (j = i1; j <= i2; j += v->tamElem)
        {
            if (cmp(j, m) < 0)
                m = j;

            if (cmp(j, M) > 0)
                M = j;
        }

        intercambiar(i1, m, v->tamElem);

        if (i1 == M)
            M = m;

        intercambiar(i2, M, v->tamElem);

        i1 += v->tamElem;
        i2 -= v->tamElem;
    }
}

void ordenarInsercion(Vector* v, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* elemAIns = malloc(v->tamElem);
    void* j;

    for(void* i = v->vec + v->tamElem ; i <= ult; i += v->tamElem)
    {
        memcpy(elemAIns, i, v->tamElem);
        j = i - v->tamElem;
        while(j >= v->vec && cmp(j, elemAIns) > 0)
        {
            memcpy(j + v->tamElem, j, v->tamElem);
            j -= v->tamElem;
        }

        memcpy(j + v->tamElem, elemAIns, v->tamElem);
    }

    free(elemAIns);
}

void intercambiar (void* a, void* b, size_t tamElem)
{
    void* aTemp = malloc(tamElem);  //Es muy poca memoria, no deberia fallar
    memcpy(aTemp, a, tamElem);
    memcpy(a, b, tamElem);
    memcpy(b, aTemp, tamElem);
    free(aTemp);
}

void* buscarMenor (void* inicio, void* fin, Cmp cmp, size_t tamElem)
{
    void* menor = inicio;
    for(void* j = inicio + tamElem; j <= fin; j += tamElem)
        if(cmp(j, menor) < 0)
            menor = j;
    return menor;
}

/**Vector iterador**/

void vectorIteradorCrear (VectorIterador* it, const Vector* v)
{
    it->act = NULL;
    it->ult = NULL;
    it->finIter = true;
    it->vec = (Vector*) v;
}

void* vectorIteradorPrimero (VectorIterador* it)
{
    Vector* v = it->vec;

    if (v->ce == 0)
    {
        it->act = NULL;
        it->ult = NULL;
        it->finIter = true;
        return NULL;
    }

    it->act = v->vec;
    it->ult = v->vec + (v->ce - 1) * v->tamElem;
    it->finIter = false;

    return it->act;
}

void* vectorIteradorSiguiente (VectorIterador* it)
{
    Vector* v = it->vec;

    void* siguiente = it->act + v->tamElem;

    if (siguiente > it->ult)
        {
        it->finIter = true;
        return it->act;
    }

    it->act = siguiente;
    it->finIter = false;

    return siguiente;
}

void* vectorIteradorAnterior (VectorIterador* it)
{
    Vector* v = it->vec;

    void* anterior = it->act - v->tamElem;

    if(anterior < v->vec)
    {
        it->finIter = true;
        return it->act;
    }

   it->act = anterior;
   it->finIter = false;

   return anterior;
}

bool vectorIteradorFin (VectorIterador* it)
{
    return it->finIter;
}

