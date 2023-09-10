#pragma once
#include <MinHook.h>
#include <bits/stdc++.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <toml.h>
#include <windows.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

#define FUNCTION_PTR(returnType, function, location, ...) returnType (*function) (__VA_ARGS__) = (returnType (*) (__VA_ARGS__)) (location)
#define FUNCTION_PTR_H(returnType, function, ...)         extern returnType (*function) (__VA_ARGS__)

#define PROC_ADDRESS(libraryName, procName) GetProcAddress (LoadLibrary (TEXT (libraryName)), procName)

#define BASE_ADDRESS 0x140000000
#ifdef BASE_ADDRESS
const HMODULE MODULE_HANDLE = GetModuleHandle (nullptr);
#define ASLR(address) ((u64)MODULE_HANDLE + (u64)address - (u64)BASE_ADDRESS)
#endif

#define HOOK(returnType, functionName, location, ...)       \
	typedef returnType (*functionName) (__VA_ARGS__);       \
	functionName original##functionName = NULL;             \
	void *where##functionName           = (void *)location; \
	returnType implOf##functionName (__VA_ARGS__)

#define VTABLE_HOOK(returnType, className, functionName, ...)                      \
	typedef returnType (*className##functionName) (className * This, __VA_ARGS__); \
	className##functionName original##className##functionName = NULL;              \
	void *where##className##functionName                      = NULL;              \
	returnType implOf##className##functionName (className *This, __VA_ARGS__)

#define INSTALL_HOOK(functionName)                                                                                     \
	{                                                                                                                  \
		MH_Initialize ();                                                                                              \
		MH_CreateHook ((void *)where##functionName, (void *)implOf##functionName, (void **)(&original##functionName)); \
		MH_EnableHook ((void *)where##functionName);                                                                   \
	}

#define INSTALL_VTABLE_HOOK(className, object, functionName, functionIndex)                     \
	{                                                                                           \
		where##className##functionName = (*(className##functionName ***)object)[functionIndex]; \
		INSTALL_HOOK (className##functionName);                                                 \
	}

#define READ_MEMORY(location, type) *(type *)location

#define WRITE_MEMORY(location, type, ...)                                                        \
	{                                                                                            \
		const type data[] = {__VA_ARGS__};                                                       \
		DWORD oldProtect;                                                                        \
		VirtualProtect ((void *)(location), sizeof (data), PAGE_EXECUTE_READWRITE, &oldProtect); \
		memcpy ((void *)(location), data, sizeof (data));                                        \
		VirtualProtect ((void *)(location), sizeof (data), oldProtect, &oldProtect);             \
	}

#define WRITE_MEMORY_STRING(location, data, length)                                       \
	{                                                                                     \
		DWORD oldProtect;                                                                 \
		VirtualProtect ((void *)(location), length, PAGE_EXECUTE_READWRITE, &oldProtect); \
		memcpy ((void *)(location), data, length);                                        \
		VirtualProtect ((void *)(location), length, oldProtect, &oldProtect);             \
	}

#define WRITE_NOP(location, count)                                                                 \
	{                                                                                              \
		DWORD oldProtect;                                                                          \
		VirtualProtect ((void *)(location), (size_t)(count), PAGE_EXECUTE_READWRITE, &oldProtect); \
		for (size_t i = 0; i < (size_t)(count); i++)                                               \
			*((uint8_t *)(location) + i) = 0x90;                                                   \
		VirtualProtect ((void *)(location), (size_t)(count), oldProtect, &oldProtect);             \
	}

#define WRITE_NULL(location, count)                                                                \
	{                                                                                              \
		DWORD oldProtect;                                                                          \
		VirtualProtect ((void *)(location), (size_t)(count), PAGE_EXECUTE_READWRITE, &oldProtect); \
		for (size_t i = 0; i < (size_t)(count); i++)                                               \
			*((uint8_t *)(location) + i) = 0x00;                                                   \
		VirtualProtect ((void *)(location), (size_t)(count), oldProtect, &oldProtect);             \
	}

#define COUNTOFARR(arr) sizeof (arr) / sizeof (arr[0])

#define INFO_COLOUR               FOREGROUND_GREEN
#define WARNING_COLOUR            (FOREGROUND_RED | FOREGROUND_GREEN)
#define ERROR_COLOUR              FOREGROUND_RED
#define printInfo(format, ...)    printColour (INFO_COLOUR, format, __VA_ARGS__)
#define printWarning(format, ...) printColour (WARNING_COLOUR, format, __VA_ARGS__)
#define printError(format, ...)   printColour (ERROR_COLOUR, format, __VA_ARGS__)

toml_table_t *openConfig (std::filesystem::path path);
toml_table_t *openConfigSection (toml_table_t *config, const char *sectionName);
bool readConfigBool (toml_table_t *table, const char *key, bool notFoundValue);
int64_t readConfigInt (toml_table_t *table, const char *key, int64_t notFoundValue);
const char *readConfigString (toml_table_t *table, const char *key, const char *notFoundValue);
void printColour (int colour, const char *format, ...);
