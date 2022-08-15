#include "helpers.h"

// force show cursor
HOOK_DYNAMIC (i32, __stdcall, ShowMouse, i32 show) { return originalShowMouse (true); }

// xinput stuff
HOOK_DYNAMIC (u32, __stdcall, XinputGetState, u32 index, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }
HOOK_DYNAMIC (u32, __stdcall, XinputSetState, u32 index, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }
HOOK_DYNAMIC (u32, __stdcall, XinputGetCapabilites, u32 index, u32 flags, void *state) { return ERROR_DEVICE_NOT_CONNECTED; }

// bnusio stuff
RETURN_FALSE (i64, __stdcall, bnusio_ClearSram);
RETURN_FALSE (i64, __stdcall, bnusio_Close);
RETURN_FALSE (i64, __fastcall, bnusio_Communication, i32 a1);
RETURN_FALSE (i64, __fastcall, bnusio_DecService, i32 a1, u16 a2);
RETURN_FALSE (void *, __fastcall, bnusio_GetBuffer, u16 a1, i64 a2, i16 a3);
RETURN_FALSE (i64, __fastcall, bnusio_GetCDOut, u8 a1);
RETURN_FALSE (void *, __fastcall, bnusio_GetCoinError, i32 a1);
RETURN_FALSE (i64, __fastcall, bnusio_GetCoinLock, u8 a1);
RETURN_FALSE (u64, __stdcall, bnusio_GetEncoder);
RETURN_FALSE (void *, __stdcall, bnusio_GetExpansionMode);
void *__stdcall bnusio_GetFirmwareVersion () { return (void *)(u16)126; }
RETURN_FALSE (u8, __stdcall, bnusio_GetGout, u8 a1);
RETURN_FALSE (i64, __stdcall, bnusio_GetHopOut, u8 a1);
RETURN_FALSE (char *, __stdcall, bnusio_GetIoBoardName);
RETURN_FALSE (u16, __fastcall, bnusio_GetRegisterU16, i16 a1);
RETURN_FALSE (u8, __fastcall, bnusio_GetRegisterU8, u16 a1);
RETURN_FALSE (void *, __fastcall, bnusio_GetService, i32 a1);
RETURN_FALSE (void *, __fastcall, bnusio_GetServiceError, i32 a1);
RETURN_FALSE (u16, __fastcall, bnusio_GetStatusU16, u16 a1);
RETURN_FALSE (u8, __fastcall, bnusio_GetStatusU8, u16 a1);
RETURN_FALSE (u64, __stdcall, bnusio_GetSwIn64);
RETURN_FALSE (void *, __stdcall, bnusio_GetSystemError);
RETURN_FALSE (u8, __stdcall, bnusio_IsConnected);
RETURN_FALSE (u8, __stdcall, bnusio_IsWideUsio);
RETURN_FALSE (i64, __stdcall, bnusio_Open);
RETURN_FALSE (i32, __stdcall, bnusio_ResetIoBoard);
RETURN_FALSE (i64, __fastcall, bnusio_SetBuffer, u16 a1, i32 a2, i16 a3);
RETURN_FALSE (i64, __fastcall, bnusio_SetCDOut, u8 a1, u8 a2);
RETURN_FALSE (i64, __fastcall, bnusio_SetCoinLock, u8 a1, u8 a2);
RETURN_FALSE (i64, __fastcall, bnusio_SetExpansionMode, i16 a1);
RETURN_FALSE (i64, __fastcall, bnusio_SetGout, u8 a1, u8 a2);
RETURN_FALSE (i64, __fastcall, bnusio_SetHopOut, u8 a1, u8 a2);
RETURN_FALSE (i64, __fastcall, bnusio_SetHopperLimit, u16 a1, i16 a2);
RETURN_FALSE (i64, __fastcall, bnusio_SetHopperRequest, u16 a1, i16 a2);
RETURN_FALSE (void *, __fastcall, bnusio_SetPLCounter, i16 a1);
RETURN_FALSE (i64, __fastcall, bnusio_SetRegisterU16, u16 a1, u16 a2);
RETURN_FALSE (i64, __fastcall, bnusio_SetRegisterU8, u16 a1, u8 a2);
RETURN_FALSE (i64, __fastcall, bnusio_SetSystemError, i16 a1);
RETURN_FALSE (i64, __fastcall, bnusio_SramRead, i32 a1, u8 a2, i32 a3, u16 a4);
RETURN_FALSE (i64, __fastcall, bnusio_SramWrite, i32 a1, u8 a2, i32 a3, u16 a4);
RETURN_FALSE (i64, __stdcall, bnusio_ResetCoin);
RETURN_FALSE (i64, __fastcall, bnusio_DecCoin, i32 a1, u16 a2);
HOOK_DYNAMIC (i64, __stdcall, UsbFinderInitialize) { return 0; }
HOOK_DYNAMIC (i64, __stdcall, UsbFinderRelease) { return 0; }
HOOK_DYNAMIC (i64, __fastcall, UsbFinderGetSerialNumber, i32 a1, char *a2) {
	strcpy (a2, "284111080001");
	return 0;
}
HOOK_DYNAMIC (u64, __stdcall, bngrw_DevReset) { return 1; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_ReadMifare) { return 0xffffff9c; }
HOOK_DYNAMIC (void, __stdcall, bngrw_fin) { return; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_GetFwVersion) { return 0; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_GetStationID) { return 0; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_GetRetryCount) { return 0; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_Init) { return 0; }
HOOK_DYNAMIC (i32, __stdcall, bngrw_IsCmdExec) { return -1; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_ReqAction) { return 1; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_ReqAiccAuth) { return 1; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_ReqBeep) { return 1; }
HOOK_DYNAMIC (i32, __stdcall, bngrw_ReqCancel, u32 a1) { return a1 > 7 ? -100 : 1; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_ReqFwCleanup) { return 1; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_ReqFwVersionUp) { return 1; }
HOOK_DYNAMIC (i32, __stdcall, bngrw_ReqLatchID, u32 a1) { return a1 < 8 ? -100 : 1; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_ReqLed) { return 1; }
HOOK_DYNAMIC (i32, __stdcall, bngrw_ReqSendMail, u32 a1, u32 a2, u32 a3, u64 a4, u64 a5) { return a1 < 8 && a5 ? 1 : -100; }
HOOK_DYNAMIC (i32, __stdcall, bngrw_ReqSendUrl, u32 a1, u32 a2, u32 a3, u64 a4, u64 a5) { return a1 < 8 && a5 ? 1 : -100; }
HOOK_DYNAMIC (u64, __stdcall, bngrw_ReqSetLedPower) { return 0; }

