#define BASE_ADDRESS 0x140000000
#include "helpers.h"

const u64 song_data_size = 1024 * 1024 * 64;
void *song_data;

extern void set_song_data_rdx ();
extern void set_song_data_r8 ();

#define GENERATE_CALL(to)                                                                                                                            \
	0x48, 0xB8, (u8)(u64)(to), (u8)((u64)(to) >> 8), (u8)((u64)(to) >> 16), (u8)((u64)(to) >> 24), (u8)((u64)(to) >> 32), (u8)((u64)(to) >> 40),     \
	    (u8)((u64)(to) >> 48), (u8)((u64)(to) >> 56), 0xFF, 0xD0

#define NOPS_NEEDED(start, return_location) WRITE_NOP ((u64)(start) + 12, (u64)(return_location)-1 - (u64)(start)-11)

#define HOOK_SONG_DATA(function, start_loc, ret_loc, handle)                                                                                         \
	{                                                                                                                                                \
		WRITE_MEMORY (ASLR (start_loc, handle), u8, GENERATE_CALL (function));                                                                       \
		NOPS_NEEDED (ASLR (start_loc, handle), ASLR (ret_loc, handle));                                                                              \
	}

HOOK_DYNAMIC (u8, __fastcall, qrVtable1, u64 a1) { return 1; }
HOOK_DYNAMIC (u8, __fastcall, qrReadFromCOM1, u64 a1) {
	*(u32 *)(a1 + 40) = 1;
	*(u32 *)(a1 + 16) = 1;
	return 1;
}

void
PreInit () {
	toml_table_t *config = openConfig (configPath ("plugins/patches.toml"));
	if (!config) return;
	void *handle = GetModuleHandle (0);
	WRITE_MEMORY (ASLR (0x1400239C0, handle), u8, 0xC3);                                                                             // Stop error
	if (readConfigBool (config, "unlock_songs", true)) WRITE_MEMORY (ASLR (0x140314E8D, handle), u8, 0xB0, 0x01);                    // Unlock songs
	if (readConfigBool (config, "shared_audio", true)) WRITE_MEMORY (ASLR (0x140692E17, handle), u8, 0xEB);                          // Shared audio
	if (!readConfigBool (config, "vsync", false)) WRITE_MEMORY (ASLR (0x140517339, handle), u8, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x90); // Disable VSync
	// Remove song limit
	WRITE_MEMORY (ASLR (0x140313726, handle), i32, 9000);
	// Remove for with server
	WRITE_MEMORY (ASLR (0x1402F39E6, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1402F3AB0, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1402F3BE4, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x14030643B, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140306507, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403065D3, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403066FB, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403067C7, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140306893, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x14030698B, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140313666, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403139F4, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140313B04, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140313C24, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140313CF4, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403140C4, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403147AA, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140225FB6, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140226146, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314DCC, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314EC9, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140338E2C, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1400EE0A4, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1400EE8B5, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1400EEDA6, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140315608, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x14034A7EB, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x1402F3CB3, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314059, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140226063, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x14022609F, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140226296, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140306A2E, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314F46, handle), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314F97, handle), i32, 9000);

	song_data = malloc (song_data_size);
	memset (song_data, 0, song_data_size);

	// Song data
	HOOK_SONG_DATA (set_song_data_r8, 0x140313678, 0x140313685, handle);
	// Crown data
	HOOK_SONG_DATA (set_song_data_rdx, 0x1402F3AC2, 0x1402F3AD0, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x1402F39F8, 0x1402F3A06, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x1402F3BF6, 0x1402F3C04, handle);
	HOOK_SONG_DATA (set_song_data_r8, 0x1403140D4, 0x1403140E1, handle)
	// Score ranks
	HOOK_SONG_DATA (set_song_data_rdx, 0x1403065E6, 0x1403065F4, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x14030651A, 0x140306528, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x14030644E, 0x14030645C, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x1403068A6, 0x1403068B4, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x1403067DA, 0x1403067E8, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x14030670E, 0x14030671C, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x14030699E, 0x1403069B1, handle);
	// Unknown
	HOOK_SONG_DATA (set_song_data_rdx, 0x140313752, 0x14031375F, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x140313A08, 0x140313A15, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x140313B49, 0x140313B56, handle);
	HOOK_SONG_DATA (set_song_data_rdx, 0x140313D35, 0x140313D42, handle);
	HOOK_SONG_DATA (set_song_data_r8, 0x140313C3F, 0x140313C4C, handle);

	// Save settings cross session without F:/ and G:/ drive
	WRITE_MEMORY (ASLR (0x140B5C528, handle), char, "./Setting1.bin");
	WRITE_MEMORY (ASLR (0x140B5C538, handle), char, "./Setting2.bin");
	// Move F:/ files to current directory
	WRITE_MEMORY (ASLR (0x140B1B4B0, handle), char, "./");
	WRITE_MEMORY (ASLR (0x14001C941, handle), u8, 0x02);
	// Patch TLS v1.0 to v1.2
	WRITE_MEMORY (ASLR (0x14044B1A9, handle), u8, 0x10);

	toml_table_t *windowResSection = openConfigSection (config, "windowRes");
	if (windowResSection) {
		i32 windowResX = readConfigInt (windowResSection, "x", 0);
		i32 windowResY = readConfigInt (windowResSection, "y", 0);
		if (windowResX > 0 && windowResY > 0) {
			WRITE_MEMORY (ASLR (0x14035FC5B, handle), i32, windowResX);
			WRITE_MEMORY (ASLR (0x14035FC62, handle), i32, windowResY);
		}
	}

	toml_free (config);

	// Move various files to current directory
	void *amHandle = GetModuleHandle ("AMFrameWork.dll");
	WRITE_MEMORY (amHandle + 0x33EF7, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3404A, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x34429, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3457C, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3497A, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x34ACD, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x148AF, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x14A1A, u8, 0xEB);

	INSTALL_HOOK_DYNAMIC (qrVtable1, amHandle + 0x1BA00);
	INSTALL_HOOK_DYNAMIC (qrReadFromCOM1, amHandle + 0x1BC20);
}
