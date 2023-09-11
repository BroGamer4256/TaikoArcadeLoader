#include <ws2tcpip.h>

#include "helpers.h"
#include <unknwn.h>

extern const char *server;

namespace patches::AmAuth {
char server_ip[0x10];
DWORD reg = 0;

#ifdef DEFINE_GUID
#undef DEFINE_GUID
#endif

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) const GUID name = {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}
DEFINE_GUID (IID_CAuthFactory, 0x4603BB03, 0x058D, 0x43D9, 0xB9, 0x6F, 0x63, 0x9B, 0xE9, 0x08, 0xC1, 0xED);
DEFINE_GUID (IID_CAuth, 0x045A5150, 0xD2B3, 0x4590, 0xA3, 0x8B, 0xC1, 0x15, 0x86, 0x78, 0xE1, 0xAC);

class CAuth : public IUnknown {
public:
	STDMETHODIMP
	QueryInterface (REFIID riid, LPVOID *ppvObj) {
		wchar_t *iid_str;
		StringFromCLSID (riid, &iid_str);

		if (riid == IID_IUnknown || riid == IID_CAuth) {
			*ppvObj = this;
			this->AddRef ();
			return 0;
		} else {
			*ppvObj = 0;
			return E_NOINTERFACE;
		}
	}
	STDMETHODIMP_ (ULONG) AddRef () { return this->refCount++; }
	STDMETHODIMP_ (ULONG) Release () {
		this->refCount--;
		if (this->refCount <= 0) {
			// delete this;
			return 0;
		}
		return this->refCount;
	}

	virtual i64 Unk3 (u32 a1) { return 1; }
	virtual i64 Unk4 () { return 1; }
	virtual i32 Unk5 () { return 0; }
	virtual i64 Unk6 () { return 1; }
	virtual i32 Unk7 () { return 0; }
	virtual i32 Unk8 () { return 0; }
	virtual i32 Unk9 (i32 *a1) {
		memset (a1, 0, sizeof (i32) * 0x31);
		a1[0]  = 15;
		a1[2]  = 2;
		a1[3]  = 1;
		a1[6]  = 9;
		a1[8]  = 2;
		a1[9]  = 1;
		a1[10] = 27;
		a1[11] = 33;
		a1[12] = 41;
		a1[13] = 50;
		a1[14] = 59;
		a1[15] = 1179656;
		a1[30] = 1;
		a1[46] = 1;
		a1[47] = 3;
		a1[48] = 9;
		return 0;
	}
	virtual i32 Unk10 (char *a1) {
		memset (a1, 0, 0xB0);
		strncpy_s (a1, 0x10, "STANDALONE", 0xF);
		strncpy_s (a1 + 0x10, 0x10, "TAL0000001", 0xF);   // PCB ID
		strncpy_s (a1 + 0x20, 0x10, "000000-00000", 0xF); // ignored by game
		strncpy_s (a1 + 0x30, 0x10, server_ip, 0xF);
		strncpy_s (a1 + 0x40, 0x10, server_ip, 0xF);
		strncpy_s (a1 + 0x50, 0x10, server_ip, 0xF);
		strncpy_s (a1 + 0x60, 0x10, "***.***.***.***", 0xF); // Subnet mask
		strncpy_s (a1 + 0x70, 0x10, "***.***.***.***", 0xF); // GATEWAY
		strncpy_s (a1 + 0x80, 0x10, "***.***.***.***", 0xF); // PRIMARY DNS
		return 0;
	}
	virtual i32 Unk11 (char *a1) {
		memset (a1, 0, 0x13C);
		strncpy_s (a1, 4, "1", 3);
		strncpy_s (a1 + 4, 0x10, "ALLNET", 0xF);
		strncpy_s (a1 + 20, 8, "SBWY", 7);
		strncpy_s (a1 + 28, 8, "12.00", 7);
		strncpy_s (a1 + 36, 8, "TAL0", 7);  // ignored by game
		strncpy_s (a1 + 44, 8, "08.18", 7); // GAME VERSION
		strncpy_s (a1 + 52, 4, "0", 3);
		strncpy_s (a1 + 56, 4, "PCB", 3);
		return 0;
	}
	virtual i32 Unk12 () { return 1; }
	virtual i32 Unk13 () { return 1; }
	virtual i32 Unk14 (char *a1) {
		memset (a1, 0, 0x8A2);
		strncpy_s (a1, 0x101, server, 0x100);
		strncpy_s (a1 + 0x101, 0x101, server, 0x100);
		strncpy_s (a1 + 0x202, 0x100, "TAIKO ARCADE LOADER", 0xFF); // ALL.Net SHOP NAME
		strncpy_s (a1 + 0x302, 0x100, "TAIKO ARCADE LOADER", 0xFF);
		strncpy_s (a1 + 0x402, 0x10, "1", 0xF);
		strncpy_s (a1 + 0x412, 0x100, "TAIKO ARCADE LOADER", 0xFF);
		strncpy_s (a1 + 0x512, 0x100, "X", 0xFF);
		strncpy_s (a1 + 0x612, 0x100, "Y", 0xFF);
		strncpy_s (a1 + 0x712, 0x100, "Z", 0xFF);
		strncpy_s (a1 + 0x812, 0x10, "JPN0123", 0xF);
		strncpy_s (a1 + 0x832, 0x10, "JPN", 0xF);
		strncpy_s (a1 + 0x842, 0x10, "002,00", 0xF);
		strncpy_s (a1 + 0x842, 0x10, "PowerOnResponseVer2", 0xF);
		return 0;
	}
	virtual i32 Unk15 () { return 0; }
	virtual i32 Unk16 () { return 0; }
	virtual i32 Unk17 () { return 0; }
	virtual i32 Unk18 (void *a1) { return 0; }
	virtual i32 Unk19 (u8 *a1) {
		memset (a1, 0, 0x38);
		a1[0] = 1;
		return 1;
	}
	virtual i32 Unk20 () { return 0; }
	virtual i32 Unk21 () { return 1; }
	virtual i32 Unk22 () { return 0; }
	virtual i32 Unk23 () { return 0; }
	virtual i32 Unk24 () { return 0; }
	virtual i32 Unk25 () { return 1; }
	virtual i32 Unk26 () { return 0; }
	virtual i32 Unk27 () { return 1; }
	virtual i32 Unk28 () { return 0; }
	virtual i32 Unk29 () { return 0; }
	virtual i32 Unk30 () { return 0; }
	virtual i32 PrintDebugInfo () { return 0; }
	virtual i32 Unk32 (void *a1) { return 0; }
	virtual void Unk33 () {}

public:
	CAuth () {}
	virtual ~CAuth () {}

private:
	i32 refCount = 0;
};

