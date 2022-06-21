#include "helpers.h"

// xinput stuff
HOOK_DYNAMIC (u32, __stdcall, XinputGetState, u32 index, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }
HOOK_DYNAMIC (u32, __stdcall, XinputSetState, u32 index, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }
HOOK_DYNAMIC (u32, __stdcall, XinputGetCapabilites, u32 index, u32 flags, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }

// bnusio stuff
RETURN_FALSE (i64, __stdcall, bnusio_ClearSram)
RETURN_FALSE (i64, __stdcall, bnusio_Close)
RETURN_FALSE (i64, __fastcall, bnusio_Communication, i32 a1)
RETURN_FALSE (i64, __fastcall, bnusio_DecService, i32 a1, u16 a2)
RETURN_FALSE (void *, __fastcall, bnusio_GetBuffer, u16 a1, i64 a2, i16 a3)
RETURN_FALSE (i64, __fastcall, bnusio_GetCDOut, u8 a1)
RETURN_FALSE (void *, __fastcall, bnusio_GetCoinError, i32 a1)
RETURN_FALSE (i64, __fastcall, bnusio_GetCoinLock, u8 a1)
RETURN_FALSE (u64, __stdcall, bnusio_GetEncoder)
RETURN_FALSE (void *, __stdcall, bnusio_GetExpansionMode)
void *__stdcall bnusio_GetFirmwareVersion () { return (void *)(u16)126; }
RETURN_FALSE (u8, __stdcall, bnusio_GetGout, u8 a1)
RETURN_FALSE (i64, __stdcall, bnusio_GetHopOut, u8 a1)
RETURN_FALSE (char *, __stdcall, bnusio_GetIoBoardName)
RETURN_FALSE (u16, __fastcall, bnusio_GetRegisterU16, i16 a1)
RETURN_FALSE (u8, __fastcall, bnusio_GetRegisterU8, u16 a1)
RETURN_FALSE (void *, __fastcall, bnusio_GetService, i32 a1)
RETURN_FALSE (void *, __fastcall, bnusio_GetServiceError, i32 a1)
RETURN_FALSE (u16, __fastcall, bnusio_GetStatusU16, u16 a1)
RETURN_FALSE (u8, __fastcall, bnusio_GetStatusU8, u16 a1)
RETURN_FALSE (u64, __stdcall, bnusio_GetSwIn64)
RETURN_FALSE (void *, __stdcall, bnusio_GetSystemError)
RETURN_FALSE (u8, __stdcall, bnusio_IsConnected)
RETURN_FALSE (u8, __stdcall, bnusio_IsWideUsio)
RETURN_FALSE (i64, __stdcall, bnusio_Open)
RETURN_FALSE (i32, __stdcall, bnusio_ResetIoBoard)
RETURN_FALSE (i64, __fastcall, bnusio_SetBuffer, u16 a1, i32 a2, i16 a3)
RETURN_FALSE (i64, __fastcall, bnusio_SetCDOut, u8 a1, u8 a2)
RETURN_FALSE (i64, __fastcall, bnusio_SetCoinLock, u8 a1, u8 a2)
RETURN_FALSE (i64, __fastcall, bnusio_SetExpansionMode, i16 a1)
RETURN_FALSE (i64, __fastcall, bnusio_SetGout, u8 a1, u8 a2)
RETURN_FALSE (i64, __fastcall, bnusio_SetHopOut, u8 a1, u8 a2)
RETURN_FALSE (i64, __fastcall, bnusio_SetHopperLimit, u16 a1, i16 a2)
RETURN_FALSE (i64, __fastcall, bnusio_SetHopperRequest, u16 a1, i16 a2)
RETURN_FALSE (void *, __fastcall, bnusio_SetPLCounter, i16 a1)
RETURN_FALSE (i64, __fastcall, bnusio_SetRegisterU16, u16 a1, u16 a2)
RETURN_FALSE (i64, __fastcall, bnusio_SetRegisterU8, u16 a1, u8 a2)
RETURN_FALSE (i64, __fastcall, bnusio_SetSystemError, i16 a1)
RETURN_FALSE (i64, __fastcall, bnusio_SramRead, i32 a1, u8 a2, i32 a3, u16 a4)
RETURN_FALSE (i64, __fastcall, bnusio_SramWrite, i32 a1, u8 a2, i32 a3, u16 a4)
RETURN_FALSE (i64, __stdcall, bnusio_ResetCoin)
RETURN_FALSE (i64, __fastcall, bnusio_DecCoin, i32 a1, u16 a2)
HOOK_DYNAMIC (i64, __stdcall, UsbFinderInitialize) { return 0; }
HOOK_DYNAMIC (i64, __stdcall, UsbFinderRelease) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, UsbFinderGetSerialNumber, i32 a1, i64 a2) { return 1; }

void
init_boilerplate () {
	INSTALL_HOOK_DYNAMIC (XinputGetState, PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetState"));
	INSTALL_HOOK_DYNAMIC (XinputSetState, PROC_ADDRESS ("xinput9_1_0.dll", "XInputSetState"));
	INSTALL_HOOK_DYNAMIC (XinputGetCapabilites, PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetCapabilities"));

	INSTALL_HOOK_DYNAMIC (UsbFinderInitialize, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderInitialize"));
	INSTALL_HOOK_DYNAMIC (UsbFinderRelease, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderRelease"));
	INSTALL_HOOK_DYNAMIC (UsbFinderGetSerialNumber, PROC_ADDRESS ("nbamUsbFinderGetSerialNumber.dll", "nbamUsbFinderRelease"));
}