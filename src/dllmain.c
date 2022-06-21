#include "boilerplate.h"
#include "helpers.h"
#include "poll.h"

HWND windowHandle = 0;

int CALLBACK
enumWindows (HWND handle, LPARAM param) {
	char buf[64];
	GetClassName (handle, buf, 64);
	if (!strcmp (buf, "nuFoundation.Window")) {
		windowHandle = handle;
		return 0;
	}
	return 1;
}

// force show cursor
HOOK_DYNAMIC (i32, __stdcall, ShowMouse, i32 show) { return originalShowMouse (true); }

#define ON_HIT(bind) IsButtonTapped (bind) ? 0xFFFF : 0

struct Keybindings EXIT          = { .keycodes = { VK_ESCAPE } };
struct Keybindings COIN_ADD      = { .keycodes = { VK_RETURN }, .buttons = { SDL_CONTROLLER_BUTTON_START } };
struct Keybindings TEST          = { .keycodes = { VK_F1 } };
struct Keybindings SERVICE       = { .keycodes = { VK_F2 } };
struct Keybindings DEBUG_UP      = { .keycodes = { VK_UP } };
struct Keybindings DEBUG_DOWN    = { .keycodes = { VK_DOWN } };
struct Keybindings DEBUG_ENTER   = { .keycodes = { VK_RETURN } };
struct Keybindings P1_LEFT_BLUE  = { .keycodes = { 'D' } };
struct Keybindings P1_LEFT_RED   = { .keycodes = { 'F' } };
struct Keybindings P1_RIGHT_RED  = { .keycodes = { 'J' } };
struct Keybindings P1_RIGHT_BLUE = { .keycodes = { 'K' } };
struct Keybindings P2_LEFT_BLUE  = {};
struct Keybindings P2_LEFT_RED   = {};
struct Keybindings P2_RIGHT_RED  = {};
struct Keybindings P2_RIGHT_BLUE = {};

bool testEnabled = false;

u16 __fastcall bnusio_GetAnalogIn (u8 which) {
	switch (which) {
	case 0: return ON_HIT (P1_LEFT_BLUE);  // Player 1 Left Blue
	case 1: return ON_HIT (P1_LEFT_RED);   // Player 1 Left Red
	case 2: return ON_HIT (P1_RIGHT_RED);  // Player 1 Right Red
	case 3: return ON_HIT (P1_RIGHT_BLUE); // Player 1 Right Blue
	case 4: return ON_HIT (P2_LEFT_BLUE);  // Player 2 Left Blue
	case 5: return ON_HIT (P2_LEFT_RED);   // Player 2 Left Red
	case 6: return ON_HIT (P2_RIGHT_RED);  // Player 2 Right Red
	case 7: return ON_HIT (P2_RIGHT_BLUE); // Player 2 Right Blue
	default: return 0;
	}
}

u16 __fastcall bnusio_GetCoin (i32 a1) {
	static int coin_count = 0;
	if (a1 == 1) {
		static bool inited = false;
		if (!inited) {
			EnumWindows (enumWindows, 0);
			InitializePoll (windowHandle);

			toml_table_t *config = openConfig (configPath ("keyconfig.toml"));
			if (config) {
				SetConfigValue (config, "EXIT", &EXIT);

				SetConfigValue (config, "TEST", &TEST);
				SetConfigValue (config, "SERVICE", &SERVICE);
				SetConfigValue (config, "DEBUG_UP", &DEBUG_UP);
				SetConfigValue (config, "DEBUG_DOWN", &DEBUG_DOWN);
				SetConfigValue (config, "DEBUG_ENTER", &DEBUG_ENTER);

				SetConfigValue (config, "COIN_ADD", &COIN_ADD);

				SetConfigValue (config, "P1_LEFT_BLUE", &P1_LEFT_BLUE);
				SetConfigValue (config, "P1_LEFT_RED", &P1_LEFT_RED);
				SetConfigValue (config, "P1_RIGHT_RED", &P1_RIGHT_RED);
				SetConfigValue (config, "P1_RIGHT_BLUE", &P1_RIGHT_BLUE);
				SetConfigValue (config, "P2_LEFT_BLUE", &P2_LEFT_BLUE);
				SetConfigValue (config, "P2_LEFT_RED", &P2_LEFT_RED);
				SetConfigValue (config, "P2_RIGHT_RED", &P2_RIGHT_RED);
				SetConfigValue (config, "P2_RIGHT_BLUE", &P2_RIGHT_BLUE);

				toml_free (config);
			}

			inited = true;
		}

		UpdatePoll (windowHandle);
		if (IsButtonTapped (COIN_ADD) && !testEnabled) coin_count++;
		if (IsButtonTapped (TEST)) testEnabled = !testEnabled;
		if (IsButtonTapped (EXIT)) ExitProcess (0);
	}
	return coin_count;
}

u32 __stdcall bnusio_GetSwIn () {
	u32 sw = 0;
	sw |= (u32)testEnabled << 7;
	sw |= (u32)IsButtonDown (DEBUG_ENTER) << 9;
	sw |= (u32)IsButtonDown (DEBUG_DOWN) << 12;
	sw |= (u32)IsButtonDown (DEBUG_UP) << 13;
	sw |= (u32)IsButtonDown (SERVICE) << 14;
	return sw;
}

i32 __stdcall DllMain (HMODULE mod, DWORD cause, void *ctx) {
	if (cause == DLL_PROCESS_DETACH) DisposePoll ();
	if (cause != DLL_PROCESS_ATTACH) return true;

	init_boilerplate ();

	// Set current directory to the directory of the executable
	// Find all files in the plugins directory that end with .dll
	// Call loadlibraryA on those files
	// Create a message box if they fail to load
	wchar_t path[MAX_PATH];
	GetModuleFileNameW (NULL, path, MAX_PATH);
	*wcsrchr (path, '\\') = '\0';
	SetCurrentDirectoryW (path);

	WIN32_FIND_DATAW fd;
	HANDLE hFind = FindFirstFileW (L"plugins/*.dll", &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
			wchar_t filePath[MAX_PATH];
			wcscpy (filePath, path);
			wcscat (filePath, L"/plugins/");
			wcscat (filePath, fd.cFileName);
			HMODULE hModule = LoadLibraryW (filePath);
			if (!hModule) { MessageBoxW (NULL, L"Failed to load plugin", fd.cFileName, MB_ICONERROR); }
		} while (FindNextFileW (hFind, &fd));
		FindClose (hFind);
	}

	return true;
}