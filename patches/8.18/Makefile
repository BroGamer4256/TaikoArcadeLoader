OUT = patches.818
CC := clang
TARGET := x86_64-pc-windows-gnu
SRC = dllmain.c
OBJ = ${addprefix ${TARGET}/,${SRC:.c=.o}}
EXTERN_SRC = tomlc99/toml.c minhook/src/buffer.c minhook/src/hook.c minhook/src/trampoline.c minhook/src/hde/hde32.c minhook/src/hde/hde64.c
EXTERN_OBJ = ${addprefix ../../${TARGET}/,${EXTERN_SRC:.c=.o}}
CFLAGS = -std=c99 -I../../src -I../../minhook/include -I../../tomlc99 -Wall -Ofast -target ${TARGET} -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=_WIN32_WINNT_WIN7
LDFLAGS := -shared -static -static-libgcc -s
LIBS := -lmingw32 -luuid -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion

all: options ${OUT}

.PHONY: dirs
dirs:
	@mkdir -p ${TARGET}

.PHONY: options
options:
	@echo "CFLAGS	= ${CFLAGS}"
	@echo "LDFLAGS	= ${LDFLAGS}"
	@echo "CC	= ${CC}"

${TARGET}/%.o: %.c
	@echo BUILD $@
	@${CC} -c ${CFLAGS} $< -o $@

.PHONY: ${OUT}
${OUT}: dirs ${DEPS} ${OBJ}
	@echo LINK $@
	@${CC} ${CFLAGS} -o ${TARGET}/$@.dll ${OBJ} ${EXTERN_OBJ} ${LDFLAGS} ${LIBS}
