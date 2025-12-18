#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tabla-simbolos/tabla-simbolos.h"
#include "./parser_type.h"
#include <stdbool.h>
#include "./parser-utils.h"

char *_concatenaCadenas(char *str1, char *str2);

// Función que maneja tanto la existencia de la variable como la declaración
bool verificarExistenciaVariable(const char *nombre, const ECondicionesDeExistencia flags) {
    Simbolo *simbolo = buscarSimbolo(nombre);

    const bool debeExistir = flags & VARIABLE_DEBE_EXISTIR;

    // Si debe existir pero no se encuentra la variable, o si no debe existir pero se encuentra
    if ((debeExistir && !simbolo) || (!debeExistir && simbolo)) {
        const char *mensaje = debeExistir ?
            "Error: La variable '%s' no está declarada.\n" :
            "Error: La variable '%s' ya está declarada.\n";
        fprintf(stderr, mensaje, nombre);
        return false;
    }

    // si la variable debe estar inicializada
    if (simbolo && (flags & VARIABLE_DEBE_ESTAR_INICIALIZADA) && !simbolo->_inicializado) {
      fprintf(stderr, "ERROR: la variable '%s' no está inicializada.\n", nombre);
      return false;
    }

    return true;
}

// Función genérica para actualizar el valor de un símbolo, manejando ambos tipos
bool actualizarValorSimbolo(const char *nombre, TTokenValue *valor) {
    if (!verificarExistenciaVariable(nombre, VARIABLE_DEBE_EXISTIR)) return false;

    Simbolo *simbolo = buscarSimbolo(nombre);

    if (simbolo->contante) {
      fprintf(stderr, "ERROR: el identificador '%s' no puede ser reasigando, es constante.\n", nombre);
      return false;
    }

    if ((valor->tipo == TNumerica && simbolo->tipo == TSNumerica)) {
        simbolo->valor.numero = valor->valor.numero;
        if (!simbolo->_inicializado) simbolo->_inicializado = true;
    } else if (valor->tipo == TTextotring && simbolo->tipo == TSCadena) {
        simbolo->valor.cadena = valor->valor.texto;
        if (!simbolo->_inicializado) simbolo->_inicializado = true;
    } else {
        fprintf(stderr, "Error: Tipo de valor no compatible con la variable '%s'.\n", nombre);
        return false;
    }

    return true;
}


// Función para manejar la declaración de variables con o sin asignación
bool manejarDeclaracion(TipoSimbolo tipo, const char *nombre, TTokenValue *valor, bool tieneAsignacion) {
    if (!verificarExistenciaVariable(nombre, VARIABLE_NO_DEBE_EXISTIR)) return false;

    insertarSimbolo(crearSimbolo(tipo, nombre));

    // Si tiene asignación, procesar la asignación
    if (tieneAsignacion) {
        return actualizarValorSimbolo(nombre, valor);
    }
    return true;
}

bool manejarDeclaracionConstante(TipoSimbolo tipo, const char *nombre, TTokenValue *valor) {
  if (!verificarExistenciaVariable(nombre, VARIABLE_NO_DEBE_EXISTIR)) return false;

  Simbolo * simbolo = crearSimbolo(tipo, nombre);

  insertarSimbolo(simbolo);
  actualizarValorSimbolo(nombre, valor);

  simbolo->contante = true;

  return true;
}


// Función para procesar operaciones (suma, resta, concatenación de cadenas)
bool procesarOperacion(TTokenValue *resultado, TTokenValue *op1, TTokenValue *op2, char operacion) {
    if (op1->tipo == TNumerica && op2->tipo == TNumerica) {
        resultado->tipo = TNumerica;
        resultado->valor.numero = (operacion == '+') ?
            op1->valor.numero + op2->valor.numero :
            op1->valor.numero - op2->valor.numero;
    } else if (op1->tipo == TTextotring && op2->tipo == TTextotring && operacion == '+') {
        resultado->tipo = TTextotring;
        resultado->valor.texto = _concatenaCadenas(op1->valor.texto, op2->valor.texto);
        if (!resultado->valor.texto) return false;
    } else {
        fprintf(stderr, "Error: Operación no válida entre tipos incompatibles o no válida para cadenas.\n");
        return false;
    }
    return true;
}

// ------------------------------------
// Metodos Privados
// ------------------------------------

char *_concatenaCadenas(char *str1, char *str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char *resultado = malloc(len1 + len2 + 1); // +1 para el terminador nulo
    if (!resultado) {
        fprintf(stderr, "Error de memoria al concatenar cadenas.\n");
        return NULL;
    }
    strcpy(resultado, str1);
    strcat(resultado, str2);
    return resultado;
}

