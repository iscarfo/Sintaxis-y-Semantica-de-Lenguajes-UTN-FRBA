#ifndef TP3_PARSER_TYPE
#define TP3_PARSER_TYPE 1

typedef enum {
  TNumerica,
  TTextotring,
  TSimbolo,
} TipoToken;

typedef struct {
  union {
    char * texto; // TNumerica
    int numero; // TNumerica
    char * nombre; // TSimbolo
  } valor;
  TipoToken tipo;
} TTokenValue;

#endif
