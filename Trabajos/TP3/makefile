# Set compiler options
CC=gcc
BISON=bison
FLEX=flex

# Compiler flags
CSHARED=-pedantic-errors -std=c99 -D_XOPEN_SOURCE=700
CDEBUG=-g -Wall -DDEBUG -fdiagnostics-color=always

# Linker flags
LDFLAGS=

# Set source files and headers
SRCS_C = $(filter-out src/parser.c src/scanner.c, $(shell find src/ -iname "*.c")) src/parser.c src/scanner.c
SRCS_H = $(filter-out src/parser.h src/scanner.h, $(shell find src/ -iname "*.h")) src/parser.h src/scanner.h

IS_WSL=$(shell (grep Microsoft /proc/version) && echo 1 || echo 0)
ifeq ($(IS_WSL), 1)
	LDFLAGS=-mconsole -mwindows
endif

# custom deps
DEPS =

# archivos intermedios
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS_C))

# Set binary targets
BIN = bin/solucion

# punto de entrada
all: clean clean-deps build


##### Reglas de compilación por archivo #####

# compilación final
$(BIN): $(OBJS) | $(dir $(BIN))
	$(CC) $(CFLAGS) -o "$@" $^ $(LDFLAGS)

# compilación de archivos intermedios
obj/%.o: src/%.c $(SRCS_H) $(DEPS) | $(dir $(OBJS))
	$(CC) $(CFLAGS) -c -o "$@" $<

# crea las carpetas bin/ y obj/
$(sort $(dir $(BIN) $(OBJS))):
	mkdir -pv $@

# compila el archivo micro.y con bison
src/parser.c src/parser.h &: src/micro.y
	$(BISON) -d $< -o src/parser.c

# compila el archivo micro.l con flex
src/scanner.c src/scanner.h &: src/micro.l
	$(FLEX) -o src/scanner.c --header-file=src/scanner.h $<


##### comandos #####

# instrucción build, agrega variables de ambiente y "llama" a la compilación
.PHONY: build
build: CFLAGS = $(CSHARED) $(CDEBUG)
build: $(BIN)

# instrucción para compilar los archivos de flex y bison
.PHONY: build-deps
build-deps: src/parser.c src/parser.h src/scanner.c src/scanner.h

# elimina los archivos creados por flex y bison
.PHONY: clean-deps
clean-deps:
	-rm -f src/parser.c src/parser.h src/scanner.c src/scanner.h

# elimina los archivos de compilación e intermedios
.PHONY: clean
clean:
	-rm -rfv obj bin

# ejecuta el programa compilado
.PHONY: run
run: build
	exec ./bin/solucion

.PHONY: test
test: clean clean-deps build
	exec ./ejemplos/run_tests.sh
