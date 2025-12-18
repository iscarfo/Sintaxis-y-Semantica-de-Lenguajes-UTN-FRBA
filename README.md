# Sintaxis y Semántica de los Lenguajes (SSL) - UTN FRBA

Este repositorio contiene los trabajos prácticos desarrollados durante el ciclo lectivo 2024 para la materia **Sintaxis y Semántica de los Lenguajes** en la Facultad Regional Buenos Aires (UTN).

---

## 📂 Contenido del Repositorio

### [TP0] Introducción y Entorno
Primer acercamiento al lenguaje C++ y configuración del entorno de desarrollo.
* **Descripción:** Implementación de un "Hola Mundo" técnico.
* **Teoría:** Investigación sobre estándares del lenguaje, procesos de compilación y herramientas de build.

### [TP1] Generador de Lenguajes
Desarrollo de un programa en C para la generación aleatoria de palabras a partir de gramáticas.
* **Objetivo:** Validar si una gramática ingresada por el usuario es **Regular** y generar palabras pertenecientes al lenguaje.
* **Funcionalidades:**
  * Análisis de producciones lineales (izquierda/derecha).
  * Derivación paso a paso desde el axioma inicial.
  * Uso de funciones de carga dinámica de terminales y no terminales.

### [TP2] Expresiones Regulares en Bash
Estudio y aplicación de expresiones regulares (ER) y expresiones regulares extendidas (ERE) utilizando herramientas de scripting en Linux.
* **Temas abordados:**
  * Manipulación de archivos de texto con `sed`.
  * Automatización mediante sentencias condicionales (`if/else`) y cíclicas (`for/while`) en Bash.
  * Modularización mediante subprogramas (funciones) y manejo de argumentos.
* **Documentación:** Incluye informe detallado y **Casos de Prueba** con capturas de ejecución en entorno WSL/Linux.

### [TP3] Compilador
Proyecto final integrador que consiste en la creación de un compilador funcional.
* **Componentes Principales:**
  * **Analizador Léxico (Scanner):** Identificación de tokens.
  * **Analizador Sintáctico (Parser):** Validación de la estructura gramatical.
  * **Tabla de Símbolos:** Gestión de identificadores y tipos de datos.
* **Flujo de Ejecución:** El proceso abarca desde el preprocesamiento hasta la generación del código objeto, utilizando archivos `Makefile` para la gestión de dependencias y automatización de la compilación.
* **Herramientas:** C, GCC, Make, GDB/Valgrind para debugging.

---
