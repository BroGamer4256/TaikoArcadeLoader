#define BASE_ADDRESS 0x140000000
#include "helpers.h"

i32 __stdcall DllMain (HMODULE mod, DWORD cause, void *ctx) {
	if (cause != DLL_PROCESS_ATTACH) 
		return 1;
	
	void *handle = GetModuleHandle (0);
	
	WRITE_MEMORY (ASLR(0x1400239C0, handle), u8, 0xC3);       // Stop error
	WRITE_MEMORY (ASLR(0x140314E8D, handle), u8, 0xB0, 0x01); // Unlock songs
	WRITE_MEMORY (ASLR(0x140692E17, handle), u8, 0xEB);       // Shared audio
	WRITE_MEMORY (ASLR(0x140313726, handle), u8, 0x00, 0x7F); // Remove song limit
	WRITE_MEMORY (ASLR(0x140517339, handle), u8, 0xBA, 0x00, 0x00, 0x00, 0x00,
				  0x90); // Disable VSync
	// Save settings cross session
	WRITE_MEMORY (ASLR(0x140B5C528, handle), u8, "./Setting1.bin");
	WRITE_MEMORY (ASLR(0x140B5C538, handle), u8, "./Setting2.bin");
	// CabinetOfflineData
	WRITE_MEMORY (ASLR(0x140b1b4b0, handle), u8, "./");
	WRITE_MEMORY (ASLR(0x14001c941, handle), u8, 0x02);

	void *amHandle = GetModuleHandle ("AMFrameWork.dll");
	WRITE_MEMORY (amHandle + 0x33EF7, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3404A, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x34429, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3457C, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3497A, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x34ACD, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x148AF, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x14A1A, u8, 0xEB);

	return 1;
}