void
init_boilerplate () {
	INSTALL_HOOK_DYNAMIC (ShowMouse, PROC_ADDRESS ("user32.dll", "ShowCursor"));
	INSTALL_HOOK_DYNAMIC (XinputGetState, PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetState"));
	INSTALL_HOOK_DYNAMIC (XinputSetState, PROC_ADDRESS ("xinput9_1_0.dll", "XInputSetState"));
	INSTALL_HOOK_DYNAMIC (XinputGetCapabilites, PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetCapabilities"));

	INSTALL_HOOK_DYNAMIC (UsbFinderInitialize, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderInitialize"));
	INSTALL_HOOK_DYNAMIC (UsbFinderRelease, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderRelease"));
	INSTALL_HOOK_DYNAMIC (UsbFinderGetSerialNumber, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderGetSerialNumber"));

	INSTALL_HOOK_DYNAMIC (bngrw_DevReset, PROC_ADDRESS ("bngrw.dll", "BngRwDevReset"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReadMifare, PROC_ADDRESS ("bngrw.dll", "BngRwReadMifareAllBlock"));
	INSTALL_HOOK_DYNAMIC (bngrw_fin, PROC_ADDRESS ("bngrw.dll", "BngRwFin"));
	INSTALL_HOOK_DYNAMIC (bngrw_GetFwVersion, PROC_ADDRESS ("bngrw.dll", "BngRwGetFwVersion"));
	INSTALL_HOOK_DYNAMIC (bngrw_GetStationID, PROC_ADDRESS ("bngrw.dll", "BngRwGetStationID"));
	INSTALL_HOOK_DYNAMIC (bngrw_GetRetryCount, PROC_ADDRESS ("bngrw.dll", "BngRwGetTotalRetryCount"));
	INSTALL_HOOK_DYNAMIC (bngrw_Init, PROC_ADDRESS ("bngrw.dll", "BngRwInit"));
	INSTALL_HOOK_DYNAMIC (bngrw_IsCmdExec, PROC_ADDRESS ("bngrw.dll", "BngRwIsCmdExec"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqAction, PROC_ADDRESS ("bngrw.dll", "BngRwReqAction"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqAiccAuth, PROC_ADDRESS ("bngrw.dll", "BngRwReqAiccAuth"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqBeep, PROC_ADDRESS ("bngrw.dll", "BngRwReqBeep"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqCancel, PROC_ADDRESS ("bngrw.dll", "BngRwReqCancel"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqFwCleanup, PROC_ADDRESS ("bngrw.dll", "BngRwReqFwCleanup"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqFwVersionUp, PROC_ADDRESS ("bngrw.dll", "BngRwReqFwVersionUp"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqLatchID, PROC_ADDRESS ("bngrw.dll", "BngRwReqLatchID"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqLed, PROC_ADDRESS ("bngrw.dll", "BngRwReqLed"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqSendMail, PROC_ADDRESS ("bngrw.dll", "BngRwReqSendMailTo"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqSendUrl, PROC_ADDRESS ("bngrw.dll", "BngRwReqSendUrlTo"));
	INSTALL_HOOK_DYNAMIC (bngrw_ReqSetLedPower, PROC_ADDRESS ("bngrw.dll", "BngRwReqSetLedPower"));
}