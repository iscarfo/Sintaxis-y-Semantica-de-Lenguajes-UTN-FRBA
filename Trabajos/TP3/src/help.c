#include <stdio.h>

static char * programName = "";

void setProgramName(char* name) {
  programName = name;
}

#define helpStr "%s: Interprete de lenguaje Micro\n" \
  "USO:\n" \
  "\t %s\t\t\t : lee desde la entrada estandar\n" \
  "\t %s <ARCHIVO>\t : lee desde el ARCHIVO\n" \
  "\t %s --help\t\t : imprime esta ayuda\n"

void printHelp(void) {
  printf(helpStr, programName, programName, programName, programName);
}
