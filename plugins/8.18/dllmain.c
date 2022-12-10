#define BASE_ADDRESS 0x140000000
#include "helpers.h"

const u64 song_data_size = 1024 * 1024 * 64;
void *song_data;

#define RDX_MOV 0x48, 0xBA
#define R8_MOV  0x49, 0xB8
#define GENERATE_MOV(instruction, location)                                                                                                          \
	instruction, (u8)(u64)(location), (u8)((u64)(location) >> 8), (u8)((u64)(location) >> 16), (u8)((u64)(location) >> 24),                          \
	    (u8)((u64)(location) >> 32), (u8)((u64)(location) >> 40), (u8)((u64)(location) >> 48), (u8)((u64)(location) >> 56)

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
	WRITE_MEMORY (ASLR (0x14031367B, handle), u8, GENERATE_MOV (R8_MOV, song_data));
	// Crown data
	WRITE_MEMORY (ASLR (0x1402F3AC6, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1402F39FC, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1402F3BFA, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1403140D7, handle), u8, GENERATE_MOV (R8_MOV, song_data));
	// Score ranks
	WRITE_MEMORY (ASLR (0x1403065EA, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x14030651E, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140306452, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1403068AA, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1403067DE, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140306712, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1403069A2, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	// Unknown
	WRITE_MEMORY (ASLR (0x140313755, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140313A0B, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140313B4C, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140313D38, handle), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140313C42, handle), u8, GENERATE_MOV (R8_MOV, song_data));

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

void
BeforeCard1Insert () {
	memset (song_data, 0, song_data_size);
}

void
BeforeCard2Insert () {
	memset (song_data, 0, song_data_size);
}
