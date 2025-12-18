#ifndef TP3_TABLA_SIMBOLOS
#define TP3_TABLA_SIMBOLOS 1

#include <stddef.h>
#include <stdbool.h>

typedef enum {
  TSNumerica,
  TSCadena
} TipoSimbolo;

typedef union {
  char *cadena;
  int numero;
} TipoValor;

typedef struct {
  const TipoSimbolo tipo;
  const char * nombre;
  TipoValor valor;
  bool _inicializado;
  bool contante;
} Simbolo;

void insertarSimbolo(Simbolo *);
Simbolo* buscarSimbolo(const char * nombre);
Simbolo * crearSimbolo(const TipoSimbolo tipo, const char * nombre);


#endif
