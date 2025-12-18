#!/usr/bin/env bash

# Definición de colores
RED='\033[0;31m'       # Rojo
GREEN='\033[0;32m'     # Verde
YELLOW='\033[0;33m'    # Amarillo
CYAN='\033[0;36m'      # Cian
CYAN_NEGRITA='\033[1;36m'      # Cian
NC='\033[0m'           # Sin color (reset)

# Directorio donde se encuentran los archivos de prueba
TESTS_DIR="./ejemplos"

# Ejecutable de tu proyecto
EXEC="./bin/solucion"

# Archivos de prueba
# todos los archivos en $TESTS_DIR/ ordenados alfabéticamente (salvo los que comiencen con leer_ o no-test_)
mapfile -t TEST_FILES < <( find "$TESTS_DIR" -type f -iname "*.micro" -not \( -iname "leer_*" -or -iname "no-test_*" \) -execdir basename "{}" \; | sort --ignore-case )

# Archivos de log globales
OUTPUT_LOG="output.log"
ERROR_LOG="error.log"

# Limpiar los logs antes de iniciar las pruebas
# true aquí funciona como un noop ("imprime" la cadena vacía)
true > "$OUTPUT_LOG"
true > "$ERROR_LOG"

declare -i tests_exitosos=0;


# Función para ejecutar cada prueba
run_test() {
    local test_file=$1
    echo -e "\n${YELLOW}===== Ejecutando prueba: $test_file =====${NC}"

    # Capturar salida estándar y de error en logs separados
    local temp_error_log="temp_error.log"
    local temp_output_log="temp_output.log"

    # Ejecutar el archivo .micro con tu compilador y capturar salida estándar y errores
    $EXEC < "$TESTS_DIR/$test_file" > "$temp_output_log" 2> "$temp_error_log"
    local exit_code=$?

    # Verificar código de salida y notificar en la consola
    if [ $exit_code -ne 0 ] && ! [[ $test_file =~ ^error_* ]]; then
        echo -e "${RED}Prueba $test_file FALLIDA (código de salida $exit_code)${NC}"
    else
        echo -e "${GREEN}Prueba $test_file completada exitosamente.${NC}"
        (( tests_exitosos++ ))
    fi

    # Guardar el output en el archivo de log, sin mostrarlo en la consola
    if [ -s "$temp_output_log" ]; then
        echo -e "\n[Salida de la prueba: $test_file]" >> "$OUTPUT_LOG"
        echo "=====================" >> "$OUTPUT_LOG"  # Separador entre salidas
        # Eliminar líneas vacías y agregar la salida al log
        grep -v '^[[:space:]]*$' "$temp_output_log" >> "$OUTPUT_LOG"
        echo -e "=====================\n" >> "$OUTPUT_LOG"  # Fin de la sección de salida
    else
        echo -e "[Sin salida en $test_file]" >> "$OUTPUT_LOG"
    fi

    # Mover errores al log global, también mostrándolos en consola
    if [ -s "$temp_error_log" ]; then
        echo -e "${RED}Errores capturados en $test_file:${NC}"
        # Mostrar errores en consola sin el nombre entre corchetes
        while IFS= read -r line; do
            echo -e "${RED}$line${NC}"  # Mostrar el error sin corchetes
            echo "[$test_file] $line" >> "$ERROR_LOG"  # Guardar el error con el nombre del archivo en el log
        done < "$temp_error_log"
    else
        echo -e "${GREEN}Sin errores en $test_file.${NC}"
    fi

    rm -f "$temp_error_log" "$temp_output_log"
    echo -e "${YELLOW}===== Fin de prueba: $test_file =====${NC}\n"
}

# Ejecutar todas las pruebas
for test_file in "${TEST_FILES[@]}"; do
    run_test "$test_file"
done

# Mensaje final
echo -e "${CYAN}Pruebas completadas (${GREEN}${tests_exitosos} exitosas${CYAN} de ${#TEST_FILES[@]}).\n\
Salida guardada en ${CYAN_NEGRITA}$OUTPUT_LOG${CYAN}, errores en ${CYAN_NEGRITA}$ERROR_LOG${CYAN}.${NC}"
