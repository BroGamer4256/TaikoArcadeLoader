OUT = what_the_fuck
CXX := x86_64-w64-mingw32-g++
TARGET := x86_64-pc-windows-gnu
SRC = dllmain.cpp
OBJ = ${addprefix ../../${TARGET}/patches/what_the_fuck/,${SRC:.cpp=.o}}
EXTERN_SRC = src/helpers.c tomlc99/toml.c minhook/src/buffer.c minhook/src/hook.c minhook/src/trampoline.c minhook/src/hde/hde32.c minhook/src/hde/hde64.c
EXTERN_OBJ = ${addprefix ../../${TARGET}/,${EXTERN_SRC:.c=.o}}
CXXFLAGS = -std=c++11 -I../../src -I../../minhook/include -I../../tomlc99 -Wall -Ofast -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=_WIN32_WINNT_WIN7
LDFLAGS := -shared -static -static-libgcc -s
LIBS := -lmingw32 -luuid -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion -pthread

all: options ${OUT}

.PHONY: dirs
dirs:
	@mkdir -p ../../${TARGET}/patches/what_the_fuck

.PHONY: options
options:
	@echo "CFLAGS	= ${CFLAGS}"
	@echo "LDFLAGS	= ${LDFLAGS}"
	@echo "CXX	= ${CXX}"

../../${TARGET}/patches/what_the_fuck/%.o: %.cpp
	@echo BUILD $@
	@${CXX} -c ${CXXFLAGS} $< -o $@

.PHONY: ${OUT}
${OUT}: dirs ${OBJ}
	@echo LINK $@
	@${CXX} ${CXXFLAGS} -o ../../${TARGET}/$@.dll ${OBJ} ${EXTERN_OBJ} ${LDFLAGS} ${LIBS}
