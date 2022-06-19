#include "helpers.h"
#include "poll.h"
#include <math.h>

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

// show cursor
HOOK_DYNAMIC (i32, __stdcall, ShowMouse, i32 show) {
	return originalShowMouse (true);
}

// xinput stuff
HOOK_DYNAMIC (u32, __stdcall, XinputGetState, u32 index, void *state) {
	return ERROR_DEVICE_NOT_CONNECTED;
}

HOOK_DYNAMIC (u32, __stdcall, XinputSetState, u32 index, void *state) {
	return ERROR_DEVICE_NOT_CONNECTED;
}

HOOK_DYNAMIC (u32, __stdcall, XinputGetCapabilites, u32 index, u32 flags,
			  void *state) {
	return ERROR_DEVICE_NOT_CONNECTED;
}

#define DRUM_HIT 20000
#define IF_HIT(bind) return IsButtonTapped (bind) ? DRUM_HIT : 0

struct Keybindings COIN_ADD = { .keycodes = { VK_RETURN },
								.buttons = { SDL_CONTROLLER_BUTTON_START } };
struct Keybindings TEST = { .keycodes = { VK_F1 } };
struct Keybindings SERVICE = { .keycodes = { VK_F2 } };
struct Keybindings DEBUG_UP = { .keycodes = { VK_UP } };
struct Keybindings DEBUG_DOWN = { .keycodes = { VK_DOWN } };
struct Keybindings DEBUG_ENTER = { .keycodes = { VK_RETURN } };
struct Keybindings P1_LEFT_BLUE = { .keycodes = { 'D' } };
struct Keybindings P1_LEFT_RED = { .keycodes = { 'F' } };
struct Keybindings P1_RIGHT_RED = { .keycodes = { 'J' } };
struct Keybindings P1_RIGHT_BLUE = { .keycodes = { 'K' } };
struct Keybindings P2_LEFT_BLUE = {};
struct Keybindings P2_LEFT_RED = {};
struct Keybindings P2_RIGHT_RED = {};
struct Keybindings P2_RIGHT_BLUE = {};

int coin_count = 0;
bool testEnabled = false;
bool inited = false;

// bnusio stuff
HOOK_DYNAMIC (i64, __stdcall, ClearSram) { return false; }

HOOK_DYNAMIC (i64, __stdcall, Close) { return false; }

HOOK_DYNAMIC (i64, __fastcall, Communication, i32 a1) { return false; }

HOOK_DYNAMIC (i64, __fastcall, DecCoin, i32 a1, u16 a2) {
	coin_count -= a2;
	return false;
}

HOOK_DYNAMIC (i64, __fastcall, DecService, i32 a1, u16 a2) { return false; }

HOOK_DYNAMIC (u16, __fastcall, GetAnalogIn, u8 which) {
	switch (which) {
	case 0: // Player 1 Left Blue
		IF_HIT (P1_LEFT_BLUE);
	case 1: // Player 1 Left Red
		IF_HIT (P1_LEFT_RED);
	case 2: // Player 1 Right Red
		IF_HIT (P1_RIGHT_RED);
	case 3: // Player 1 Right Blue
		IF_HIT (P1_RIGHT_BLUE);
	case 4: // Player 2 Left Blue
		IF_HIT (P2_LEFT_BLUE);
	case 5: // Player 2 Left Red
		IF_HIT (P2_LEFT_RED);
	case 6: // Player 2 Right Red
		IF_HIT (P2_RIGHT_RED);
	case 7: // Player 2 Right Blue
		IF_HIT (P2_RIGHT_BLUE);
	default:
		return 0;
	}
}

HOOK_DYNAMIC (void *, __fastcall, GetBuffer, u16 a1, i64 a2, i16 a3) {
	return false;
}

HOOK_DYNAMIC (i64, __fastcall, GetCDOut, u8 a1) { return false; }

