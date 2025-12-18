%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "./scanner.h"
  #include "parser-utils.h"

  void yyerror(char const *errmsg);

%}

%code requires {
  #include "parser_type.h"
  #include "tabla-simbolos/tabla-simbolos.h"
  #include "parser-utils.h"
}

%union {
  TTokenValue token;
  TipoSimbolo tipo;
  Simbolo * simbolo;
  ListaIdentificadores *listaIdentificadores;
  ListaExpresiones *listaExpresiones;
}

%token INICIO FIN LEER ESCRIBIR INT STRING PUNTOYCOMA COMA ASIGNACION SUMA RESTA PARENIZQUIERDO PARENDERECHO CONST

%token <token> ID
%token <token> CONSTANTE
%type <token> primaria
%type <token> expresion
%type <tipo> tipo
%type <listaIdentificadores> listaIdentificadores
%type <listaExpresiones> listaExpresiones

%start objetivo

%%

// Reglas de la gramatica
objetivo:
    programa
    ;

programa:
    INICIO listaSentencias FIN
    ;

listaSentencias:
    sentencia
    | listaSentencias sentencia
    ;

sentencia:
    asignacion
    | declaracion
    | LEER PARENIZQUIERDO listaIdentificadores PARENDERECHO PUNTOYCOMA {
        NodoIdentificador *actual = $3->cabeza;
        while (actual != NULL) {
            if (!leerVariable(actual->nombre)) YYABORT; // no hace falta limpiar memoria en el abort.

            actual = actual->siguiente;
        }
        liberarListaIdentificadores($3);
    }

    | ESCRIBIR PARENIZQUIERDO listaExpresiones PARENDERECHO PUNTOYCOMA {
        NodoExpresion *actual = $3->cabeza;
        while (actual != NULL) {
          escribirExpresion(&(actual->expresion));
          actual = actual->siguiente;
        }
        liberarListaExpresiones($3);
    }

declaracion:
      CONST tipo ID PUNTOYCOMA {
        // esto no es parte del lenguaje,
        // pero se pone acá para permitir arrojar un error semántico
        // en lugar de uno sintáctico y con un mensaje más descriptivo

        fprintf(stderr, "ERROR: la constante '%s' no fue inicializada.\n", $3.valor.nombre);
        fprintf(stderr, "ERROR: las constantes deben ser asignadas al momento de su declaración.\n");
        YYABORT;
      }
    | CONST tipo ID ASIGNACION expresion PUNTOYCOMA {
          if (!manejarDeclaracionConstante($2, $3.valor.nombre, &($5)))  YYABORT;
      }
    | tipo ID ASIGNACION expresion PUNTOYCOMA {
          if (!manejarDeclaracion($1, $2.valor.nombre, &($4), true))  YYABORT;
      }
    | tipo ID PUNTOYCOMA {
          if (!manejarDeclaracion($1, $2.valor.nombre, NULL, false))  YYABORT;
      }
    ;



asignacion:
  ID ASIGNACION expresion PUNTOYCOMA {
    if (!actualizarValorSimbolo($1.valor.nombre, &($3))) YYABORT;
  }

tipo:
    INT   { $<tipo>$ = TSNumerica; }
  | STRING { $<tipo>$ = TSCadena; }
  ;


listaIdentificadores:
    ID {
        $$ = crearListaIdentificadores();
        agregarAListaIdentificadores($$, $1.valor.nombre);
    }
    | ID COMA listaIdentificadores {
        agregarAListaIdentificadores($3, $1.valor.nombre);
        $$ = $3;
    }
    ;

listaExpresiones:
    expresion {
        $$ = crearListaExpresiones();
        agregarAListaExpresiones($$, &$1);
    }
    | expresion COMA listaExpresiones {
        agregarAListaExpresiones($3, &$1);
        $$ = $3;
    }
    ;

expresion:
    primaria
    | expresion SUMA primaria {
          if (!procesarOperacion(&($$), (&$1), &($3), '+')) { YYABORT; }
      }
    | expresion RESTA primaria {
          if (!procesarOperacion(&($$), (&$1), &($3), '-')) { YYABORT; }
      }
    ;


primaria:
    ID {
        if (!verificarExistenciaVariable($1.valor.nombre, VARIABLE_DEBE_EXISTIR | VARIABLE_DEBE_ESTAR_INICIALIZADA))  YYABORT;
        Simbolo * simbolo = buscarSimbolo($1.valor.nombre);
        if (simbolo->tipo == TSNumerica) {
          $$.tipo = TNumerica;
          $$.valor.numero = simbolo->valor.numero;
        } else {
          $$.tipo = TTextotring;
          $$.valor.texto = simbolo->valor.cadena;
        }
      }
    | CONSTANTE
    | PARENIZQUIERDO expresion PARENDERECHO { $$ = $2; }
    ;

%%

void yyerror(char const *errmsg) {
    extern char* yytext;  // Texto que causó el error
    extern int yylineno;  // Línea de la entrada
    extern FILE* yyin;    // Archivo de entrada actual

    // Imprimir mensaje de error de sintaxis
    fprintf(stderr, "Error de sintaxis: %s\n", errmsg);

    // Imprimir el archivo y la línea si es posible
    if (yyin) {
        fprintf(stderr, "En el archivo: %s, línea: %d\n", yyin ? "<entrada>" : "desconocido", yylineno);
    }

    // Imprimir el texto que causó el error
    if (yytext) {
        fprintf(stderr, "Texto que causó el error: '%s'\n", yytext);
    }

    // Detectar posibles errores de asignación o declaración
    if (strstr(yytext, "=")) {
        fprintf(stderr, "Parece un error en la asignación o declaración.\n");
    }

    // Terminar el programa en caso de error
    exit(1);
}