class CAuthFactory : public IClassFactory {
public:
	STDMETHODIMP
	QueryInterface (REFIID riid, LPVOID *ppvObj) {
		wchar_t *iid_str;
		StringFromCLSID (riid, &iid_str);
		printf ("QueryInterface %ls\n", iid_str);

		if (riid == IID_IUnknown || riid == IID_IClassFactory || riid == IID_CAuthFactory) {
			*ppvObj = this;
			return 0;
		} else {
			*ppvObj = 0;
			return E_NOINTERFACE;
		}
	}
	STDMETHODIMP_ (ULONG) AddRef () { return 2; }
	STDMETHODIMP_ (ULONG) Release () { return 1; }
	virtual HRESULT CreateInstance (IUnknown *outer, REFIID riid, void **object) {
		if (outer != 0) return CLASS_E_NOAGGREGATION;
		wchar_t *iid_str;
		StringFromCLSID (riid, &iid_str);
		CAuth *auth = new CAuth ();
		return auth->QueryInterface (riid, object);
	}
	virtual HRESULT LockServer (i32 lock) { return 0; }
};

void
Init () {
	CoInitializeEx (0, 0);
	CoRegisterClassObject (IID_CAuthFactory, (IUnknown *)new CAuthFactory (), CLSCTX_LOCAL_SERVER, 1, &reg);

	struct addrinfo *res = 0;
	getaddrinfo (server, "", 0, &res);
	for (struct addrinfo *i = res; i != 0; i = i->ai_next) {
		if (res->ai_addr->sa_family != AF_INET) continue;
		struct sockaddr_in *p = (struct sockaddr_in *)res->ai_addr;
		inet_ntop (AF_INET, &p->sin_addr, server_ip, 0x10);
		break;
	}
}
} // namespace patches::AmAuth
