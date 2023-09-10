#include "helpers.h"
#include <windows.h>

void *consoleHandle = 0;

toml_table_t *
openConfig (std::filesystem::path path) {
	if (!std::filesystem::exists (path) || !path.has_filename ()) {
		printWarning ("%s (%s): file does not exist\n", __func__, path.string ().c_str ());
		return 0;
	}

	std::ifstream stream (path);
	if (!stream.is_open ()) {
		printWarning ("%s (%s): could not open\n", __func__, path.string ().c_str ());
		return 0;
	}

	stream.seekg (0, stream.end);
	size_t length = stream.tellg ();
	stream.seekg (0, stream.beg);

	char *buf = (char *)calloc (length + 1, sizeof (char));
	stream.read (buf, length);

	char errorbuf[200];
	toml_table_t *config = toml_parse (buf, errorbuf, 200);
	stream.close ();
	free (buf);

	if (!config) {
		printWarning ("%s (%s): %s\n", __func__, path.string ().c_str (), errorbuf);
		return 0;
	}

	return config;
}

toml_table_t *
openConfigSection (toml_table_t *config, const char *sectionName) {
	toml_table_t *section = toml_table_in (config, sectionName);
	if (!section) {
		printWarning ("%s (%s): cannot find section\n", __func__, sectionName);
		return 0;
	}

	return section;
}

bool
readConfigBool (toml_table_t *table, const char *key, bool notFoundValue) {
	toml_datum_t data = toml_bool_in (table, key);
	if (!data.ok) return notFoundValue;

	return (bool)data.u.b;
}

int64_t
readConfigInt (toml_table_t *table, const char *key, int64_t notFoundValue) {
	toml_datum_t data = toml_int_in (table, key);
	if (!data.ok) return notFoundValue;

	return data.u.i;
}

const char *
readConfigString (toml_table_t *table, const char *key, const char *notFoundValue) {
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
