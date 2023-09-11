project('TaikoArcadeLoader', 'c', 'cpp', 'nasm', version: '1.0.0')
warning_level = 3
debug = true
optimization = 3
b_lto = true
b_pgo = 'use'
cpp_std = 'c++20'

cpp = meson.get_compiler('cpp')

add_project_arguments(
	cpp.get_supported_arguments(
		'-D_WIN32_WINNT=_WIN32_WINNT_WIN10',
		'-std=c++20',
		'-Werror',
	),
	language: 'cpp',
)

add_project_link_arguments(
	cpp.get_supported_arguments(
		'-static',
		#'-s',
		'-lws2_32',
		'-lssp',
	),
	language: 'cpp',
)

minhook = subproject('minhook')
tomlc99 = subproject('tomlc99')
sdl2 = subproject('sdl2', default_options: ['default_library=static', 'test=false', 'use_render=disabled'])
xxhash = subproject('xxhash', default_options: ['default_library=static', 'cli=false'])

library(
	'bnusio',
	link_with: [
		minhook.get_variable('minhook_lib'),
		tomlc99.get_variable('tomlc99_lib'),
		sdl2.get_variable('sdl2'),
		xxhash.get_variable('xxhash'),
	],
	include_directories: [
		'src',
		minhook.get_variable('minhook_inc'),
		tomlc99.get_variable('tomlc99_inc'),
		sdl2.get_variable('core_inc'),
		xxhash.get_variable('inc'),
	],
	sources : [
		'src/dllmain.cpp',
		'src/helpers.cpp',
		'src/poll.cpp',
		'src/bnusio.cpp',
		'src/patches/jp_nov_2020.cpp',
		'src/patches/cn_jun_2023.cpp',
		'src/patches/amauth.cpp',
	],
	name_prefix: ''
)
