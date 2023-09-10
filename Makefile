all:
	@meson compile -C build
	@strip build/bnusio.dll

setup:
	@meson setup build --cross cross-mingw-64.txt

dist-no-7z: all
	@mkdir -p out/
	@cp build/bnusio.dll out/
	@cp -r dist/* out/

dist: dist-no-7z
	@cd out && 7z a -t7z ../dist.7z .
	@rm -rf out
