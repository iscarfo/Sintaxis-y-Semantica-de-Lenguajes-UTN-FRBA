#ifndef TP3_PARSER_UTILS
#define TP3_PARSER_UTILS 1

  #include "tabla-simbolos/tabla-simbolos.h"
  #include "parser_type.h"
  #include <stdbool.h>

  typedef struct NodoExpresion {
    TTokenValue expresion;
    struct NodoExpresion *siguiente;
  } NodoExpresion;

  typedef struct {
      NodoExpresion *cabeza;
      size_t size;
  } ListaExpresiones;

  typedef struct NodoIdentificador {
      char *nombre;
      struct NodoIdentificador *siguiente;
  } NodoIdentificador;

  typedef struct {
      NodoIdentificador *cabeza;
      size_t size;
  } ListaIdentificadores;

typedef enum {
  VARIABLE_NO_DEBE_EXISTIR = 0, // 0b0
  VARIABLE_DEBE_EXISTIR = 1, // 0b1
  VARIABLE_DEBE_ESTAR_INICIALIZADA = 2, // ob10
} ECondicionesDeExistencia;


  bool verificarExistenciaVariable(const char *nombre, const ECondicionesDeExistencia flags);
  bool manejarDeclaracion(TipoSimbolo tipo, const char *nombre, TTokenValue *valor, bool tieneAsignacion);
  bool manejarDeclaracionConstante(TipoSimbolo tipo, const char *nombre, TTokenValue *valor);
  bool actualizarValorSimbolo(const char *nombre, TTokenValue *valor);

  bool procesarOperacion(TTokenValue *resultado, TTokenValue *op1, TTokenValue *op2, char operacion);

  ListaExpresiones *crearListaExpresiones(void);
  void agregarAListaExpresiones(ListaExpresiones *lista, TTokenValue *expresion);
  void liberarListaExpresiones(ListaExpresiones *lista);

  ListaIdentificadores *crearListaIdentificadores(void);
  void agregarAListaIdentificadores(ListaIdentificadores *lista, const char *nombre);
  void liberarListaIdentificadores(ListaIdentificadores *lista);

  void procesarSecuenciasEscape(char *cadena);
  bool leerVariable(const char *nombreVariable);
  bool escribirExpresion(TTokenValue *nodoActual) ;

#endif
