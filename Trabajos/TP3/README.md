# TP3 · Interprete del lengauje MICRO
Fecha de entrega: **19/11/2024**

## Referencias

Algunas están en ingles

- **[FLEX](https://en.wikipedia.org/wiki/Flex_(lexical_analyser_generator))**: versión de GNU https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_mono/flex.html

    ```shell
    # En distros Debian/Ubuntu se puede installar con:
    sudo apt install flex
    ```

- **[BISON](https://en.wikipedia.org/wiki/GNU_Bison)**: versión de GNU https://www.gnu.org/software/bison/

    ```shell
    # En distros Debian/Ubuntu se puede installar con:
    sudo apt install bison
    ```


- pequeño manual de la Universidad de Zaragoza

    http://webdiis.unizar.es/asignaturas/LGA/material_2004_2005/Intro_Flex_Bison.pdf

---

## Enunciado

### Consigna del trabajo práctico:
Realizar un intérprete del lenguaje MICRO.

Para ello se debe utilizar FLEX y BISON (cualquier producto que lo implemente) y lenguaje C.

La entrada de las sentencias debe ser por entrada estándar o desde un archivo. El usuario debe
poder seleccionar el tipo de input que realizará.

El programa debe compilar, es decir deben reducirse las expresiones y se deben ejecutar las
sentencias. Además se deben leer y sacar por pantalla los valores correspondientes cuando las
sentencias lo requieran.

### Descripción del lenguaje MICRO (modificado):

MICRO es un lenguaje reducido, con objetivos puramente didácticos. Los únicos tipos de datos
en el lenguaje serán entero y string (vector de de hasta 255 caracteres, incluyendo el `‘\0’`). Si
bien, en MICRO propuesto por Muchnik todos los identificadores son declarados
implícitamente, para esta instancia de trabajo práctico **se solicita declaración explícita, además
poder crear constantes a través de una sentencia de tipo**
```
const nombre := expresión;
```

**Realice las modificaciones que requiera necesarias en la gramática presentada más adelante.**

Los identificadores deben comenzar con una letra o guión bajo, seguido opcionalmente de
letras, dígitos o guiones bajos.

Las constantes son secuencias de dígitos (números enteros) o texto literal (hardcoded) en el
caso de una invocación a “escribir”.

Las sentencias pueden ser:
-  **Declaración**:

    Tiene la forma: `TIPO ID;`

    Ejemplo:
    ```
    int x;
    int _iva;
    string palabra; // es un char v[255];
    ```

    Si una variable es utilizada en una expresión y la variable no está declarada previamente o hay inconsistencia de tipos, el intérprete debe detectar error.

- **Asignación**:

  Tiene la forma: `ID := Expresión;`

  El operador es infijo y se construye con identificadores, constantes y los operadores `+` y
  `–`; los paréntesis están permitidos, por ejemplo `x := 3 + (5 -2);` es válido

-  **Entrada/Salida***

    cuyo formato es:
    ```
    leer (lista de IDs);
    escribir (lista de Expresiones);
    ````

    Cada sentencia termina con un "punto y coma" (`;`).

    El cuerpo de un programa está delimitado por las palabras reservadas `inicio` y `fin`.
    `inicio`, `fin`, `leer` y `escribir` son palabras reservadas y deben escribirse en minúscula.


- **Gramática**

    Definida de la siguiente manera:

    ```
    <objetivo> -> <programa> FDT

    <programa> -> INICIO <listaSentencias> FIN

    <listaSentencias> -> <sentencia> {<sentencia>}

    <sentencia> -> ID ASIGNACIÓN <expresión> PUNTOYCOMA |
                   LEER PARENIZQUIERDO <listaIdentificadores> PARENDERECHO PUNTOYCOMA |
                   ESCRIBIR PARENIZQUIERDO <listaExpresiones> PARENDERECHO PUNTOYCOMA

    <listaIdentificadores> -> ID {COMA ID}<>

    <listaExpresiones> -> <expresión> {COMA <expresión>}

    <expresión> -> <primaria> {<operadorAditivo> <primaria>}

    <primaria> -> ID | CONSTANTE | PARENIZQUIERDO <expresión> PARENDERECHO

    <operadorAditivo> -> SUMA | RESTA
    ```

### Parte teórica:
1. Enumere las fases de compilación de un programa C. Identifique en qué fases
interactúa la tabla de símbolos.

2. Describa el concepto de “espacio de nombres” en C (namespaces) y dé ejemplos. Qué
rol cumple la tabla de símbolos con este concepto?.

3. Investigue y describa el makefile de C. Enumere sus parámetros y dé un ejemplo de uso
utilizando un programa con bibliotecas.

4. Describa los pasos que realiza make para llegar al programa ejecutable. Identifique en
qué paso se confeccionan los programas objeto y explique qué función cumple el
linker.

### Entrega:

Debe entregarse y defenderse el trabajo el **19/11/2024** en una nueva carpeta dentro del
repositorio del grupo. Debe haber al menos un commit de cada integrante del grupo para
aprobar el trabajo.

La entrega debe ser con el programa compilado y funcionando. La misma debe incluir:

- Informe, con carátula que indique los legajos, nombres, apellidos y correos de los
integrantes del grupo. En el informe debe indicarse una descripción de la solución con
hipótesis tenidas en cuenta para la misma, división de tareas en el equipo. La
presentación debe ser prolija, con hojas abrochadas y en folio o carpeta.

- Formato entregable: En el repositorio provisto por la cátedra con código fuente,
ejecutable, casos de validación y versión digital del informe en formato pdf.
