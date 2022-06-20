#include "helpers.h"

// xinput stuff
HOOK_DYNAMIC (u32, __stdcall, XinputGetState, u32 index, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }
HOOK_DYNAMIC (u32, __stdcall, XinputSetState, u32 index, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }
HOOK_DYNAMIC (u32, __stdcall, XinputGetCapabilites, u32 index, u32 flags, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }

// bnusio stuff
HOOK_DYNAMIC (i64, __stdcall, ClearSram) { return 0; }
HOOK_DYNAMIC (i64, __stdcall, Close) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, Communication, i32 a1) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, DecService, i32 a1, u16 a2) { return 0; }
HOOK_DYNAMIC (void *, __fastcall, GetBuffer, u16 a1, i64 a2, i16 a3) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, GetCDOut, u8 a1) { return 0; }
HOOK_DYNAMIC (void *, __fastcall, GetCoinError, i32 a1) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, GetCoinLock, u8 a1) { return 0; }
HOOK_DYNAMIC (u64, __stdcall, GetEncoder) { return 0; }
HOOK_DYNAMIC (void *, __stdcall, GetExpansionMode) { return 0; }
HOOK_DYNAMIC (void *, __stdcall, GetFirmwareVersion) { return (void *)(u16)126; }
HOOK_DYNAMIC (u8, __stdcall, GetGout, u8 a1) { return 0; }
HOOK_DYNAMIC (i64, __stdcall, GetHopOut, u8 a1) { return 0; }
HOOK_DYNAMIC (char *, __stdcall, GetIoBoardName) { return 0; }
HOOK_DYNAMIC (u16, __fastcall, GetRegisterU16, i16 a1) { return 0; }
HOOK_DYNAMIC (u8, __fastcall, GetRegisterU8, u16 a1) { return 0; }
HOOK_DYNAMIC (void *, __fastcall, GetService, i32 a1) { return 0; }
HOOK_DYNAMIC (void *, __fastcall, GetServiceError, i32 a1) { return 0; }
HOOK_DYNAMIC (u16, __fastcall, GetStatusU16, u16 a1) { return 0; }
HOOK_DYNAMIC (u8, __fastcall, GetStatusU8, u16 a1) { return 0; }
HOOK_DYNAMIC (u64, __stdcall, GetSwIn64) { return 0; }
HOOK_DYNAMIC (void *, __stdcall, GetSystemError) { return 0; }
HOOK_DYNAMIC (u8, __stdcall, IsConnected) { return 0; }
HOOK_DYNAMIC (u8, __stdcall, IsWideUsio) { return 0; }
HOOK_DYNAMIC (i64, __stdcall, Open) { return 0; }
HOOK_DYNAMIC (i32, __stdcall, ResetIoBoard) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetBuffer, u16 a1, i32 a2, i16 a3) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetCDOut, u8 a1, u8 a2) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetCoinLock, u8 a1, u8 a2) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetExpansionMode, i16 a1) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetGout, u8 a1, u8 a2) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetHopOut, u8 a1, u8 a2) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetHopperLimit, u16 a1, i16 a2) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetHopperRequest, u16 a1, i16 a2) { return 0; }
HOOK_DYNAMIC (void *, __fastcall, SetPLCounter, i16 a1) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetRegisterU16, u16 a1, u16 a2) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetRegisterU8, u16 a1, u8 a2) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SetSystemError, i16 a1) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SramRead, i32 a1, u8 a2, i32 a3, u16 a4) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, SramWrite, i32 a1, u8 a2, i32 a3, u16 a4) { return 0; }
HOOK_DYNAMIC (i64, __stdcall, UsbFinderInitialize) { return 0; }
HOOK_DYNAMIC (i64, __stdcall, UsbFinderRelease) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, UsbFinderGetSerialNumber, i32 a1, i64 a2) { return 1; }

