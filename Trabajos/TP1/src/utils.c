#include "./defs.h"
#include "./gramatica.h"
#include "./producciones.h"
#include <stdio.h>

void limpiarBufferDeEntrada() {
  while (getchar() != '\n')
    continue;
}

bool_t esNoTerminalValido(char noTerminal) {
  return noTerminal >= 'A' && noTerminal <= 'Z';
}

bool_t esTerminalValido(char terminal) {
  return terminal >= 'a' && terminal <= 'z';
}

bool_t existeNoTerminal(gramatica_t *gramatica, char noTerminal) {
  // buscar si ya existe
  for (short i = 0; i < gramatica->cantNoTerminales; i++) {
    if (gramatica->noTerminales[i] == noTerminal)
      return true;
  }

  // es válido y está disponible para ser agregado
  return false;
}

bool_t existeTerminal(gramatica_t *gramatica, char terminal) {
  for (short i = 0; i < gramatica->cantTerminales; i++) {
      if (gramatica->terminales[i] == terminal)
        return true;
    }

    return false;
}

bool_t contieneNoTerminal(char *cadena, gramatica_t *gramatica) {
    // Verifica si una cadena contiene al menos un no terminal valido de la gramática.
    for (int i = 0; cadena[i] != '\0'; ++i) {
        if (esNoTerminalValido(cadena[i])) {
            return true;
        }
    }
    return false;
}

// impresión

void imprimirProducciones(lista_produciones_t *producciones) {
  produccion_t *p;
  for (size_t i = 0; i < producciones->cantidad; i++) {
    p = producciones->list[i];

    printf("\t%c -> %s", p->noTerminal, p->opciones[0]);

    for (size_t o = 1; o < p->cantidadDeOpciones; o++)
      printf(" | %s", p->opciones[o]);

    printf("\n");
  }
}

void imprimirNoTerminales(gramatica_t *gramaticaUsada) {
  for (short i = 0; i < 5 && i < gramaticaUsada->cantNoTerminales; i++)
    printf("%c ", gramaticaUsada->noTerminales[i]);
}

void imprimirTerminales(gramatica_t *gramaticaUsada) {
  for (short i = 0; i < 5 && i < gramaticaUsada->cantTerminales; i++)
    printf("%c ", gramaticaUsada->terminales[i]);
}

void imprimirGramatica(gramatica_t *gramaticaUsada) {
  printf("\nGramática formal: ({ ");
  imprimirNoTerminales(gramaticaUsada);
  printf("}, { ");
  imprimirTerminales(gramaticaUsada);
  printf("}, P, %c)\n", gramaticaUsada->axioma);

  printf("donde P = {\n");
  imprimirProducciones(&gramaticaUsada->producciones);
  printf("}\n\n");
}
