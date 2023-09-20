#include "constants.h"
#include "helpers.h"
#include "poll.h"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

extern GameVersion version;
extern Keybindings QR_CARD_READ;
extern Keybindings QR_DATA_READ;

namespace patches::Qr {

enum class State {
	Ready,
	CopyWait,
	AfterCopy1,
	AfterCopy2,
};

enum class Mode { Card, Data };

State gState = State::Ready;
Mode gMode   = Mode::Card;

HOOK_DYNAMIC (char, __fastcall, qrInit, int64_t a1) { return 1; }
HOOK_DYNAMIC (char, __fastcall, qrRead, int64_t a1) {
	*reinterpret_cast<DWORD *> (a1 + 40) = 1;
	*reinterpret_cast<DWORD *> (a1 + 16) = 1;
	*reinterpret_cast<BYTE *> (a1 + 112) = 0;
	return 1;
}
HOOK_DYNAMIC (char, __fastcall, qrClose, int64_t a1) { return 1; }
HOOK_DYNAMIC (int64_t, __fastcall, callQrUnknown, int64_t a1) {
	switch (gState) {
	case State::Ready:
	case State::CopyWait: {
		return 1;
	}
	case State::AfterCopy1: {
		gState = State::AfterCopy2;
		return 1;
	}
	case State::AfterCopy2: {
		return 1;
	}
	default: return 0;
	}
}
HOOK_DYNAMIC (bool, __fastcall, Send1, int64_t, const void *, int64_t) { return true; }
HOOK_DYNAMIC (bool, __fastcall, Send2, int64_t a1, char a2) { return true; }
HOOK_DYNAMIC (bool, __fastcall, Send3, int64_t a1) {
	*(WORD *)(a1 + 88) = 0;
	*(BYTE *)(a1 + 90) = 0;
	return true;
}
HOOK_DYNAMIC (bool, __fastcall, Send4, int64_t a1) {
	*(BYTE *)(a1 + 88)    = 1;
	*(int64_t *)(a1 + 32) = *(int64_t *)(a1 + 24);
	*(WORD *)(a1 + 89)    = 0;
	return true;
}
HOOK_DYNAMIC (int64_t, __fastcall, copy_data, int64_t this_, void *dest, int length) {
	if (gState == State::CopyWait) {
		std::cout << "Copy data, length: " << length << std::endl;
		auto configPath      = std::filesystem::current_path () / "config.toml";
		toml_table_t *config = openConfig (configPath);
		std::string data     = "";

		if (gMode == Mode::Card) {
			if (config) {
				data = readConfigString (config, "qr_card_string", "");
				toml_free (config);
			}

			memcpy (dest, data.c_str (), data.size () + 1);
			gState = State::AfterCopy1;
			return data.size () + 1;
		} else if (gMode == Mode::Data) {
			if (config) {
				data = readConfigString (config, "qr_data_string", "");
				toml_free (config);
			}

			BYTE data_length             = static_cast<BYTE> (data.size ());
			std::vector<BYTE> byteBuffer = {0x53, 0x31, 0x32, 0x00, 0x00, 0xFF, 0xFF, data_length, 0x01, 0x00};

			for (char c : data)
				byteBuffer.push_back (static_cast<BYTE> (c));

			byteBuffer.push_back (0xEE);
			byteBuffer.push_back (0xFF);

			memcpy (dest, byteBuffer.data (), byteBuffer.size ());
			gState = State::AfterCopy1;
			return byteBuffer.size ();
		}
	}
	return 0;
}

int gCount = 0;

void
Update () {
	if (gState == State::AfterCopy2) {
		gCount++;
		if (gCount > 10) {
			gCount = 0;
			gState = State::Ready;
		}
	}
	if (gState == State::Ready) {
		if (IsButtonTapped (QR_CARD_READ)) {
			std::cout << "Insert" << std::endl;
			gState = State::CopyWait;
			gMode  = Mode::Card;
		} else if (IsButtonTapped (QR_DATA_READ)) {
			std::cout << "Insert" << std::endl;
			gState = State::CopyWait;
			gMode  = Mode::Data;
		}
	}
}

void
Init () {
	auto amHandle = reinterpret_cast<uintptr_t> (GetModuleHandle ("AMFrameWork.dll"));
	switch (version) {
	case GameVersion::JP_NOV_2020: {
		INSTALL_HOOK_DYNAMIC (qrInit, reinterpret_cast<LPVOID> (amHandle + 0x1BA00));
		INSTALL_HOOK_DYNAMIC (qrRead, reinterpret_cast<LPVOID> (amHandle + 0x1BC20));
		INSTALL_HOOK_DYNAMIC (qrClose, reinterpret_cast<LPVOID> (amHandle + 0x1BBD0));
		INSTALL_HOOK_DYNAMIC (callQrUnknown, reinterpret_cast<LPVOID> (amHandle + 0xFD40));
		// 08.18 has no Send1
		INSTALL_HOOK_DYNAMIC (Send2, reinterpret_cast<LPVOID> (amHandle + 0x1C2D0));
		INSTALL_HOOK_DYNAMIC (Send3, reinterpret_cast<LPVOID> (amHandle + 0x1C260));
		INSTALL_HOOK_DYNAMIC (Send4, reinterpret_cast<LPVOID> (amHandle + 0x1C220));
		INSTALL_HOOK_DYNAMIC (copy_data, reinterpret_cast<LPVOID> (amHandle + 0x1C2A0));
        break;
	}
	case GameVersion::CN_JUN_2023: {
		INSTALL_HOOK_DYNAMIC (qrInit, reinterpret_cast<LPVOID> (amHandle + 0x161B0));
		INSTALL_HOOK_DYNAMIC (qrRead, reinterpret_cast<LPVOID> (amHandle + 0x163A0));
		INSTALL_HOOK_DYNAMIC (qrClose, reinterpret_cast<LPVOID> (amHandle + 0x16350));
		INSTALL_HOOK_DYNAMIC (callQrUnknown, reinterpret_cast<LPVOID> (amHandle + 0x8F60));
		INSTALL_HOOK_DYNAMIC (Send1, reinterpret_cast<LPVOID> (amHandle + 0x16A30));
		INSTALL_HOOK_DYNAMIC (Send2, reinterpret_cast<LPVOID> (amHandle + 0x16A00));
		INSTALL_HOOK_DYNAMIC (Send3, reinterpret_cast<LPVOID> (amHandle + 0x16990));
		INSTALL_HOOK_DYNAMIC (Send4, reinterpret_cast<LPVOID> (amHandle + 0x16940));
		INSTALL_HOOK_DYNAMIC (copy_data, reinterpret_cast<LPVOID> (amHandle + 0x169D0));
        break;
	}
    default: {
        break;
    }
	}
}
} // namespace patches::Qr
