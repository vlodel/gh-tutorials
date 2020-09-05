#include "stdafx.h"
#include <iostream>
#include "mem.h"
#include "csgo_ent.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"client.dll");

	CBaseEntityList* entList = (CBaseEntityList*)(moduleBase + 0x4D534EC);

	while (true)
	{
		if (GetAsyncKeyState(VK_F10) & 1)
		{
			break;
		}

		for (auto e : entList->entList)
		{
			if (e.entptr)
			{
				std::cout << e.entptr->health << "\n";
			}
		}

		std::cout << "=========" << "\n";
		Sleep(1000);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = nullptr;
		hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
		if (hThread)
		{
			CloseHandle(hThread);
		}

	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}