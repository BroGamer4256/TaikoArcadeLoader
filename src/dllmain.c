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

int coin_count   = 0;
bool testEnabled = false;
bool inited      = false;

HOOK_DYNAMIC (i64, __fastcall, DecCoin, i32 a1, u16 a2) {
	coin_count -= a2;
	return false;
}

HOOK_DYNAMIC (u16, __fastcall, GetAnalogIn, u8 which) {
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

HOOK_DYNAMIC (u16, __fastcall, GetCoin, i32 a1) {
	if (a1 == 1) {
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
		if (IsButtonTapped (COIN_ADD)) coin_count++;
		if (IsButtonTapped (TEST)) testEnabled = !testEnabled;
		if (IsButtonTapped (EXIT)) ExitProcess (0);
	}
	return coin_count;
}

HOOK_DYNAMIC (u32, __stdcall, GetSwIn) {
	u32 mask = 0;
	mask |= (u32)testEnabled << 7;
	mask |= (u32)IsButtonDown (DEBUG_ENTER) << 9;
	mask |= (u32)IsButtonDown (DEBUG_DOWN) << 12;
	mask |= (u32)IsButtonDown (DEBUG_UP) << 13;
	mask |= (u32)IsButtonDown (SERVICE) << 14;
	return mask;
}

HOOK_DYNAMIC (i64, __stdcall, ResetCoin) {
	coin_count = 0;
	return false;
}

HOOK (i32, __stdcall, CrtMain, 0x140666d2c, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, i32 nShowCmd) {
	// Blatantly stolen patches from mon.im
	WRITE_MEMORY (0x1400239C0, u8, 0xC3);       // Stop error
	WRITE_MEMORY (0x140314E8D, u8, 0xB0, 0x01); // Unlock songs
	WRITE_MEMORY (0x140692E17, u8, 0xEB);       // Shared audio
	WRITE_MEMORY (0x140517339, u8, 0xBA, 0x00, 0x00, 0x00, 0x00,
	              0x90); // Disable VSync
	// Save settings cross session
	WRITE_MEMORY (0x140B5C528, u8, "./Setting1.bin");
	WRITE_MEMORY (0x140B5C538, u8, "./Setting2.bin");

	return originalCrtMain (hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

i32 __stdcall DllMain (HMODULE mod, DWORD cause, void *ctx) {
	if (cause == DLL_PROCESS_DETACH) DisposePoll ();
	if (cause != DLL_PROCESS_ATTACH) return true;

	INSTALL_HOOK (CrtMain);
	INSTALL_HOOK_DYNAMIC (ShowMouse, PROC_ADDRESS ("user32.dll", "ShowCursor"));
	INSTALL_HOOK_DYNAMIC (DecCoin, PROC_ADDRESS ("bnusio.dll", "bnusio_DecCoin"));
	INSTALL_HOOK_DYNAMIC (GetAnalogIn, PROC_ADDRESS ("bnusio.dll", "bnusio_GetAnalogIn"));
	INSTALL_HOOK_DYNAMIC (GetCoin, PROC_ADDRESS ("bnusio.dll", "bnusio_GetCoin"));
	INSTALL_HOOK_DYNAMIC (GetSwIn, PROC_ADDRESS ("bnusio.dll", "bnusio_GetSwIn"));
	INSTALL_HOOK_DYNAMIC (ResetCoin, PROC_ADDRESS ("bnusio.dll", "bnusio_ResetCoin"));
	init_boilerplate ();

	return true;
}