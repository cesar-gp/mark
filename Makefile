# Makefile para Mark, intérprete de Markdown de Cesitah.
# --
#
# Las secciones "Dependencias" y "Dependencias opcionales"
# contienen información sobre el software necesario para
# compilar el programa.
#
# RECETAS:
#  - all (predeterminada):	compila el programa.
#  - clean:					limpia los archivos de compilación.
#  - run:					compila y ejecuta el programa.
#  - execute:				ejecuta el programa sin recompilar.
#
# OPCIONES DE CONFIGURACIÓN:
#  - DEBUG=true:			ejecuta el programa en un depurador.
#  - UNTESTED=true:			ignora los tests y ejecuta directamente.

# ---- Entrada ----

# Carpetas
DIR_SRC=$(abspath src)
DIR_TEST=${DIR_SRC}/test

# Archivos
FILE_MAIN="${DIR_SRC}/main.c"

# Lista de archivos de código del programa
INFLAGS=-mindepth 1
INEXCLUDE=-path "${DIR_TEST}" -prune -o
INFILTERS=-type f -name "*.c"
INOUTPUT=-printf "%h/%f "

IN=$(shell find "${DIR_SRC}" ${INFLAGS} ${INEXCLUDE} ${INFILTERS} ${INOUTPUT})

# Lista de archivos de código para tests
ifneq (${UNTESTED}, true)
	INTESTEXCLUDE=-path "${FILE_MAIN}" -prune -o
	INTEST=$(shell find "${DIR_SRC}" ${INFLAGS} ${INTESTEXCLUDE} ${INFILTERS} ${INOUTPUT})
endif

# ---- Salida ----

# Carpetas
DIR_TARGET=$(abspath target)

# Archivos
OUT=${DIR_TARGET}/mark

ifneq (${UNTESTED}, true)
	OUTTEST=${OUT}-test
endif

# ---- Dependencias ----

# GCC. Versión mínima recomendada: 16.1.1.
CC=gcc
CCFLAGS=-Wall -Wextra
CCINCLUDE=-iquote "${DIR_SRC}/include"

ifeq (${DEBUG}, true)
	CCFLAGS:=${CCFLAGS} -g
endif

ifneq (${UNTESTED}, true)
	CCINCLUDETEST:=-iquote "${DIR_TEST}/include"
endif

# ---- Dependencias opcionales ----

# GDB (para variable DEBUG). Versión mínima recomendada: 17.2.
DEBUGGER=gdb
DEBUGFLAGS=

# ---- Recetas ----

# Recetas de uso general
all: ${OUT}

execute: ${OUT}
ifeq (${DEBUG}, true)
	"${DEBUGGER}" "${OUT}"
else
	"${OUT}"
endif

clean:
	rm -rf "${DIR_TARGET}"

run: clean execute

# Recetas internas
${OUT}: ${DIR_TARGET}
ifneq (${UNTESTED}, true)
	"${CC}" ${CCFLAGS} ${CCINCLUDE} ${CCINCLUDETEST} -o "${OUTTEST}" ${INTEST}
	"${OUTTEST}"
endif

	"${CC}" ${CCFLAGS} ${CCINCLUDE} -o "${OUT}" ${IN}

${DIR_TARGET}:
	mkdir "${DIR_TARGET}"