#include "bnusio.h"
#include "helpers.h"
#include "patches/patches.h"
#include "poll.h"
#include "constants.h"

GameVersion version = GameVersion::UNKNOWN;
std::vector<HMODULE> plugins;

const char *server  = "127.0.0.1";
char accessCode1[21] = "00000000000000000001";
char accessCode2[21] = "00000000000000000002";
char chipId1[33]     = "00000000000000000000000000000001";
char chipId2[33]     = "00000000000000000000000000000002";

HOOK (i32, ShowMouse, PROC_ADDRESS ("user32.dll", "ShowCursor"), bool) { return originalShowMouse (true); }
HOOK (i32, ExitWindows, PROC_ADDRESS ("user32.dll", "ExitWindowsEx")) {
	ExitProcess (0);
	return true;
}

HOOK (i32, XinputGetState, PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetState")) { return ERROR_DEVICE_NOT_CONNECTED; }
HOOK (i32, XinputSetState, PROC_ADDRESS ("xinput9_1_0.dll", "XInputSetState")) { return ERROR_DEVICE_NOT_CONNECTED; }
HOOK (i32, XinputGetCapabilites, PROC_ADDRESS ("xinput9_1_0.dll", "XInputGetCapabilities")) { return ERROR_DEVICE_NOT_CONNECTED; }

HOOK (i32, ssleay_Shutdown, PROC_ADDRESS ("ssleay32.dll", "SSL_shutdown")) { return 1; }

HOOK (i64, UsbFinderInitialize, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderInitialize")) { return 0; }
HOOK (i64, UsbFinderRelease, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderRelease")) { return 0; }
HOOK (i64, UsbFinderGetSerialNumber, PROC_ADDRESS ("nbamUsbFinder.dll", "nbamUsbFinderGetSerialNumber"), i32 a1, char *a2) {
	strcpy (a2, "284111080001");
	return 0;
}

HOOK (i32, ws2_getaddrinfo, PROC_ADDRESS ("ws2_32.dll", "getaddrinfo"), const char *node, char *service, void *hints, void *out) { return originalws2_getaddrinfo (server, service, hints, out); }

void
GetGameVersion () {
	wchar_t w_path[MAX_PATH];
	GetModuleFileNameW (0, w_path, MAX_PATH);
	std::filesystem::path path (w_path);

	if (!std::filesystem::exists (path) || !path.has_filename ()) {
		MessageBoxA (0, "Failed to find executable", 0, MB_OK);
		ExitProcess (0);
	}

	std::ifstream stream (path, std::ios::binary);
	if (!stream.is_open ()) {
		MessageBoxA (0, "Failed to read executable", 0, MB_OK);
		ExitProcess (0);
	}

	stream.seekg (0, stream.end);
	size_t length = stream.tellg ();
	stream.seekg (0, stream.beg);

	char *buf = (char *)calloc (length + 1, sizeof (char));
	stream.read (buf, length);

	version = (GameVersion)XXH64 (buf, length, 0);

	stream.close ();
	free (buf);

	switch (version) {
	case GameVersion::JP_NOV_2020:
	case GameVersion::CN_JUN_2023: break;
	default: MessageBoxA (0, "Unknown game version", 0, MB_OK); ExitProcess (0);
	}
}

void
createCard () {
	const char hexCharacterTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char buf[64]                   = {0};
	srand (time (0));

	std::generate (buf, buf + 20, [&] () { return hexCharacterTable[rand () % 10]; });
	WritePrivateProfileStringA ("card", "accessCode1", buf, ".\\card.ini");
	std::generate (buf, buf + 32, [&] () { return hexCharacterTable[rand () % 16]; });
	WritePrivateProfileStringA ("card", "chipId1", buf, ".\\card.ini");
	std::generate (buf, buf + 20, [&] () { return hexCharacterTable[rand () % 10]; });
	WritePrivateProfileStringA ("card", "accessCode2", buf, ".\\card.ini");
	std::generate (buf, buf + 32, [&] () { return hexCharacterTable[rand () % 16]; });
	WritePrivateProfileStringA ("card", "chipId2", buf, ".\\card.ini");
}

BOOL
DllMain (HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		// This is bad, dont do this
		// I/O in DllMain can easily cause a deadlock
		GetGameVersion ();

		auto pluginPath = std::filesystem::current_path () / "plugins";

		if (std::filesystem::exists (pluginPath)) {
			for (auto entry : std::filesystem::directory_iterator (pluginPath)) {
				if (entry.path ().extension () == ".dll") {
					auto name       = entry.path ().wstring ();
					HMODULE hModule = LoadLibraryW (name.c_str ());
					if (!hModule) {
						wchar_t buf[128];
						wsprintfW (buf, L"Failed to load plugin %ls", name.c_str ());
						MessageBoxW (0, buf, name.c_str (), MB_ICONERROR);
					} else {
						plugins.push_back (hModule);
					}
				}
			}
		}

		auto configPath      = std::filesystem::current_path () / "config.toml";
		toml_table_t *config = openConfig (configPath);
		if (config) {
			auto amauth = openConfigSection (config, "amauth");
			if (amauth) {
				server = readConfigString (amauth, "server", server);
			}
			toml_free (config);
		}

		if (!std::filesystem::exists (".\\card.ini")) createCard ();
		GetPrivateProfileStringA ("card", "accessCode1", accessCode1, accessCode1, 21, ".\\card.ini");
		GetPrivateProfileStringA ("card", "chipId1", chipId1, chipId1, 33, ".\\card.ini");
		GetPrivateProfileStringA ("card", "accessCode2", accessCode2, accessCode2, 21, ".\\card.ini");
		GetPrivateProfileStringA ("card", "chipId2", chipId2, chipId2, 33, ".\\card.ini");

		INSTALL_HOOK (ShowMouse);
		INSTALL_HOOK (ExitWindows);

		INSTALL_HOOK (XinputGetState);
		INSTALL_HOOK (XinputSetState);
		INSTALL_HOOK (XinputGetCapabilites);

		INSTALL_HOOK (ssleay_Shutdown);

		INSTALL_HOOK (UsbFinderInitialize);
		INSTALL_HOOK (UsbFinderRelease);
		INSTALL_HOOK (UsbFinderGetSerialNumber);

		INSTALL_HOOK (ws2_getaddrinfo);

		bnusio::Init ();

		switch (version) {
		case GameVersion::UNKNOWN: break;
		case GameVersion::JP_NOV_2020: patches::JP_NOV_2020::Init (); break;
		case GameVersion::CN_JUN_2023: patches::CN_JUN_2023::Init (); break;
		}
	}
	return true;
}
