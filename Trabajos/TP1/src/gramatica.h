#ifndef __TP1_GRAMATICA_H
#define __TP1_GRAMATICA_H 1

#include <stddef.h>
#include "producciones.h"

typedef struct {
  char noTerminales[5];
  size_t cantNoTerminales;

  char terminales[5];
  size_t cantTerminales;

  lista_produciones_t producciones;

  char axioma;
} gramatica_t;

gramatica_t * inicializarGramtica();

#endif // __TP1_GRAMATICA_H
