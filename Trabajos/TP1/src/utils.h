#ifndef __TP1_UTLS_H
#define __TP1_UTLS_H 1

#include "./defs.h"
#include "./gramatica.h"

// limpia el buffer de stdin.
// se recomienda utlizarla luego de un getchr o fgets (con stdin) si el caracter
// final no es un \n
void limpiarBufferDeEntrada();

// devuelve 1 si es un no terminal valido, caso contrario devuelve 0
bool_t esNoTerminalValido(char noTerminal);

// devuelve 1 si es un terminal valido, caso contrario devuelve 0
bool_t esTerminalValido(char terminal);

// devuelve 1 (true) si el no terminal se encuentra en la gramática, caso contrario devuelve 0 (false)
bool_t existeNoTerminal(gramatica_t *gramatica, char noTerminal);
// devuelve 1 (true) si el terminal se encuentra en la gramática, caso contrario devuelve 0 (false)
bool_t existeTerminal(gramatica_t *gramatica, char terminal);

void imprimirNoTerminales(gramatica_t * gramaticaUsada);
void imprimirTerminales(gramatica_t * gramaticaUsada);
void imprimirProducciones(lista_produciones_t *producciones);
void imprimirGramatica(gramatica_t * gramaticaUsada);

// Verifica si una cadena contiene al menos un no terminal valido de la gramática.
bool_t contieneNoTerminal(char *cadena, gramatica_t *gramatica);

#endif