void
init_boilerplate () {
	INSTALL_HOOK_DYNAMIC (XinputGetState, PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetState"));
	INSTALL_HOOK_DYNAMIC (XinputSetState, PROC_ADDRESS ("xinput9_1_0.dll", "XInputSetState"));
	INSTALL_HOOK_DYNAMIC (XinputGetCapabilites, PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetCapabilities"));

	INSTALL_HOOK_DYNAMIC (ClearSram, PROC_ADDRESS ("bnusio.dll", "bnusio_ClearSram"));
	INSTALL_HOOK_DYNAMIC (Close, PROC_ADDRESS ("bnusio.dll", "bnusio_Close"));
	INSTALL_HOOK_DYNAMIC (Communication, PROC_ADDRESS ("bnusio.dll", "bnusio_Communication"));
	INSTALL_HOOK_DYNAMIC (DecService, PROC_ADDRESS ("bnusio.dll", "bnusio_DecService"));
	INSTALL_HOOK_DYNAMIC (GetBuffer, PROC_ADDRESS ("bnusio.dll", "bnusio_GetBuffer"));
	INSTALL_HOOK_DYNAMIC (GetCDOut, PROC_ADDRESS ("bnusio.dll", "bnusio_GetCDOut"));
	INSTALL_HOOK_DYNAMIC (GetCoinError, PROC_ADDRESS ("bnusio.dll", "bnusio_GetCoinError"));
	INSTALL_HOOK_DYNAMIC (GetCoinLock, PROC_ADDRESS ("bnusio.dll", "bnusio_GetCoinLock"));
	INSTALL_HOOK_DYNAMIC (GetEncoder, PROC_ADDRESS ("bnusio.dll", "bnusio_GetEncoder"));
	INSTALL_HOOK_DYNAMIC (GetExpansionMode, PROC_ADDRESS ("bnusio.dll", "bnusio_GetExpansionMode"));
	INSTALL_HOOK_DYNAMIC (GetFirmwareVersion, PROC_ADDRESS ("bnusio.dll", "bnusio_GetFirmwareVersion"));
	INSTALL_HOOK_DYNAMIC (GetGout, PROC_ADDRESS ("bnusio.dll", "bnusio_GetGout"));
	INSTALL_HOOK_DYNAMIC (GetHopOut, PROC_ADDRESS ("bnusio.dll", "bnusio_GetHopOut"));
	INSTALL_HOOK_DYNAMIC (GetIoBoardName, PROC_ADDRESS ("bnusio.dll", "bnusio_GetIoBoardName"));
	INSTALL_HOOK_DYNAMIC (GetRegisterU16, PROC_ADDRESS ("bnusio.dll", "bnusio_GetRegisterU16"));
	INSTALL_HOOK_DYNAMIC (GetRegisterU8, PROC_ADDRESS ("bnusio.dll", "bnusio_GetRegisterU8"));
	INSTALL_HOOK_DYNAMIC (GetService, PROC_ADDRESS ("bnusio.dll", "bnusio_GetService"));
	INSTALL_HOOK_DYNAMIC (GetServiceError, PROC_ADDRESS ("bnusio.dll", "bnusio_GetServiceError"));
	INSTALL_HOOK_DYNAMIC (GetStatusU16, PROC_ADDRESS ("bnusio.dll", "bnusio_GetStatusU16"));
	INSTALL_HOOK_DYNAMIC (GetStatusU8, PROC_ADDRESS ("bnusio.dll", "bnusio_GetStatusU8"));
	INSTALL_HOOK_DYNAMIC (GetSwIn64, PROC_ADDRESS ("bnusio.dll", "bnusio_GetSwIn64"));
	INSTALL_HOOK_DYNAMIC (GetSystemError, PROC_ADDRESS ("bnusio.dll", "bnusio_GetSystemError"));
	INSTALL_HOOK_DYNAMIC (IsConnected, PROC_ADDRESS ("bnusio.dll", "bnusio_IsConnected"));
	INSTALL_HOOK_DYNAMIC (IsWideUsio, PROC_ADDRESS ("bnusio.dll", "bnusio_IsWideUsio"));
	INSTALL_HOOK_DYNAMIC (Open, PROC_ADDRESS ("bnusio.dll", "bnusio_Open"));
	INSTALL_HOOK_DYNAMIC (ResetIoBoard, PROC_ADDRESS ("bnusio.dll", "bnusio_ResetIoBoard"));
	INSTALL_HOOK_DYNAMIC (SetBuffer, PROC_ADDRESS ("bnusio.dll", "bnusio_SetBuffer"));
	INSTALL_HOOK_DYNAMIC (SetCDOut, PROC_ADDRESS ("bnusio.dll", "bnusio_SetCDOut"));
	INSTALL_HOOK_DYNAMIC (SetCoinLock, PROC_ADDRESS ("bnusio.dll", "bnusio_SetCoinLock"));
	INSTALL_HOOK_DYNAMIC (SetExpansionMode, PROC_ADDRESS ("bnusio.dll", "bnusio_SetExpansionMode"));
	INSTALL_HOOK_DYNAMIC (SetGout, PROC_ADDRESS ("bnusio.dll", "bnusio_SetGout"));
	INSTALL_HOOK_DYNAMIC (SetHopOut, PROC_ADDRESS ("bnusio.dll", "bnusio_SetHopOut"));
	INSTALL_HOOK_DYNAMIC (SetHopperLimit, PROC_ADDRESS ("bnusio.dll", "bnusio_SetHopperLimit"));
	INSTALL_HOOK_DYNAMIC (SetHopperRequest, PROC_ADDRESS ("bnusio.dll", "bnusio_SetHopperRequest"));
	INSTALL_HOOK_DYNAMIC (SetPLCounter, PROC_ADDRESS ("bnusio.dll", "bnusio_SetPLCounter"));
	INSTALL_HOOK_DYNAMIC (SetRegisterU16, PROC_ADDRESS ("bnusio.dll", "bnusio_SetRegisterU16"));
	INSTALL_HOOK_DYNAMIC (SetRegisterU8, PROC_ADDRESS ("bnusio.dll", "bnusio_SetRegisterU8"));
	INSTALL_HOOK_DYNAMIC (SetSystemError, PROC_ADDRESS ("bnusio.dll", "bnusio_SetSystemError"));
	INSTALL_HOOK_DYNAMIC (SramRead, PROC_ADDRESS ("bnusio.dll", "bnusio_SramRead"));
	INSTALL_HOOK_DYNAMIC (SramWrite, PROC_ADDRESS ("bnusio.dll", "bnusio_SramWrite"));

	INSTALL_HOOK_DYNAMIC (UsbFinderInitialize, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderInitialize"));
	INSTALL_HOOK_DYNAMIC (UsbFinderRelease, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderRelease"));
	INSTALL_HOOK_DYNAMIC (UsbFinderGetSerialNumber, PROC_ADDRESS ("nbamUsbFinderGetSerialNumber.dll", "nbamUsbFinderRelease"));
}