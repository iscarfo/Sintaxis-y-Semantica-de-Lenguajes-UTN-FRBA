/*El presente trabajo consiste en realizar un programa en lenguaje C
que permita generar aleatoriamente palabras de un lenguaje regular a
partir de una gramática ingresada por el usuario. */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "./producciones.h"
#include "./defs.h"
#include "./utils.h"

typedef enum {
    LINEAL_A_IZQUIERDA = 0,
    LINEAL_A_DERECHA = 1,
    NO_LINEAL = -1,
    NO_APLICA = 2,
} linealidad_t;


void cargarNoTerminales(gramatica_t *g);
void cargarTerminales(gramatica_t *g);
void cargarProducciones(gramatica_t *g);
void cargarAxioma(gramatica_t *g);

void ingresarGramatica(gramatica_t *g);
linealidad_t verificarLinealidad(char * ladoDerecho, char *terminales, char * noTerminales);
bool_t esGramaticaRegular(gramatica_t *g);
void generarPalabras(gramatica_t *g);


int main() {
    gramatica_t * gramaticaUsada;

    while(true)
    {
        gramaticaUsada = inicializarGramtica();

        ingresarGramatica(gramaticaUsada);

        if (esGramaticaRegular(gramaticaUsada)) {
            break;
        } else {
            printf("Gramatica no regular, ingrese otra\n");
        }
    }

    generarPalabras(gramaticaUsada);

    return 0;
}


/* [1] Ingresar una gramática:
 Es decir que se debe poder ingresar la cuatrupla: (símbolos terminales, símbolos no terminales, producciones, axioma). */

void cargarNoTerminales(gramatica_t *gramaticaUsada){
    printf("Indique los no terminales en letras mayuscula "
        "(ejemplo: S T D H) y presione enter \n"
        "> "
    );

    char buffer[50];

    fgets(buffer, 50, stdin);

    if (strchr(buffer, '\n') == NULL) limpiarBufferDeEntrada();

    // buscamos no terminales en el buffer, ignorando carateres inválidos (como epsacios)
    // hasta el final de la cadena
    for (size_t i = 0; i < 50 && gramaticaUsada->cantNoTerminales < 5; i++) {
        // fin de cadena
        if (buffer[i] == '\0') break;

        if (esNoTerminalValido(buffer[i]) && !existeNoTerminal(gramaticaUsada, buffer[i])) {
            // agregar no terminal
            gramaticaUsada->noTerminales[gramaticaUsada->cantNoTerminales] = buffer[i];
            gramaticaUsada->cantNoTerminales++;
        }
    }


    printf("\nNo terminales ingresados: ");
    imprimirNoTerminales(gramaticaUsada);
    printf("\n\n");
}

void cargarTerminales(gramatica_t *gramaticaUsada){
    printf("Indique los terminales en letras minuscula "
        "(ejemplo: s t d h) y presione enter \n"
        "> "
    );

    char buffer[50];

    fgets(buffer, 50, stdin);

    if (strchr(buffer, '\n') == NULL) limpiarBufferDeEntrada();

    for (size_t i = 0; i < 50 && gramaticaUsada->cantTerminales < 5; i++) {
        if (buffer[i] == '\0') break;

        if (esTerminalValido(buffer[i]) && !existeTerminal(gramaticaUsada, buffer[i])) {
            // agregar no terminal
            gramaticaUsada->terminales[gramaticaUsada->cantTerminales] = buffer[i];
            gramaticaUsada->cantTerminales++;
        }
    }


    printf("\nTerminales ingresados: ");
    imprimirTerminales(gramaticaUsada);
    printf("\n\n");
}

void cargarAxioma(gramatica_t *gramaticaUsada) {
    while (true) {
        printf("Indique el axioma: ");
        scanf("%c", &gramaticaUsada->axioma);
        limpiarBufferDeEntrada();

        if (!esNoTerminalValido(gramaticaUsada->axioma) || !existeNoTerminal(gramaticaUsada, gramaticaUsada->axioma)) {
            printf("[ERROR] axiona invalido, debe ser uno de los no terminales previamente ingresados:");
            imprimirNoTerminales(gramaticaUsada);
            printf("\n");
        } else {
            // el axioma es válido, salimos del ciclo porque ya no necesitamos pedir uno nuevo.
            break;
        }
    }
}

void cargarProducciones(gramatica_t *gramaticaUsada) {
    while(true) {
        printf("Agregue una produccion con este formato (S -> aT): ");
        char noTerminal = '\0';
        char * desarrollo = calloc(3, sizeof(char *));

        scanf("%c -> %2[a-zA-Z]", &noTerminal, desarrollo);

        limpiarBufferDeEntrada();

        produccion_t * produccion = buscarProduccion(noTerminal, &gramaticaUsada->producciones);

        if (produccion == NULL) {
            produccion = crearProduccion(noTerminal);

            if (agregarProduccion(&gramaticaUsada->producciones, produccion)) {
                printf("se alcanzo el numero maximo de producciones posibles\n");
                eliminarProduccion(produccion);
                break;
            };

        }

        if (agregarOpcionAProduccion(produccion, desarrollo)) {
            printf("se alcanzo el numero maximo de produccion posibles para el no terminal %c\n", noTerminal);
            continue;
        };

        fprintf(stderr, "desea agregar una nueva produccion? [S/n] ");
        char response = getchar();
        if (response == 's' || response == 'S') {
            limpiarBufferDeEntrada();
            continue;
        } else if (response == '\n') {
            continue;
        } else {
            limpiarBufferDeEntrada();
            // el ususario ya no quiere ingresar más terminales
            break;
        }

    }

    printf("\nproducciones agregadas:\n");
    imprimirProducciones(&gramaticaUsada->producciones);
    printf("\n");
}