// ------------------------------------
// Metodos para el Manejo de listas
// ------------------------------------

ListaExpresiones *crearListaExpresiones(void) {
    ListaExpresiones *lista = malloc(sizeof(ListaExpresiones));
    lista->cabeza = NULL;
    lista->size = 0;
    return lista;
}

void agregarAListaExpresiones(ListaExpresiones *lista, TTokenValue *expresion) {
    NodoExpresion *nuevoNodo = malloc(sizeof(NodoExpresion));
    nuevoNodo->expresion = *expresion; // Copiar el contenido de la expresión
    nuevoNodo->siguiente = lista->cabeza; // Insertar al principio
    lista->cabeza = nuevoNodo;
    lista->size++;
}

void liberarListaExpresiones(ListaExpresiones *lista) {
    NodoExpresion *actual = lista->cabeza;
    while (actual != NULL) {
        NodoExpresion *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    free(lista);
}


ListaIdentificadores *crearListaIdentificadores(void) {
    ListaIdentificadores *lista = malloc(sizeof(ListaIdentificadores));
    lista->cabeza = NULL;
    lista->size = 0;
    return lista;
}

void agregarAListaIdentificadores(ListaIdentificadores *lista, const char *nombre) {
    NodoIdentificador *nuevoNodo = malloc(sizeof(NodoIdentificador));
    nuevoNodo->nombre = strdup(nombre); // Copiar el nombre
    nuevoNodo->siguiente = lista->cabeza; // Insertar al principio
    lista->cabeza = nuevoNodo;
    lista->size++;
}

void liberarListaIdentificadores(ListaIdentificadores *lista) {
    NodoIdentificador *actual = lista->cabeza;
    while (actual != NULL) {
        NodoIdentificador *siguiente = actual->siguiente;
        free(actual->nombre); // Liberar el nombre
        free(actual);
        actual = siguiente;
    }
    free(lista);
}


// ------------------------------------
// Metodos Manejo de entrada salida
// ------------------------------------

// Escribir expresiones almacenadas en una lista
bool escribirExpresion(TTokenValue *expresion) {
    printf("Expresion:\n");
    switch (expresion->tipo) {
        case TNumerica:
            printf("  Valor numérico: %d\n", expresion->valor.numero);
            break;
        case TTextotring:
            printf("  Texto: %s\n", expresion->valor.texto);
            break;
        default:
            printf("  Tipo desconocido\n");
            return false;
    }

    return true;
}

void procesarSecuenciasEscape(char *cadena) {
    char *procesada = malloc(strlen(cadena) + 1);
    if (!procesada) {
        fprintf(stderr, "Error de memoria al procesar la cadena.\n");
        return;
    }

    char *src = cadena, *dest = procesada;
    while (*src) {
        if (*src == '\\') {
            src++;
            switch (*src) {
                case 'n': *dest++ = '\n'; break;
                case 't': *dest++ = '\t'; break;
                case 'r': *dest++ = '\r'; break;
                case '\\': *dest++ = '\\'; break;
                default: *dest++ = *src; break;
            }
        } else {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';
    strcpy(cadena, procesada);
    free(procesada);
}

bool leerVariable(const char *nombreVariable) {
    Simbolo *variable = buscarSimbolo(nombreVariable);

    if (!variable) {
        fprintf(stderr, "Error: La variable '%s' no está declarada.\n", nombreVariable);
        return false;
    }

    if (variable->contante) {
      fprintf(stderr, "ERROR: el identificador '%s' no puede ser reasigando, es constante.\n", nombreVariable);
      return false;
    }

    switch (variable->tipo) {
        case TSNumerica:
            printf("Ingrese el valor para %s (numérico): ", nombreVariable);
            scanf("%d", &variable->valor.numero);
            if (!variable->_inicializado) variable->_inicializado = true;
            break;

        case TSCadena:
            printf("Ingrese el texto para %s: ", nombreVariable);
            char buffer[256];
            fgets(buffer, sizeof(buffer), stdin);
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0'; // Eliminar el salto de línea final
            }
            procesarSecuenciasEscape(buffer); // Procesar secuencias de escape
            variable->valor.cadena = strdup(buffer); // Copiar la cadena procesada
            if (!variable->_inicializado) variable->_inicializado = true;
            break;

        default:
            fprintf(stderr, "Error: Tipo desconocido para la variable '%s'.\n", nombreVariable);
            return false;
    }

    return true;
}