HOOK_DYNAMIC (u16, __fastcall, GetCoin, i32 a1) {
	if (a1 == 1) {
		if (!inited) {
			EnumWindows (enumWindows, 0);
			InitializePoll (windowHandle);

			toml_table_t *config = openConfig (configPath ("keyconfig.toml"));
			if (config) {
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
		if (IsButtonTapped (COIN_ADD)) {
			coin_count++;
		}
		if (IsButtonTapped (TEST))
			testEnabled = !testEnabled;
	}
	return coin_count;
}

HOOK_DYNAMIC (void *, __fastcall, GetCoinError, i32 a1) { return false; }

HOOK_DYNAMIC (i64, __fastcall, GetCoinLock, u8 a1) { return false; }

HOOK_DYNAMIC (u64, __stdcall, GetEncoder) { return false; }

HOOK_DYNAMIC (void *, __stdcall, GetExpansionMode) { return false; }

HOOK_DYNAMIC (void *, __stdcall, GetFirmwareVersion) {
	return (void *)(u16)126;
}

HOOK_DYNAMIC (u8, __stdcall, GetGout, u8 a1) { return false; }

HOOK_DYNAMIC (i64, __stdcall, GetHopOut, u8 a1) { return false; }

HOOK_DYNAMIC (char *, __stdcall, GetIoBoardName) { return false; }

HOOK_DYNAMIC (u16, __fastcall, GetRegisterU16, i16 a1) { return false; }

HOOK_DYNAMIC (u8, __fastcall, GetRegisterU8, u16 a1) { return false; }

HOOK_DYNAMIC (void *, __fastcall, GetService, i32 a1) { return false; }

HOOK_DYNAMIC (void *, __fastcall, GetServiceError, i32 a1) { return false; }

HOOK_DYNAMIC (u16, __fastcall, GetStatusU16, u16 a1) { return false; }

HOOK_DYNAMIC (u8, __fastcall, GetStatusU8, u16 a1) { return false; }

HOOK_DYNAMIC (u32, __stdcall, GetSwIn) {
	u32 mask = 0;
	mask |= (u32)testEnabled << 7;
	mask |= (u32)IsButtonDown (DEBUG_ENTER) << 9;
	mask |= (u32)IsButtonDown (DEBUG_DOWN) << 12;
	mask |= (u32)IsButtonDown (DEBUG_UP) << 13;
	mask |= (u32)IsButtonDown (SERVICE) << 14;
	return mask;
}

HOOK_DYNAMIC (u64, __stdcall, GetSwIn64) { return false; }

HOOK_DYNAMIC (void *, __stdcall, GetSystemError) { return false; }

HOOK_DYNAMIC (u8, __stdcall, IsConnected) { return false; }

HOOK_DYNAMIC (u8, __stdcall, IsWideUsio) { return false; }

HOOK_DYNAMIC (i64, __stdcall, Open) { return false; }

HOOK_DYNAMIC (i64, __stdcall, ResetCoin) {
	coin_count = 0;
	return false;
}

HOOK_DYNAMIC (i32, __stdcall, ResetIoBoard) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SetBuffer, u16 a1, i32 a2, i16 a3) {
	return false;
}

HOOK_DYNAMIC (i64, __fastcall, SetCDOut, u8 a1, u8 a2) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SetCoinLock, u8 a1, u8 a2) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SetExpansionMode, i16 a1) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SetGout, u8 a1, u8 a2) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SetHopOut, u8 a1, u8 a2) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SetHopperLimit, u16 a1, i16 a2) {
	return false;
}

HOOK_DYNAMIC (i64, __fastcall, SetHopperRequest, u16 a1, i16 a2) {
	return false;
}

HOOK_DYNAMIC (void *, __fastcall, SetPLCounter, i16 a1) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SetRegisterU16, u16 a1, u16 a2) {
	return false;
}

