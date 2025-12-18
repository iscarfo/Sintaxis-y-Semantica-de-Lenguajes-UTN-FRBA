%{
  #include <stdlib.h>
  #include <string.h>
  #include <stdio.h>
  #include "./parser.h"
  #include "tabla-simbolos/tabla-simbolos.h"
%}

  // Seccion de Reglas

DIGITO  [0-9]
LETRA   [a-zA-Z_]

%%
  // Seccion de Codigo
  // Palabras reservadas
inicio            { return INICIO; }
fin               { return FIN; }
leer              { return LEER; }
escribir          { return ESCRIBIR; }

int               { return INT; }
string            { return STRING; }
const             { return CONST; }


{LETRA}({LETRA}|{DIGITO})*  {
  yylval.token.valor.nombre = strdup(yytext);
  yylval.token.tipo = TSimbolo;
  return ID;
}



[-]?{DIGITO}+ {
  yylval.token.valor.numero = atof(yytext);
  yylval.token.tipo = TNumerica;
  return CONSTANTE;
}

\"([^\"\\]|\\[ntr])*\" {
  char *original = yytext + 1; // Remover la primera comilla
  original[strlen(original) - 1] = '\0'; // Remover la última comilla

  // Procesar las secuencias escapadas
  char *procesada = malloc(strlen(original) + 1);
  char *src = original, *dest = procesada;
  while (*src) {
    if (*src == '\\') {
      src++;
      switch (*src) {
        case 'n': *dest++ = '\n'; break;
        case 't': *dest++ = '\t'; break;
        case 'r': *dest++ = '\r'; break;
        default: *dest++ = *src; break;
      }
    } else {
      *dest++ = *src;
    }
    src++;
  }
  *dest = '\0';

  yylval.token.tipo = TTextotring;
  yylval.token.valor.texto = procesada; // Guardar la cadena procesada
  return CONSTANTE;
}


","               { return COMA; }
";"               { return PUNTOYCOMA; }
":="              { return ASIGNACION; }
"+"               { return SUMA; }
"-"               { return RESTA; }
"("               { return PARENIZQUIERDO; }
")"               { return PARENDERECHO; }

. { /* Ignorar caracteres no reconocidos */ }

%%

int yywrap(void) {
   return 1;
}
