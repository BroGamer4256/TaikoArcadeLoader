#include "boilerplate.h"
#include "helpers.h"
#include "poll.h"

bool testEnabled = false;
u16 drumMax      = 0xFFFF;
u16 drumMin      = 0xFFFF;

typedef i32 (*callbackAttach) (i32, i32, i32 *);
typedef void (*callbackTouch) (i32, i32, u8[168], u64);

#define ON_HIT(bind) IsButtonTapped (bind) ? drumMax == drumMin ? drumMax : (u16)(rand () % drumMax + drumMin) : 0

Keybindings EXIT          = { .keycodes = { VK_ESCAPE } };
Keybindings TEST          = { .keycodes = { VK_F1 } };
Keybindings SERVICE       = { .keycodes = { VK_F2 } };
Keybindings DEBUG_UP      = { .keycodes = { VK_UP } };
Keybindings DEBUG_DOWN    = { .keycodes = { VK_DOWN } };
Keybindings DEBUG_ENTER   = { .keycodes = { VK_RETURN } };
Keybindings COIN_ADD      = { .keycodes = { VK_RETURN }, .buttons = { SDL_CONTROLLER_BUTTON_START } };
Keybindings CARD_INSERT   = { .keycodes = { 'P' } };
Keybindings P1_LEFT_BLUE  = { .keycodes = { 'D' }, .axis = { SDL_AXIS_LTRIGGER_DOWN } };
Keybindings P1_LEFT_RED   = { .keycodes = { 'F' }, .buttons = { SDL_CONTROLLER_BUTTON_LEFTSTICK } };
Keybindings P1_RIGHT_RED  = { .keycodes = { 'J' }, .buttons = { SDL_CONTROLLER_BUTTON_RIGHTSTICK } };
Keybindings P1_RIGHT_BLUE = { .keycodes = { 'K' }, .axis = { SDL_AXIS_RTRIGGER_DOWN } };
Keybindings P2_LEFT_BLUE  = {};
Keybindings P2_LEFT_RED   = {};
Keybindings P2_RIGHT_RED  = {};
Keybindings P2_RIGHT_BLUE = {};

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
		static bool inited       = false;
		static HWND windowHandle = 0;

		if (!inited) {
			windowHandle = FindWindowA ("nuFoundation.Window", 0);

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

callbackAttach attach;
i32 *attachData;
callbackTouch touch;
u64 touchData;

u32 __stdcall bnusio_GetSwIn () {
	u32 sw = 0;
	sw |= (u32)testEnabled << 7;
	sw |= (u32)IsButtonDown (DEBUG_ENTER) << 9;
	sw |= (u32)IsButtonDown (DEBUG_DOWN) << 12;
	sw |= (u32)IsButtonDown (DEBUG_UP) << 13;
	sw |= (u32)IsButtonDown (SERVICE) << 14;

	if (attach != 0) {
		attach (0, 0, attachData);
		attach = 0;
	}
	if (touch != 0) {
		u8 data[168]
		    = { 0x01, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0x2E, 0x58, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x5C, 0x97, 0x44, 0xF0, 0x88, 0x04, 0x00, 0x43, 0x26, 0x2C, 0x33, 0x00, 0x04,
			    0x06, 0x10, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
			    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30,
			    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x42, 0x47, 0x49, 0x43, 0x36,
			    0x00, 0x00, 0xFA, 0xE9, 0x69, 0x00, 0xF6, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		memcpy (data + 0x2C, "7F5C9744F111111143262C3300040610", 34);
		memcpy (data + 0x50, "30764352518498791337", 22);
		touch (0, 0, data, touchData);
		touch = 0;
	}
	return sw;
}

HOOK_DYNAMIC (u64, __stdcall, bngrw_attach, i32 a1, char *a2, i32 a3, i32 a4, callbackAttach callback, i32 *a6) {
	attach     = callback;
	attachData = a6;
	return 1;
}

HOOK_DYNAMIC (i32, __stdcall, bngrw_reqWaitTouch, u32 a1, i32 a2, u32 a3, callbackTouch callback, u64 a5) {
	if (!IsButtonDown (CARD_INSERT)) return -1;
	touch     = callback;
	touchData = a5;
	return 1;
}

i32 __stdcall DllMain (HMODULE mod, DWORD cause, void *ctx) {
	if (cause == DLL_PROCESS_DETACH) DisposePoll ();
	if (cause != DLL_PROCESS_ATTACH) return true;

	init_boilerplate ();
	INSTALL_HOOK_DYNAMIC (bngrw_attach, PROC_ADDRESS ("bngrw.dll", "BngRwAttach"));
	INSTALL_HOOK_DYNAMIC (bngrw_reqWaitTouch, PROC_ADDRESS ("bngrw.dll", "BngRwReqWaitTouch"));

	// Set current directory to the directory of the executable
	// Find all files in the plugins directory that end with .dll
	// Call loadlibraryW on those files
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
			if (!hModule) {
				wchar_t buf[128];
				swprintf (buf, 128, L"Failed to load plugin %d", GetLastError ());
				MessageBoxW (NULL, buf, fd.cFileName, MB_ICONERROR);
			}
		} while (FindNextFileW (hFind, &fd));
		FindClose (hFind);
	}

	toml_table_t *config = openConfig (configPath ("config.toml"));
	if (config) {
		drumMax = readConfigInt (config, "drumMax", drumMax);
		drumMin = readConfigInt (config, "drumMin", drumMin);
		toml_free (config);
	}

	return true;
}