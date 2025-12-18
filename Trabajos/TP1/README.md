# TP1 ·  Generador de lenguajes

Resolución del Trabajo Práctico 1 ([enunciado](https://aulasvirtuales.frba.utn.edu.ar/pluginfile.php/3255347/mod_resource/content/1/2024%20-%20SSL%20-%20K2102%20-%20TP%201.pdf)).

## Estructura del proyecto

- `./src` contiene el código en C
- `./bin/solucion` programa compilado (no es almacenado por git)
- `Makefile` makefile
    
    comandos:
    - `make build` : compila con flags de gcc útiles para debuggin
    - `make release` : compila con flags de gcc de optimización
    - `make clean` : elimina los archivos generados por la compilación
    - `make run` : compila y ejecuta el programa compilado
    - `make all` : equivalente a `make clean build`

    El compilador es `gcc`
    

Notas:
- hecho en C-ANSI 99