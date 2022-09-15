// Here be lions
#include <chrono>
#include <combaseapi.h>
#include <thread>
#include <unknwn.h>
#include <winerror.h>
#include <winnt.h>
// Needs to be after for DEFINE_GUID
#include "helpers.h"

extern "C" {

DWORD reg = 0;

DEFINE_GUID (IID_CAuthFactory, 0x4603BB03, 0x058D, 0x43D9, 0xB9, 0x6F, 0x63, 0x9B, 0xE9, 0x08, 0xC1, 0xED);
DEFINE_GUID (IID_CAuth, 0x045A5150, 0xD2B3, 0x4590, 0xA3, 0x8B, 0xC1, 0x15, 0x86, 0x78, 0xE1, 0xAC);

class CAuth : public IUnknown {
  public:
	STDMETHODIMP
	QueryInterface (REFIID riid, LPVOID *ppvObj) {
		wchar_t *iid_str;
		StringFromCLSID (riid, &iid_str);
		printf ("QueryInterface %ls\n", iid_str);

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
			delete this;
			return 0;
		}
		return this->refCount;
	}

	// Seems to verify that local network is valid?
	virtual i64
	Unk3 (u32 a1) {
		return 1;
	}
	virtual i64
	Unk4 () {
		return 1;
	}
	// Seems to check a config field, something to do with server/client
	virtual i32
	Unk5 () {
		return 0;
	}
	// Seems to check that some config field is valid
	virtual i64
	Unk6 () {
		return 1;
	}
	// Sets a random global to 1
	virtual i32
	Unk7 () {
		return 0;
	}
	// Set some global vars
	virtual i32
	Unk8 () {
		return 0;
	}
	// Puts some globals into a1
	virtual i32
	Unk9 (i32 *a1) {
		printf ("Unk9\n");
		a1[0x28] = 0;
		a1[1]    = 1;
		a1[7]    = 1;
		a1[0x2C] = 1;
		a1[0x2A] = 1;
		a1[0x2E] = 1;
		a1[0x31] = 0x11;
		a1[0x30] = 1;
		a1[0x2F] = 1;
		return 0;
	}
	// Copies from a global to a1
	virtual i32
	Unk10 (void *a1) {
		printf ("Unk10\n");
		memset (a1, 0, 0xA8);
		return 0;
	}
	// Lotsa fancy stuff here
	virtual i32
	Unk11 (char *a1) {
		printf ("Unk11\n");
		memset (a1, 0, 0x13C);
		strncpy_s (a1, 4, "1", 3);
		strncpy_s (a1 + 4, 0x10, "ALLNET", 0xF);
		strncpy_s (a1 + 20, 8, "SWBY", 7);
		strncpy_s (a1 + 28, 8, "12.00", 7);
		strncpy_s (a1 + 36, 8, "S121", 7);
		strncpy_s (a1 + 44, 8, "08.18", 7);
		strncpy_s (a1 + 52, 4, "0", 3);
		strncpy_s (a1 + 56, 4, "PCB", 3);
		strncpy_s (a1 + 60, 0x100, "https://divamodarchive.com/", 0xFF);
		return 0;
	}
	// Some config stuff, unsure if true or false wanted
	virtual i32
	Unk12 () {
		return 1;
	}
	virtual i32
	Unk13 () {
		return 1;
	}
	// Seems to be some kind of copy? Can return E_FAIL and E_ACCESSDENIED
	virtual i32
	Unk14 (void *a1) {
		printf ("Unk14\n");
		memset (a1, 0, 0x8A2);
		return 0;
	}
	// Can return E_FAIL E_ABORT E_UNEXPECTED
	virtual i32
	Unk15 () {
		return 0;
	}
	// Config stuff
	virtual i32
	Unk16 () {
		return 0;
	}
	virtual i32
	Unk17 () {
		return 0;
	}
	// Seems to be some kind of copy? Can return E_FAIL and E_ACCESSDENIED
	virtual i32
	Unk18 (void *a1) {
		printf ("Unk18\n");
		return 0;
	}
	// Fancy stuff happening here
	virtual i32
	Unk19 (void *a1) {
		printf ("Unk19\n");
		return 0;
	}
	// Can return E_FAIL E_ABORT E_UNEXPECTED E_NOTIMPL
	// Something to do with MH
	virtual i32
	Unk20 () {
		printf ("Unk20\n");
		return 0;
	}
	// Can return E_FAIL E_ABORT E_UNEXPECTED E_NOTIMPL
	// Something to do with a list<T>
	// Maybe an init function
	virtual i32
	Unk21 () {
		return 1;
	}
	virtual i32
	Unk22 () {
		return 0;
	}
	virtual i32
	Unk23 () {
		return 0;
	}
	// E_FAIL E_NOTIMPL
	virtual i32
	Unk24 () {
		return 0;
	}
	// E_FAIL E_UNEXPECTED E_NOTIMPL
	virtual i32
	Unk25 () {
		return 1;
	}
	// E_FAIL E_NOTIMPL
	virtual i32
	Unk26 () {
		return 0;
	}
	// E_FAIL E_UNEXPECTED E_NOTIMPL
	virtual i32
	Unk27 () {
		return 1;
	}
	// E_FAIL E_NOTIMPL
	virtual i32
	Unk28 () {
		return 0;
	}
	// E_FAIL E_ACCESSDENIED
	virtual i32
	Unk29 () {
		return 0;
	}
	virtual i32
	Unk30 () {
		return 0;
	}
	// nothing special
	virtual i32
	PrintDebugInfo () {
		return 0;
	}
	virtual i32
	Unk32 (void *a1) {
		return 0;
	}
	virtual void
	Unk33 () {}

  public:
	CAuth () { printf ("CAuth\n"); }
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
			this->AddRef ();
			return 0;
		} else {
			*ppvObj = 0;
			return E_NOINTERFACE;
		}
	}
	STDMETHODIMP_ (ULONG) AddRef () { return 2; }
	STDMETHODIMP_ (ULONG) Release () { return 1; }
	virtual HRESULT
	CreateInstance (IUnknown *outer, REFIID riid, void **object) {
		if (outer != 0) return CLASS_E_NOAGGREGATION;
		wchar_t *iid_str;
		StringFromCLSID (riid, &iid_str);
		printf ("CreateInstance %ls\n", iid_str);
		if (riid == IID_CAuth) {
			CAuth *auth = new CAuth ();
			return auth->QueryInterface (riid, object);
		} else {
			*object = 0;
			return E_NOINTERFACE;
		}
	}
	virtual HRESULT
	LockServer (i32 lock) {
		return 0;
	}
};

void
Init () {
	CoInitializeEx (0, 0);
	CoRegisterClassObject (IID_CAuthFactory, (IUnknown *)new CAuthFactory (), 4, 1, &reg);
}

void
Exit () {
	CoRevokeClassObject (reg);
	CoUninitialize ();
}
}
