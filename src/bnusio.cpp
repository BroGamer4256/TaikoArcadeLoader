#include "helpers.h"
#include "poll.h"
#include "patches/patches.h"
#include "keyconfig.h"
#include "constants.h"

extern std::vector<HMODULE> plugins;
extern char accessCode1[21];
extern char chipId1[33];
extern char accessCode2[21];
extern char chipId2[33];
extern GameVersion version;

typedef i32 (*callbackAttach) (i32, i32, i32 *);
typedef void (*callbackTouch) (i32, i32, u8[168], u64);
typedef void event ();
typedef void waitTouchEvent (callbackTouch, u64);
bool waitingForTouch = false;
callbackTouch touchCallback;
u64 touchData;
callbackAttach attachCallback;
i32 *attachData;

namespace bnusio {
#define RETURN_FALSE(returnType, functionName, ...) \
	returnType functionName (__VA_ARGS__) { return 0; }

extern "C" {
RETURN_FALSE (i64, bnusio_ClearSram);
RETURN_FALSE (i64, bnusio_Communication, i32 a1);
RETURN_FALSE (i64, bnusio_DecService, i32 a1, u16 a2);
RETURN_FALSE (void *, bnusio_GetBuffer, u16 a1, i64 a2, i16 a3);
RETURN_FALSE (i64, bnusio_GetCDOut, u8 a1);
RETURN_FALSE (void *, bnusio_GetCoinError, i32 a1);
RETURN_FALSE (i64, bnusio_GetCoinLock, u8 a1);
RETURN_FALSE (u64, bnusio_GetEncoder);
RETURN_FALSE (void *, bnusio_GetExpansionMode);
RETURN_FALSE (u8, bnusio_GetGout, u8 a1);
RETURN_FALSE (i64, bnusio_GetHopOut, u8 a1);
RETURN_FALSE (char *, bnusio_GetIoBoardName);
RETURN_FALSE (u16, bnusio_GetRegisterU16, i16 a1);
RETURN_FALSE (u8, bnusio_GetRegisterU8, u16 a1);
RETURN_FALSE (void *, bnusio_GetService, i32 a1);
RETURN_FALSE (void *, bnusio_GetServiceError, i32 a1);
RETURN_FALSE (u16, bnusio_GetStatusU16, u16 a1);
RETURN_FALSE (u8, bnusio_GetStatusU8, u16 a1);
RETURN_FALSE (u64, bnusio_GetSwIn64);
RETURN_FALSE (void *, bnusio_GetSystemError);
RETURN_FALSE (u8, bnusio_IsConnected);
RETURN_FALSE (u8, bnusio_IsWideUsio);
RETURN_FALSE (i64, bnusio_Open);
RETURN_FALSE (i32, bnusio_ResetIoBoard);
RETURN_FALSE (i64, bnusio_SetBuffer, u16 a1, i32 a2, i16 a3);
RETURN_FALSE (i64, bnusio_SetCDOut, u8 a1, u8 a2);
RETURN_FALSE (i64, bnusio_SetCoinLock, u8 a1, u8 a2);
RETURN_FALSE (i64, bnusio_SetExpansionMode, i16 a1);
RETURN_FALSE (i64, bnusio_SetGout, u8 a1, u8 a2);
RETURN_FALSE (i64, bnusio_SetHopOut, u8 a1, u8 a2);
RETURN_FALSE (i64, bnusio_SetHopperLimit, u16 a1, i16 a2);
RETURN_FALSE (i64, bnusio_SetHopperRequest, u16 a1, i16 a2);
RETURN_FALSE (void *, bnusio_SetPLCounter, i16 a1);
RETURN_FALSE (i64, bnusio_SetRegisterU16, u16 a1, u16 a2);
RETURN_FALSE (i64, bnusio_SetRegisterU8, u16 a1, u8 a2);
RETURN_FALSE (i64, bnusio_SetSystemError, i16 a1);
RETURN_FALSE (i64, bnusio_SramRead, i32 a1, u8 a2, i32 a3, u16 a4);
RETURN_FALSE (i64, bnusio_SramWrite, i32 a1, u8 a2, i32 a3, u16 a4);
RETURN_FALSE (i64, bnusio_ResetCoin);
RETURN_FALSE (i64, bnusio_DecCoin, i32 a1, u16 a2);
size_t
bnusio_GetFirmwareVersion () {
	return 126;
}

extern Keybindings EXIT;
extern Keybindings TEST;
extern Keybindings SERVICE;
extern Keybindings DEBUG_UP;
extern Keybindings DEBUG_DOWN;
extern Keybindings DEBUG_ENTER;
extern Keybindings COIN_ADD;
extern Keybindings CARD_INSERT_1;
extern Keybindings CARD_INSERT_2;
extern Keybindings P1_LEFT_BLUE;
extern Keybindings P1_LEFT_RED;
extern Keybindings P1_RIGHT_RED;
extern Keybindings P1_RIGHT_BLUE;
extern Keybindings P2_LEFT_BLUE;
extern Keybindings P2_LEFT_RED;
extern Keybindings P2_RIGHT_RED;
extern Keybindings P2_RIGHT_BLUE;

u16 drumMin        = 10000;
u16 drumMax        = 20000;
u16 drumWaitPeriod = 4;

u16 lastHitValue             = drumMin;
Keybindings *analogButtons[] = {&P1_LEFT_BLUE, &P1_LEFT_RED, &P1_RIGHT_RED, &P1_RIGHT_BLUE, &P2_LEFT_BLUE, &P2_LEFT_RED, &P2_RIGHT_RED, &P2_RIGHT_BLUE};

u16 buttonWaitPeriodP1 = 0;
u16 buttonWaitPeriodP2 = 0;
std::queue<u8> buttonQueueP1;
std::queue<u8> buttonQueueP2;

u16
bnusio_GetAnalogIn (u8 which) {
	auto button = analogButtons[which];
	if (which == 0) {
		if (buttonWaitPeriodP1 > 0) buttonWaitPeriodP1--;
		if (buttonWaitPeriodP2 > 0) buttonWaitPeriodP2--;
	}
	bool isP1 = which / 4 == 0;
	if ((isP1 && !buttonQueueP1.empty ()) || (!isP1 && !buttonQueueP2.empty ())) {
		if ((isP1 && buttonQueueP1.front () == which && buttonWaitPeriodP1 == 0) || (!isP1 && buttonQueueP2.front () == which && buttonWaitPeriodP2 == 0)) {
			if (isP1) {
				buttonQueueP1.pop ();
				buttonWaitPeriodP1 = drumWaitPeriod;
			} else {
				buttonQueueP2.pop ();
				buttonWaitPeriodP2 = drumWaitPeriod;
			}

			lastHitValue++;
			if (lastHitValue >= drumMax) lastHitValue = drumMin;
			return lastHitValue;
		}
		if (IsButtonTapped (*button)) {
			if (isP1) buttonQueueP1.push (which);
			else buttonQueueP2.push (which);
		}
		return 0;
	} else if (IsButtonTapped (*button)) {
		if (isP1 && buttonWaitPeriodP1 > 0) {
			buttonQueueP1.push (which);
			return 0;
		} else if (!isP1 && buttonWaitPeriodP2 > 0) {
			buttonQueueP2.push (which);
			return 0;
		}
		if (isP1) buttonWaitPeriodP1 = drumWaitPeriod;
		else buttonWaitPeriodP2 = drumWaitPeriod;
		lastHitValue++;
		if (lastHitValue >= drumMax) lastHitValue = drumMin;
		return lastHitValue;
	} else {
		return 0;
	}
}

bool testEnabled  = false;
int coin_count    = 0;
bool inited       = false;
HWND windowHandle = 0;

u16 __fastcall bnusio_GetCoin (i32 a1) {
	if (a1 != 1) return coin_count;

	if (!inited) {
		windowHandle = FindWindowA ("nuFoundation.Window", 0);
		InitializePoll (windowHandle);

		for (auto plugin : plugins) {
			auto initEvent = GetProcAddress (plugin, "Init");
			if (initEvent) initEvent ();
		}

		inited = true;
	}

	UpdatePoll (windowHandle);
	if (IsButtonTapped (COIN_ADD) && !testEnabled) coin_count++;
	if (IsButtonTapped (TEST)) testEnabled = !testEnabled;
	if (IsButtonTapped (EXIT)) ExitProcess (0);
	if (waitingForTouch) {
		static u8 cardData[168]
		    = { 0x01, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0x2E, 0x58, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x5C, 0x97, 0x44, 0xF0, 0x88, 0x04, 0x00, 0x43, 0x26, 0x2C, 0x33, 0x00, 0x04,
			    0x06, 0x10, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
			    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30,
			    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x42, 0x47, 0x49, 0x43, 0x36,
			    0x00, 0x00, 0xFA, 0xE9, 0x69, 0x00, 0xF6, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		bool hasInserted = false;
		if (IsButtonTapped (CARD_INSERT_1)) {
			for (auto plugin : plugins) {
				FARPROC insertEvent = GetProcAddress (plugin, "BeforeCard1Insert");
				if (insertEvent) ((event *)insertEvent) ();
			}
			for (auto plugin : plugins) {
				FARPROC insertEvent = GetProcAddress (plugin, "Card1Insert");
				if (insertEvent) {
					((event *)insertEvent) ();
					hasInserted = true;
				}
			}
			if (!hasInserted) {
				memcpy (cardData + 0x2C, chipId1, 33);
				memcpy (cardData + 0x50, accessCode1, 21);
				touchCallback (0, 0, cardData, touchData);
			}
		} else if (IsButtonTapped (CARD_INSERT_2)) {
			for (auto plugin : plugins) {
				FARPROC insertEvent = GetProcAddress (plugin, "BeforeCard2Insert");
				if (insertEvent) ((event *)insertEvent) ();
			}
			for (auto plugin : plugins) {
				FARPROC insertEvent = GetProcAddress (plugin, "Card2Insert");
				if (insertEvent) {
					((event *)insertEvent) ();
					hasInserted = true;
				}
			}
			if (!hasInserted) {
				memcpy (cardData + 0x2C, chipId2, 33);
				memcpy (cardData + 0x50, accessCode2, 21);
				touchCallback (0, 0, cardData, touchData);
			}
		}
	}

	for (auto plugin : plugins) {
		auto updateEvent = GetProcAddress (plugin, "Update");
		if (updateEvent) updateEvent ();
	}

	patches::Qr::Update();

	return coin_count;
}

u32
bnusio_GetSwIn () {
	u32 sw = 0;
	sw |= (u32)testEnabled << 7;
	sw |= (u32)IsButtonDown (DEBUG_ENTER) << 9;
	sw |= (u32)IsButtonDown (DEBUG_DOWN) << 12;
	sw |= (u32)IsButtonDown (DEBUG_UP) << 13;
	sw |= (u32)IsButtonDown (SERVICE) << 14;
	return sw;
}

i64
bnusio_Close () {
	for (auto plugin : plugins) {
		FARPROC exitEvent = GetProcAddress (plugin, "Exit");
		if (exitEvent) ((event *)exitEvent) ();
	}
	return 0;
}
}

HOOK (u64, bngrw_DevReset, PROC_ADDRESS ("bngrw.dll", "BngRwDevReset")) { return 1; }
HOOK (u64, bngrw_ReadMifare, PROC_ADDRESS ("bngrw.dll", "BngRwExReadMifareAllBlock")) { return 0xFFFFFF9C; }
HOOK (void, bngrw_fin, PROC_ADDRESS ("bngrw.dll", "BngRwFin")) { return; }
HOOK (u64, bngrw_GetFwVersion, PROC_ADDRESS ("bngrw.dll", "BngRwGetFwVersion")) { return 0; }
HOOK (u64, bngrw_GetStationID, PROC_ADDRESS ("bngrw.dll", "BngRwGetStationID")) { return 0; }
HOOK (u64, bngrw_GetRetryCount, PROC_ADDRESS ("bngrw.dll", "BngRwGetTotalRetryCount")) { return 0; }
HOOK (u64, bngrw_IsCmdExec, PROC_ADDRESS ("bngrw.dll", "BngRwIsCmdExec")) { return 0xFFFFFFFF; }
HOOK (u64, bngrw_ReqAction, PROC_ADDRESS ("bngrw.dll", "BngRwReqAction")) { return 1; }
HOOK (u64, bngrw_ReqAiccAuth, PROC_ADDRESS ("bngrw.dll", "BngRwReqAiccAuth")) { return 1; }
HOOK (u64, bngrw_ReqBeep, PROC_ADDRESS ("bngrw.dll", "BngRwReqBeep")) { return 1; }
HOOK (u64, bngrw_ReqFwCleanup, PROC_ADDRESS ("bngrw.dll", "BngRwReqFwCleanup")) { return 1; }
HOOK (u64, bngrw_ReqFwVersionUp, PROC_ADDRESS ("bngrw.dll", "BngRwReqFwVersionUp")) { return 1; }
HOOK (i32, bngrw_ReqLatchID, PROC_ADDRESS ("bngrw.dll", "BngRwReqLatchID")) { return 1; }
HOOK (u64, bngrw_ReqLed, PROC_ADDRESS ("bngrw.dll", "BngRwReqLed")) { return 1; }
HOOK (i32, bngrw_ReqSendMail, PROC_ADDRESS ("bngrw.dll", "BngRwReqSendMailTo")) { return 1; }
HOOK (i32, bngrw_ReqSendUrl, PROC_ADDRESS ("bngrw.dll", "BngRwReqSendUrlTo")) { return 1; }
HOOK (u64, bngrw_ReqSetLedPower, PROC_ADDRESS ("bngrw.dll", "BngRwReqSetLedPower")) { return 0; }
HOOK (i32, bngrw_reqCancel, PROC_ADDRESS ("bngrw.dll", "BngRwReqCancel")) { return 1; }
HOOK (u64, bngrw_Init, PROC_ADDRESS ("bngrw.dll", "BngRwInit")) { return 0; }
HOOK (u64, bngrw_attach, PROC_ADDRESS ("bngrw.dll", "BngRwAttach"), i32 a1, char *a2, i32 a3, i32 a4, i32 (*callback) (i32, i32, i32 *), i32 *a6) {
	// This is way too fucking jank
	attachCallback = callback;
	attachData     = a6;
	return 1;
}
HOOK (u64, bngrw_reqWaitTouch, PROC_ADDRESS ("bngrw.dll", "BngRwReqWaitTouch"), u32 a1, i32 a2, u32 a3, void (*callback) (i32, i32, u8[168], u64), u64 a5) {
	waitingForTouch = true;
	touchCallback   = callback;
	touchData       = a5;
	for (auto plugin : plugins) {
		FARPROC touchEvent = GetProcAddress (plugin, "WaitTouch");
		if (touchEvent) ((waitTouchEvent *)touchEvent) (callback, a5);
	}
	return 1;
}

void
Init () {
	INSTALL_HOOK (bngrw_DevReset);
	INSTALL_HOOK (bngrw_ReadMifare);
	INSTALL_HOOK (bngrw_fin);
	INSTALL_HOOK (bngrw_GetFwVersion);
	INSTALL_HOOK (bngrw_GetStationID);
	INSTALL_HOOK (bngrw_GetRetryCount);
	INSTALL_HOOK (bngrw_IsCmdExec);
	INSTALL_HOOK (bngrw_ReqAction);
	INSTALL_HOOK (bngrw_ReqAiccAuth);
	INSTALL_HOOK (bngrw_ReqBeep);
	INSTALL_HOOK (bngrw_ReqFwCleanup);
	INSTALL_HOOK (bngrw_ReqFwVersionUp);
	INSTALL_HOOK (bngrw_ReqLatchID);
	INSTALL_HOOK (bngrw_ReqLed);
	INSTALL_HOOK (bngrw_ReqSendMail);
	INSTALL_HOOK (bngrw_ReqSendUrl);
	INSTALL_HOOK (bngrw_ReqSetLedPower);
	INSTALL_HOOK (bngrw_reqCancel);
	INSTALL_HOOK (bngrw_Init)
	INSTALL_HOOK (bngrw_attach);
	INSTALL_HOOK (bngrw_reqWaitTouch);

	KeyConfig::Init();
}
} // namespace bnusio