void ingresarGramatica(gramatica_t *gramaticaUsada){
    cargarNoTerminales(gramaticaUsada);
    cargarTerminales(gramaticaUsada);
    cargarProducciones(gramaticaUsada);
    cargarAxioma(gramaticaUsada);

    imprimirGramatica(gramaticaUsada);
}


/* [2]Validar si la gramática es regular o no:
 El programa no debe permitir el ingreso de gramáticas que no sean regulares.
*/

bool_t esGramaticaRegular(gramatica_t *gramaticaUsada) {
    linealidad_t tipo = 0;
    bool_t prodDoble = false;

    lista_produciones_t * producciones = &gramaticaUsada->producciones;

    produccion_t * produccion = NULL;

    if (gramaticaUsada->cantNoTerminales > gramaticaUsada->producciones.cantidad) {
        // faltan producciones para los no terminales
        return false;
    }

    for (int i = 0; i < producciones->cantidad; i++) {
        produccion = producciones->list[i];

        // Verificar que el lado izquierdo es un no terminal válido para la gramática
        if (!strchr(gramaticaUsada->noTerminales, produccion->noTerminal)) {
            return false;
        }

        // verificar linealidad de las producciones que empiezan con el mismo no terminal
        for (size_t o = 0; o < produccion->cantidadDeOpciones; o++) {
            // Verificar tipo de linealidad
            linealidad_t resultado = verificarLinealidad(produccion->opciones[o], gramaticaUsada->terminales, gramaticaUsada->noTerminales);
            if (resultado == NO_LINEAL) {
                return false;
            } else if (resultado == NO_APLICA) {
                // se considera válido
                continue;
            } if (prodDoble == false) {
                // si es la primera vez, se considera válido y se usa su linealidad para validar el resto de producciones
                tipo = resultado;
                prodDoble = true;
            } else if (tipo != resultado) {
                return false;
            }
        }
    }

    return true;
}


linealidad_t verificarLinealidad(char * ladoDerecho, char *terminales, char * noTerminales) {
    if (strlen(ladoDerecho) == 1) {
        if (strchr(terminales, ladoDerecho[0])) {
            return NO_APLICA;
        } else {
            return NO_LINEAL; /* si la cadena tiene longitud 1 y esta conformada por un no terminal entonces no es regular */
        }
    } else {
        if (strlen(ladoDerecho) == 2) {
            if (strchr(noTerminales, ladoDerecho[0]) && strchr(terminales, ladoDerecho[1])) {
                return LINEAL_A_IZQUIERDA;
            } else {
                if (strchr(terminales, ladoDerecho[0]) && strchr(noTerminales, ladoDerecho[1])) {
                    return LINEAL_A_DERECHA;
                }
            }
        }
    }

    return NO_LINEAL;
}

/* [3] Generar aleatoriamente palabras del lenguaje
 Cada vez que genera una palabra nueva debe mostrar el proceso de
derivación que se realiza. Puede mostrar una derivaciónhorizontal. */

void generarPalabras(gramatica_t *gramatica) {
    char palabra[100];
    srand(time(NULL));
  
    while (true) {
        

        strcpy(palabra, &gramatica->axioma);

        printf("Proceso de derivacion:\n");
        printf("%s\n", palabra);

        while (contieneNoTerminal(palabra, gramatica)) {
            for (int i = 0; palabra[i] != '\0'; ++i) {
                if (esNoTerminalValido(palabra[i])) {

                    produccion_t *produccion = buscarProduccion(palabra[i], &gramatica->producciones);

                    if (produccion == NULL) {
                        fprintf(stderr, "[ERROR] no se encontro produccion para el no terminal %c\n", palabra[i]);
                        exit(1);
                    }

                    int opcion = rand() % produccion->cantidadDeOpciones;
                    char *derivacion = produccion->opciones[opcion];

                    char nuevaPalabra[100];
                    strncpy(nuevaPalabra, palabra, i);
                    nuevaPalabra[i] = '\0';
                    strcat(nuevaPalabra, derivacion);
                    strcat(nuevaPalabra, &palabra[i+1]);

                    strcpy(palabra, nuevaPalabra);

                    printf("%s\n", palabra);
                    break;
                }
            }
        }

        printf("Palabra generada: %s\n", palabra);


        printf("Desea generar otra palabra? [S/n]: ");
        char respuesta;
        scanf("%c", &respuesta);

        if (respuesta == 's' || respuesta == 'S') {
            limpiarBufferDeEntrada();
            continue;
        } else if (respuesta == '\n') {
            continue;
        } else {
            limpiarBufferDeEntrada();
            break;
        }
    }
}

