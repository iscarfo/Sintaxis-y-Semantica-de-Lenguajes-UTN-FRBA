#ifndef __TP1_PRODUCCIONES_H
#define __TP1_PRODUCCIONES_H 1

#include "./defs.h"
#include <stddef.h>

typedef struct {
  char noTerminal;
  char *opciones[10];
  size_t cantidadDeOpciones;
} produccion_t;

typedef struct {
  produccion_t *list[10];
  size_t cantidad;
} lista_produciones_t;

// devuelve 1 (true) si no se pudo agregar la produción, en caso contrario devuelve 0 (false)
bool_t agregarProduccion(lista_produciones_t *producciones, produccion_t *production);

// devuelve un puntero produccion_t con valores inicalizados a 0
produccion_t *crearProduccion(char noTerminal);

void eliminarProduccion(produccion_t *);

// busca una producíon en el parámetro producciones cuya parte
// izquierda sea el no terminal indicado en el parámetro noTerminal.
// Si al consigue, devuelve un putero a dicha produción, si no la consigue,
// devuelve puntero NULL
produccion_t *buscarProduccion(char noTerminal, lista_produciones_t *producciones);

// devuelve 1 (true) si no se pudo agregar la opción, en caso contrario devuelve 0 (false)
bool_t agregarOpcionAProduccion(produccion_t *produccion, char *opcion);

#endif // __TP1_PRODUCCIONES_H