HOOK_DYNAMIC (i64, __fastcall, SetRegisterU8, u16 a1, u8 a2) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SetSystemError, i16 a1) { return false; }

HOOK_DYNAMIC (i64, __fastcall, SramRead, i32 a1, u8 a2, i32 a3, u16 a4) {
	return false;
}

HOOK_DYNAMIC (i64, __fastcall, SramWrite, i32 a1, u8 a2, i32 a3, u16 a4) {
	return false;
}

HOOK_DYNAMIC (i64, __stdcall, UsbFinderInitialize) { return false; }

HOOK_DYNAMIC (i64, __stdcall, UsbFinderRelease) { return false; }

HOOK_DYNAMIC (i64, __fastcall, UsbFinderGetSerialNumber, i32 a1, i64 a2) {
	return true;
}

i32 __stdcall DllMain (HMODULE mod, DWORD cause, void *ctx) {
	if (cause == DLL_PROCESS_DETACH)
		DisposePoll ();
	if (cause != DLL_PROCESS_ATTACH)
		return 1;

	// Blatantly stolen patches from mon.im
	WRITE_MEMORY ((void *)0x140000C00 + 0x22DC0, u8,
				  0xC3); // Actually get shit working
	WRITE_MEMORY ((void *)0x140000C00 + 0x31428D, u8, 0xB0,
				  0x01);									 // Unlock songs
	WRITE_MEMORY ((void *)0x140000C00 + 0x692217, u8, 0xEB); // Shared audio
	WRITE_MEMORY ((void *)0x140000C00 + 0x516739, u8, 0xBA, 0x00, 0x00, 0x00,
				  0x00, 0x90); // Disable VSync

	INSTALL_HOOK_DYNAMIC (ShowMouse,
						  PROC_ADDRESS ("user32.dll", "ShowCursor"));

	INSTALL_HOOK_DYNAMIC (XinputGetState,
						  PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetState"));
	INSTALL_HOOK_DYNAMIC (XinputSetState,
						  PROC_ADDRESS ("xinput9_1_0.dll", "XInputSetState"));
	INSTALL_HOOK_DYNAMIC (
		XinputGetCapabilites,
		PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetCapabilities"));

	INSTALL_HOOK_DYNAMIC (ClearSram,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_ClearSram"));
	INSTALL_HOOK_DYNAMIC (Close, PROC_ADDRESS ("bnusio.dll", "bnusio_Close"));
	INSTALL_HOOK_DYNAMIC (Communication,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_Communication"));
	INSTALL_HOOK_DYNAMIC (DecCoin,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_DecCoin"));
	INSTALL_HOOK_DYNAMIC (DecService,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_DecService"));
	INSTALL_HOOK_DYNAMIC (GetAnalogIn,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetAnalogIn"));
	INSTALL_HOOK_DYNAMIC (GetBuffer,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetBuffer"));
	INSTALL_HOOK_DYNAMIC (GetCDOut,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetCDOut"));
	INSTALL_HOOK_DYNAMIC (GetCoin,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetCoin"));
	INSTALL_HOOK_DYNAMIC (GetCoinError,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetCoinError"));
	INSTALL_HOOK_DYNAMIC (GetCoinLock,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetCoinLock"));
	INSTALL_HOOK_DYNAMIC (GetEncoder,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetEncoder"));
	INSTALL_HOOK_DYNAMIC (
		GetExpansionMode,
		PROC_ADDRESS ("bnusio.dll", "bnusio_GetExpansionMode"));
	INSTALL_HOOK_DYNAMIC (
		GetFirmwareVersion,
		PROC_ADDRESS ("bnusio.dll", "bnusio_GetFirmwareVersion"));
	INSTALL_HOOK_DYNAMIC (GetGout,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetGout"));
	INSTALL_HOOK_DYNAMIC (GetHopOut,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetHopOut"));
	INSTALL_HOOK_DYNAMIC (
		GetIoBoardName, PROC_ADDRESS ("bnusio.dll", "bnusio_GetIoBoardName"));
	INSTALL_HOOK_DYNAMIC (
		GetRegisterU16, PROC_ADDRESS ("bnusio.dll", "bnusio_GetRegisterU16"));
	INSTALL_HOOK_DYNAMIC (GetRegisterU8,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetRegisterU8"));
	INSTALL_HOOK_DYNAMIC (GetService,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetService"));
	INSTALL_HOOK_DYNAMIC (
		GetServiceError,
		PROC_ADDRESS ("bnusio.dll", "bnusio_GetServiceError"));
	INSTALL_HOOK_DYNAMIC (GetStatusU16,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetStatusU16"));
	INSTALL_HOOK_DYNAMIC (GetStatusU8,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetStatusU8"));
	INSTALL_HOOK_DYNAMIC (GetSwIn,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetSwIn"));
	INSTALL_HOOK_DYNAMIC (GetSwIn64,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_GetSwIn64"));
	INSTALL_HOOK_DYNAMIC (
		GetSystemError, PROC_ADDRESS ("bnusio.dll", "bnusio_GetSystemError"));
	INSTALL_HOOK_DYNAMIC (IsConnected,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_IsConnected"));
	INSTALL_HOOK_DYNAMIC (IsWideUsio,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_IsWideUsio"));
	INSTALL_HOOK_DYNAMIC (Open, PROC_ADDRESS ("bnusio.dll", "bnusio_Open"));
	INSTALL_HOOK_DYNAMIC (ResetCoin,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_ResetCoin"));
	INSTALL_HOOK_DYNAMIC (ResetIoBoard,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_ResetIoBoard"));
	INSTALL_HOOK_DYNAMIC (SetBuffer,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SetBuffer"));
	INSTALL_HOOK_DYNAMIC (SetCDOut,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SetCDOut"));
	INSTALL_HOOK_DYNAMIC (SetCoinLock,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SetCoinLock"));
	INSTALL_HOOK_DYNAMIC (
		SetExpansionMode,
		PROC_ADDRESS ("bnusio.dll", "bnusio_SetExpansionMode"));
	INSTALL_HOOK_DYNAMIC (SetGout,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SetGout"));
	INSTALL_HOOK_DYNAMIC (SetHopOut,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SetHopOut"));
	INSTALL_HOOK_DYNAMIC (
		SetHopperLimit, PROC_ADDRESS ("bnusio.dll", "bnusio_SetHopperLimit"));
	INSTALL_HOOK_DYNAMIC (
		SetHopperRequest,
		PROC_ADDRESS ("bnusio.dll", "bnusio_SetHopperRequest"));
	INSTALL_HOOK_DYNAMIC (SetPLCounter,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SetPLCounter"));
	INSTALL_HOOK_DYNAMIC (
		SetRegisterU16, PROC_ADDRESS ("bnusio.dll", "bnusio_SetRegisterU16"));
	INSTALL_HOOK_DYNAMIC (SetRegisterU8,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SetRegisterU8"));
	INSTALL_HOOK_DYNAMIC (
		SetSystemError, PROC_ADDRESS ("bnusio.dll", "bnusio_SetSystemError"));
	INSTALL_HOOK_DYNAMIC (SramRead,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SramRead"));
	INSTALL_HOOK_DYNAMIC (SramWrite,
						  PROC_ADDRESS ("bnusio.dll", "bnusio_SramWrite"));

	INSTALL_HOOK_DYNAMIC (
		UsbFinderInitialize,
		PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderInitialize"));
	INSTALL_HOOK_DYNAMIC (
		UsbFinderRelease,
		PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderRelease"));
	INSTALL_HOOK_DYNAMIC (UsbFinderGetSerialNumber,
						  PROC_ADDRESS ("nbamUsbFinderGetSerialNumber.dll",
										"nbamUsbFinderRelease"));

	return true;
}