#include "./producciones.h"
#include <stdio.h>
#include <stdlib.h>
#include "./defs.h"

bool_t agregarProduccion(lista_produciones_t *producciones, produccion_t *production) {
  if (producciones->cantidad == 10)
    return true;

  producciones->list[producciones->cantidad++] = production;

  return false;
}

produccion_t *crearProduccion(char noTerminal) {
  produccion_t *p = malloc(sizeof(produccion_t));

  // como malloc devuelve un espacio de memoria con datos basura,
  // tenemos que inicializar todo.

  p->noTerminal = noTerminal;
  p->cantidadDeOpciones = 0;
  for (short i = 0; i < 10; i++)
    p->opciones[i] = 0;

  return p;
}

void eliminarProduccion(produccion_t * produccion) {
  free(produccion);
}

produccion_t *buscarProduccion(char noTerminal, lista_produciones_t *producciones) {
  for (unsigned short i = 0; i < producciones->cantidad; i++) {
    if (producciones->list[i]->noTerminal == noTerminal) {
      return producciones->list[i];
    }
  }

  return NULL;
}

bool_t agregarOpcionAProduccion(produccion_t *produccion, char *opcion) {
  if (produccion->cantidadDeOpciones >= 10)
    return true;

  produccion->opciones[produccion->cantidadDeOpciones++] = opcion;
  return false;
}
