OUT = TAL
CC := x86_64-w64-mingw32-gcc
CXX := x86_64-w64-mingw32-g++
TARGET := x86_64-pc-windows-gnu
SDL_TARGET := x86_64-w64-mingw32
SRC = src/dllmain.c src/helpers.c src/poll.c src/boilerplate.c src/card.cpp tomlc99/toml.c minhook/src/buffer.c minhook/src/hook.c minhook/src/trampoline.c minhook/src/hde/hde32.c minhook/src/hde/hde64.c
OBJ = ${addprefix ${TARGET}/,${subst .cpp,.o,${SRC:.c=.o}}}
CFLAGS = -std=c99 -Iminhook/include -ISDL/${SDL_TARGET}/include -ISDL/include -Itomlc99 -Wall -Ofast -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=_WIN32_WINNT_WIN7
CXXFLAGS = -std=c++17 -Imingw-std-threads -Iminhook/include -ISDL/${SDL_TARGET}/include -ISDL/include -Itomlc99 -Wall -Ofast -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=_WIN32_WINNT_WIN7
LDFLAGS := -shared -static -static-libgcc -s
LIBS := SDL/${SDL_TARGET}/build/.libs/libSDL2.a SDL/${SDL_TARGET}/build/.libs/libSDL2main.a -lmingw32 -luuid -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion -pthread
DEPS = SDL

all: options ${OUT}

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

${TARGET}/%.o: %.cpp
	@echo BUILD $@
	@${CXX} -c ${CXXFLAGS} $< -o $@

.PHONY: SDL
SDL:
	@mkdir -p SDL/${SDL_TARGET}
	@cd SDL/${SDL_TARGET} && ../configure --build=x86_64-linux-gnu --host=${SDL_TARGET} --disable-sdl2-config --disable-shared --enable-assertions=release --enable-directx --enable-haptic 
	@make -s -C SDL/${SDL_TARGET}

.PHONY: ${OUT}
${OUT}: dirs ${DEPS} ${OBJ}
	@echo LINK $@
	@${CXX} ${CXXFLAGS} -o ${TARGET}/$@.dll ${OBJ} ${LDFLAGS} ${LIBS}

.PHONY: fmt
fmt:
	@cd src && clang-format -i *.h *.c -style=file
	@cd patches/8.18 && clang-format -i *.c -style=file

.PHONY: clean
clean:
	rm -rf ${TARGET}

.PHONY: patches
patches:
	make -C patches/8.18

.PHONY: dist-no-7z
dist-no-7z: options ${OUT} patches
	mkdir -p out/plugins
	cp ${TARGET}/${OUT}.dll out/
	mv out/${OUT}.dll out/bnusio.dll
	cp ${TARGET}/patches.*.dll out/plugins
	cp dist/* out/

.PHONY: dist
dist: dist-no-7z
	cd out && 7z a -t7z ../${OUT}.7z .
	rm -rf out
