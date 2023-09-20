#include "helpers.h"
#include "patches.h"

const u64 song_data_size = 1024 * 1024 * 64;
void *song_data;

#define RDX_MOV 0x48, 0xBA
#define R8_MOV  0x49, 0xB8
#define GENERATE_MOV(instruction, location)                                                                                                                                           \
	instruction, (u8)(u64)(location), (u8)((u64)(location) >> 8), (u8)((u64)(location) >> 16), (u8)((u64)(location) >> 24), (u8)((u64)(location) >> 32), (u8)((u64)(location) >> 40), \
	    (u8)((u64)(location) >> 48), (u8)((u64)(location) >> 56)

namespace patches::JP_NOV_2020 {
void
Init () {
	i32 xRes         = 1360;
	i32 yRes         = 768;
	bool unlockSongs = true;
	bool sharedAudio = true;
	bool vsync       = false;

	auto configPath      = std::filesystem::current_path () / "config.toml";
	toml_table_t *config = openConfig (configPath);
	if (config) {
		auto res = openConfigSection (config, "res");
		if (res) {
			xRes = readConfigInt (res, "x", xRes);
			yRes = readConfigInt (res, "y", yRes);
		}
		unlockSongs = readConfigBool (config, "unlock_songs", unlockSongs);
		sharedAudio = readConfigBool (config, "shared_audio", sharedAudio);
		vsync       = readConfigBool (config, "vsync", vsync);
		toml_free (config);
	}

	WRITE_MEMORY (ASLR (0x1400239C0), u8, 0xC3); // Stop error
	if (unlockSongs) WRITE_MEMORY (ASLR (0x140314E8D), u8, 0xB0, 0x01);
	if (sharedAudio) WRITE_MEMORY (ASLR (0x140692E17), u8, 0xEB);
	if (!vsync) WRITE_MEMORY (ASLR (0x140517339), u8, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x90);

	// Remove song limit
	WRITE_MEMORY (ASLR (0x140313726), i32, 9000);
	WRITE_MEMORY (ASLR (0x1402F39E6), i32, 9000);
	WRITE_MEMORY (ASLR (0x1402F3AB0), i32, 9000);
	WRITE_MEMORY (ASLR (0x1402F3BE4), i32, 9000);
	WRITE_MEMORY (ASLR (0x14030643B), i32, 9000);
	WRITE_MEMORY (ASLR (0x140306507), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403065D3), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403066FB), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403067C7), i32, 9000);
	WRITE_MEMORY (ASLR (0x140306893), i32, 9000);
	WRITE_MEMORY (ASLR (0x14030698B), i32, 9000);
	WRITE_MEMORY (ASLR (0x140313666), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403139F4), i32, 9000);
	WRITE_MEMORY (ASLR (0x140313B04), i32, 9000);
	WRITE_MEMORY (ASLR (0x140313C24), i32, 9000);
	WRITE_MEMORY (ASLR (0x140313CF4), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403140C4), i32, 9000);
	WRITE_MEMORY (ASLR (0x1403147AA), i32, 9000);
	WRITE_MEMORY (ASLR (0x140225FB6), i32, 9000);
	WRITE_MEMORY (ASLR (0x140226146), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314DCC), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314EC9), i32, 9000);
	WRITE_MEMORY (ASLR (0x140338E2C), i32, 9000);
	WRITE_MEMORY (ASLR (0x1400EE0A4), i32, 9000);
	WRITE_MEMORY (ASLR (0x1400EE8B5), i32, 9000);
	WRITE_MEMORY (ASLR (0x1400EEDA6), i32, 9000);
	WRITE_MEMORY (ASLR (0x140315608), i32, 9000);
	WRITE_MEMORY (ASLR (0x14034A7EB), i32, 9000);
	WRITE_MEMORY (ASLR (0x1402F3CB3), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314059), i32, 9000);
	WRITE_MEMORY (ASLR (0x140226063), i32, 9000);
	WRITE_MEMORY (ASLR (0x14022609F), i32, 9000);
	WRITE_MEMORY (ASLR (0x140226296), i32, 9000);
	WRITE_MEMORY (ASLR (0x140306A2E), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314F46), i32, 9000);
	WRITE_MEMORY (ASLR (0x140314F97), i32, 9000);

	song_data = malloc (song_data_size);
	memset (song_data, 0, song_data_size);

	// Song data
	WRITE_MEMORY (ASLR (0x14031367B), u8, GENERATE_MOV (R8_MOV, song_data));
	// Crown data
	WRITE_MEMORY (ASLR (0x1402F3AC6), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1402F39FC), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1402F3BFA), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1403140D7), u8, GENERATE_MOV (R8_MOV, song_data));
	// Score ranks
	WRITE_MEMORY (ASLR (0x1403065EA), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x14030651E), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140306452), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1403068AA), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1403067DE), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140306712), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x1403069A2), u8, GENERATE_MOV (RDX_MOV, song_data));
	// Unknown
	WRITE_MEMORY (ASLR (0x140313755), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140313A0B), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140313B4C), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140313D38), u8, GENERATE_MOV (RDX_MOV, song_data));
	WRITE_MEMORY (ASLR (0x140313C42), u8, GENERATE_MOV (R8_MOV, song_data));

	// Save settings cross session without F:/ and G:/ drive
	WRITE_MEMORY (ASLR (0x140B5C528), char, "./Setting1.bin");
	WRITE_MEMORY (ASLR (0x140B5C538), char, "./Setting2.bin");
	// Move F:/ files to current directory
	WRITE_MEMORY (ASLR (0x140B1B4B0), char, "./");
	WRITE_MEMORY (ASLR (0x14001C941), u8, 0x02);
	// Patch TLS v1.0 to v1.2
	WRITE_MEMORY (ASLR (0x14044B1A9), u8, 0x10);

	if (xRes > 0 && yRes > 0) {
		WRITE_MEMORY (ASLR (0x14035FC5B), i32, xRes);
		WRITE_MEMORY (ASLR (0x14035FC62), i32, yRes);
	}

	// Move various files to current directory
	auto amHandle = GetModuleHandle ("AMFrameWork.dll");
	WRITE_MEMORY ((u64)amHandle + 0x33EF7, u8, 0xEB);
	WRITE_MEMORY ((u64)amHandle + 0x3404A, u8, 0xEB);
	WRITE_MEMORY ((u64)amHandle + 0x34429, u8, 0xEB);
	WRITE_MEMORY ((u64)amHandle + 0x3457C, u8, 0xEB);
	WRITE_MEMORY ((u64)amHandle + 0x3497A, u8, 0xEB);
	WRITE_MEMORY ((u64)amHandle + 0x34ACD, u8, 0xEB);
	WRITE_MEMORY ((u64)amHandle + 0x148AF, u8, 0xEB);
	WRITE_MEMORY ((u64)amHandle + 0x14A1A, u8, 0xEB);

	patches::Qr::Init ();

	patches::AmAuth::Init ();
}
} // namespace patches::JP_NOV_2020
