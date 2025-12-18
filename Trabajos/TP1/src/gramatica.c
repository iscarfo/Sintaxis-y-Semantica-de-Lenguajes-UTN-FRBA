#include "./gramatica.h"

gramatica_t * inicializarGramtica() {
    // solo se necesita una gramática en la app
    // por lo que las consideramos singleton
    static gramatica_t gramatica;

    // inicialización
    for (unsigned short i = 0; i < 5; i++) {
        gramatica.terminales[i] =
        gramatica.noTerminales[i] = '\0';
    }
    gramatica.cantNoTerminales = 0;
    gramatica.cantTerminales = 0;
    if (gramatica.producciones.cantidad) {
      for (short i = 0; i < gramatica.producciones.cantidad; i++) {
        eliminarProduccion(gramatica.producciones.list[i]);
        gramatica.producciones.list[i] = NULL;
      }
    }
    gramatica.producciones.cantidad = 0;
    gramatica.axioma = '\0';

    return &gramatica;
}
