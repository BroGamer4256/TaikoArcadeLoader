#define BASE_ADDRESS 0x140000000
#include "helpers.h"

i32 __stdcall DllMain (HMODULE mod, DWORD cause, void *ctx) {
	if (cause != DLL_PROCESS_ATTACH) 
		return 1;
	
	WRITE_MEMORY (ASLR(0x1400239C0), u8, 0xC3);       // Stop error
	WRITE_MEMORY (ASLR(0x140314E8D), u8, 0xB0, 0x01); // Unlock songs
	WRITE_MEMORY (ASLR(0x140692E17), u8, 0xEB);       // Shared audio
	WRITE_MEMORY (ASLR(0x140313726), u8, 0x00, 0x7F); // Remove song limit
	WRITE_MEMORY (ASLR(0x140517339), u8, 0xBA, 0x00, 0x00, 0x00, 0x00,
				  0x90); // Disable VSync
	// Save settings cross session
	WRITE_MEMORY (ASLR(0x140B5C528), u8, "./Setting1.bin");
	WRITE_MEMORY (ASLR(0x140B5C538), u8, "./Setting2.bin");

	return 1;
}
