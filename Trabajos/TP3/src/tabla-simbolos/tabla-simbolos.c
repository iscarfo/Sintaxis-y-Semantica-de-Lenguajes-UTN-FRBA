#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tabla-simbolos.h"

typedef struct Nodo {
  struct Nodo * next;
  struct Nodo * prev;
  Simbolo * simbolo;
} TNodo;

//Tabla de Simbolos
typedef struct {
  TNodo * cabeza;
  TNodo * cola;
} TablaSimbolos;


//Prototipos privados
TablaSimbolos * _crearTabla(void);
TablaSimbolos * _obtenerTablaSimbolos(void);
TNodo * _crearNodo(Simbolo * simbolo);

//Implementaciones Publicas
void insertarSimbolo(Simbolo * simbolo) {
  TablaSimbolos * tabla = _obtenerTablaSimbolos();

  TNodo * nodo = _crearNodo(simbolo);

  if (tabla->cabeza == NULL) {
    tabla->cabeza = nodo;
    tabla->cola = nodo;
  } else {
    tabla->cola->next = nodo;
    nodo->prev = tabla->cola;
    tabla->cola = nodo;
  }
}

Simbolo * buscarSimbolo(const char * nombre) {
  TNodo * nodo = NULL;

  for (TNodo * _nodo = _obtenerTablaSimbolos()->cola; nodo == NULL && _nodo != NULL; _nodo = _nodo->prev) {
    if (strcmp(_nodo->simbolo->nombre, nombre) == 0) {
      nodo = _nodo;
    }
  }

  return nodo == NULL ? NULL : nodo->simbolo;
}


Simbolo * crearSimbolo(const TipoSimbolo tipo, const char * nombre) {
  // crea e inicializa un nuevo simbolo
  Simbolo * simbolo = memcpy(
    /* nuevo puntero */  malloc(sizeof(Simbolo)),
    /* inciialización */ &(Simbolo){ .tipo=tipo, .nombre=nombre, ._inicializado=false, .contante=false },
    sizeof(Simbolo)
  );

  return simbolo;
}


// funciones privadas
TablaSimbolos * _crearTabla(void) {
  TablaSimbolos * tabla = malloc(sizeof(TablaSimbolos));
  tabla->cabeza = NULL;
  tabla->cola = NULL;
  return tabla;
}

TablaSimbolos * _obtenerTablaSimbolos(void) {
  static TablaSimbolos * tabla = NULL;

  if (tabla == NULL) {
    tabla = _crearTabla();
  }

  return tabla;
}

TNodo * _crearNodo(Simbolo * simbolo) {
  TNodo * nodo = malloc(sizeof(TNodo));
  nodo->next = NULL;
  nodo->prev = NULL;
  nodo->simbolo = simbolo;
  return nodo;
}
