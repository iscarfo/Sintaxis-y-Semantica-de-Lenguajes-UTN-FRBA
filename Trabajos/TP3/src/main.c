#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./scanner.h"
#include "./parser.h"
#include "./help.h"


int main(int argc, char* argv[]) {
  FILE *file = NULL;

  setProgramName(argv[0]);

  if (argc == 2) {

    #define firstArg argv[1]

    if (strcmp("--help", firstArg) == 0) {
      printHelp();
      return 0;
    }

    else {
      // utiliza
      file = fopen(firstArg, "r");

      if (file == NULL) {
        fprintf(stderr, "The file \"%s\" could't be open.\n", firstArg);
        return 1;
      }

      // establece el nuevo archivo para ser leido por flex
      yyset_in(file);
      fprintf(stderr, "leyendo archivo %s\n", firstArg);
    }
  }


  if (file == NULL) {
    printf("Leyendo desde la entrda estandar\n");
  }

  int result = yyparse();

  if (file != NULL) {
    fclose(file);
  }

  return result;
}
