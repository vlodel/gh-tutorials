#include <Windows.h>

bool Hook(void* toHook, void* mFunc, int len) {
	if (len < 5) {
		return false;
	}

	DWORD currProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &currProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)mFunc - (DWORD)toHook)-5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, currProtection, &temp);

	return true;
}

DWORD jmpBackAddy;
void __declspec(naked) mFunc() {
	_asm {
		add ecx, ecx
		mov edx, [ebp-8]
		jmp [jmpBackAddy]
	}
}

DWORD WINAPI MainThread(LPVOID param) {
	int hookLength = 6;
	DWORD hookAddress = 0x332768;
	jmpBackAddy = hookAddress + hookLength;

	Hook((void*)hookAddress, mFunc, hookLength);

	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) break;
		Sleep(50);
	}

	FreeLibraryAndExitThread((HMODULE)param, 0);

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		break;
	}

	return TRUE;
}