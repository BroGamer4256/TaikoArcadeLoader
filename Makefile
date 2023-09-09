OUT = TAL
CC = x86_64-w64-mingw32-gcc
TARGET = x86_64-pc-windows-gnu
SRC = src/dllmain.c src/helpers.c src/poll.c src/boilerplate.c tomlc99/toml.c minhook/src/buffer.c minhook/src/hook.c minhook/src/trampoline.c minhook/src/hde/hde32.c minhook/src/hde/hde64.c
OBJ = ${addprefix ${TARGET}/,${SRC:.c=.o}}
CFLAGS = -std=c99 -Iminhook/include -ISDL/include -ISDL/build/include -Itomlc99 -Wall -Ofast -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=_WIN32_WINNT_WIN7
LDFLAGS = -shared -static -static-libgcc -s
LIBS = SDL/build/libSDL3.a SDL/build/libSDL3_main.a -lmingw32 -luuid -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion
DEPS = SDL

all: options ${OUT} plugins

.PHONY: dirs
dirs:
	@mkdir -p ${TARGET}/src
	@mkdir -p ${TARGET}/minhook/src/hde
	@mkdir -p ${TARGET}/tomlc99

.PHONY: options
options:
	@echo "CFLAGS	= ${CFLAGS}"
	@echo "LDFLAGS	= ${LDFLAGS}"
	@echo "CC	= ${CC}"

${TARGET}/%.o: %.c
	@echo BUILD $@
	@${CC} -c ${CFLAGS} $< -o $@

.PHONY: SDL
SDL:
	@cd SDL && cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build-scripts/cmake-toolchain-mingw64-x86_64.cmake && cmake --build build

.PHONY: ${OUT}
${OUT}: dirs ${DEPS} ${OBJ}
	@echo LINK $@
	@${CC} ${CFLAGS} -o ${TARGET}/$@.dll ${OBJ} ${LDFLAGS} ${LIBS}

.PHONY: fmt
fmt:
	@cd src && clang-format -i *.h *.c -style=file
	@cd plugins/8.18 && clang-format -i *.c -style=file
	@cd plugins/amauth && clang-format -i *.c -style=file

.PHONY: clean
clean:
	rm -rf ${TARGET}

.PHONY: plugins
plugins:
	make -C plugins/8.18
	make -C plugins/amauth
	strip ${TARGET}/*.dll

.PHONY: dist-no-7z
dist-no-7z: options ${OUT} plugins
	mkdir -p out/plugins
	cp ${TARGET}/${OUT}.dll out/
	mv out/${OUT}.dll out/bnusio.dll
	cp ${TARGET}/patches.*.dll out/plugins
	cp ${TARGET}/amauth.dll out/plugins
	cp -r dist/* out/

.PHONY: dist
dist: dist-no-7z
	cd out && 7z a -t7z ../${OUT}.7z .
	rm -rf out
