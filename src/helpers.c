#include "helpers.h"
#include <windows.h>

void *consoleHandle = 0;

char *
configPath (char *name) {
	static char buffer[MAX_PATH];
	GetModuleFileNameA (NULL, buffer, MAX_PATH);
	*(strrchr (buffer, '\\') + 1) = 0;
	strcat_s (buffer, MAX_PATH, name);
	return buffer;
}

toml_table_t *
openConfig (char *configFilePath) {
	FILE *file = fopen (configFilePath, "r");
	if (!file) {
		printWarning ("%s (%s): cannot open file\n", __func__, configFilePath);
		return 0;
	}
	char errorbuf[200];
	toml_table_t *config = toml_parse_file (file, errorbuf, 200);
	fclose (file);

	if (!config) {
		printWarning ("%s (%s): %s\n", __func__, configFilePath, errorbuf);
		return 0;
	}

	return config;
}

toml_table_t *
openConfigSection (toml_table_t *config, char *sectionName) {
	toml_table_t *section = toml_table_in (config, sectionName);
	if (!section) {
		printWarning ("%s (%s): cannot find section\n", __func__, sectionName);
		return 0;
	}

	return section;
}

bool
readConfigBool (toml_table_t *table, char *key, bool notFoundValue) {
	toml_datum_t data = toml_bool_in (table, key);
	if (!data.ok) return notFoundValue;

	return (bool)data.u.b;
}

int64_t
readConfigInt (toml_table_t *table, char *key, int64_t notFoundValue) {
	toml_datum_t data = toml_int_in (table, key);
	if (!data.ok) return notFoundValue;

	return data.u.i;
}

char *
readConfigString (toml_table_t *table, char *key, char *notFoundValue) {
	toml_datum_t data = toml_string_in (table, key);
	if (!data.ok) return notFoundValue;

	return data.u.s;
}

void
printColour (int colour, const char *format, ...) {
	va_list args;
	va_start (args, format);

	if (consoleHandle == 0) consoleHandle = GetStdHandle (STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute (consoleHandle, colour);
	vprintf (format, args);
	SetConsoleTextAttribute (consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

	va_end (args);
}