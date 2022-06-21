#include "helpers.h"

HOOK (i32, __stdcall, CrtMain, 0x140666d2c, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, i32 nShowCmd) {
	// Blatantly stolen patches from mon.im
	WRITE_MEMORY (0x1400239C0, u8, 0xC3);       // Stop error
	WRITE_MEMORY (0x140314E8D, u8, 0xB0, 0x01); // Unlock songs
	WRITE_MEMORY (0x140692E17, u8, 0xEB);       // Shared audio
	WRITE_MEMORY (0x140313726, u8, 0x00, 0x7F); // Remove song limit
	WRITE_MEMORY (0x140517339, u8, 0xBA, 0x00, 0x00, 0x00, 0x00,
	              0x90); // Disable VSync
	// Save settings cross session
	WRITE_MEMORY (0x140B5C528, u8, "./Setting1.bin");
	WRITE_MEMORY (0x140B5C538, u8, "./Setting2.bin");

	return originalCrtMain (hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

i32 __stdcall DllMain (HMODULE mod, DWORD cause, void *ctx) {
    if (cause != DLL_PROCESS_ATTACH) 
        return 1;
    
    INSTALL_HOOK(CrtMain);

    return 1;
}
