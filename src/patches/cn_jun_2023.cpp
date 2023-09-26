#include "helpers.h"
#include "patches.h"
namespace patches::CN_JUN_2023 {

u8 *haspBuffer;
HOOK (i32, HaspDecrypt, PROC_ADDRESS ("hasp_windows_x64.dll", "hasp_decrypt")) { return 0; }
HOOK (i32, HaspEncrypt, PROC_ADDRESS ("hasp_windows_x64.dll", "hasp_encrypt")) { return 0; }
HOOK (i32, HaspLogout, PROC_ADDRESS ("hasp_windows_x64.dll", "hasp_logout")) { return 0; }
HOOK (i32, HaspWrite, PROC_ADDRESS ("hasp_windows_x64.dll", "hasp_write")) { return 0; }
HOOK (i32, HaspLogin, PROC_ADDRESS ("hasp_windows_x64.dll", "hasp_login"), i32, char *, i32 *id) {
	*id = 1;
	return 0;
}
HOOK (i32, HaspGetInfo, PROC_ADDRESS ("hasp_windows_x64.dll", "hasp_get_info"), const char *, const char *, void *, const char **a4) {
	*a4 = "type=\"HASP-HL\"";
	return 0;
}
HOOK (i32, HaspRead, PROC_ADDRESS ("hasp_windows_x64.dll", "hasp_read"), i32, i32, i32 offset, i32 length, void *buffer) {
	memcpy (buffer, haspBuffer + offset, length);
	return 0;
}

void (__fastcall* lua_touserdata) (i64 L, i64 index);
void (__fastcall* lua_settop) (i64 L, i64 index);
void (__fastcall* lua_pushboolean) (i64 L, i64 b);

HOOK (i64, sub_1401AC550, ASLR (0x1401AC550), i64 a1) {
	lua_touserdata (a1, 4294957292);
	lua_settop (a1, 0);
	lua_pushboolean (a1, 1);
	return 1;
}

void
Init () {
	i32 xRes         = 1920;
	i32 yRes         = 1080;
	bool unlockSongs = true;
	bool sharedAudio = true;
	bool vsync       = false;

	haspBuffer = (u8 *)malloc (0xD40);
	memset (haspBuffer, 0, 0xD40);
	strcpy ((char *)(haspBuffer + 0xD00), "284111080001");
	u8 crc = 0;
	for (int i = 0; i < 62; i++)
		crc += haspBuffer[0xD00 + i];
	haspBuffer[0xD3E] = crc;
	haspBuffer[0xD3F] = haspBuffer[0xD3E] ^ 0xFF;

	INSTALL_HOOK (HaspDecrypt);
	INSTALL_HOOK (HaspEncrypt);
	INSTALL_HOOK (HaspLogout);
	INSTALL_HOOK (HaspWrite);
	INSTALL_HOOK (HaspLogin);
	INSTALL_HOOK (HaspGetInfo);
	INSTALL_HOOK (HaspRead);

	auto configPath      = std::filesystem::current_path () / "config.toml";
	toml_table_t *config = openConfig (configPath);
	if (config) {
		auto patches = openConfigSection (config, "patches");
		if (patches) {
			auto res = openConfigSection (patches, "res");
			if (res) {
				xRes = readConfigInt (res, "x", xRes);
				yRes = readConfigInt (res, "y", yRes);
			}
			unlockSongs = readConfigBool (patches, "unlock_songs", unlockSongs);
			sharedAudio = readConfigBool (patches, "shared_audio", sharedAudio);
			vsync       = readConfigBool (patches, "vsync", vsync);
		}
		toml_free (config);
	}

	WRITE_MEMORY (ASLR (0x14003F690), u8, 0xC3); // Skip errors
	if (unlockSongs) WRITE_MEMORY (ASLR (0x140425BCD), u8, 0xB0, 0x01);
	if (sharedAudio) WRITE_MEMORY (ASLR (0x140777F87), u8, 0xEB);
	if (!vsync) WRITE_MEMORY (ASLR (0x1405FC5B9), u8, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x90);

	// Move various files to current dir
	WRITE_MEMORY (ASLR (0x140C7B158), char, "./SettingChina1.bin");
	WRITE_MEMORY (ASLR (0x140C7B2B8), char, "./SettingChina1.bin");
	WRITE_MEMORY (ASLR (0x140C7B2A0), char, "./SettingChina2.bin");
	WRITE_MEMORY (ASLR (0x140C33C40), char, "./");
	WRITE_MEMORY (ASLR (0x140C33C44), char, "./");

	// Res
	WRITE_MEMORY (ASLR (0x1404A4ED3), i32, xRes);
	WRITE_MEMORY (ASLR (0x1404A4EDA), i32, yRes);

	// Use TLS v1.2
	WRITE_MEMORY (ASLR (0x140369662), u8, 0x10);

	// Disable SSLVerify
	WRITE_MEMORY (ASLR (0x14034C182), u8, 0x00);

	lua_touserdata = reinterpret_cast<void ((__fastcall*)(i64, i64))> (PROC_ADDRESS ("lua51.dll", "lua_touserdata"));
	lua_settop = reinterpret_cast<void ((__fastcall*)(i64, i64))> (PROC_ADDRESS ("lua51.dll", "lua_settop"));
	lua_pushboolean = reinterpret_cast<void ((__fastcall*)(i64, i64))> (PROC_ADDRESS ("lua51.dll", "lua_pushboolean"));

	INSTALL_HOOK (sub_1401AC550);

	patches::Qr::Init ();
}
} // namespace patches::CN_JUN_2023
