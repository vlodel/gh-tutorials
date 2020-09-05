#include "stdafx.h"
#include <iostream>
#include "mem.h"
#include "hook.h"
#include "glDraw.h"
#include "gltext.h"

//https://guidedhacking.com/threads/opengl-hooking-drawing-text-rendering.14460/

uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

bool bHealth = false, bAmmo = false, bRecoil = false;

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

twglSwapBuffers owglSwapBuffers;
twglSwapBuffers wglSwapBuffersGateway;

GL::Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

const char* example = "ESP Box";
const char* example2 = "I'm inside fam";

void Draw()
{
	HDC currentHDC = wglGetCurrentDC();

	if (!glFont.bBuilt || currentHDC != glFont.hdc)
	{
		glFont.Build(FONT_HEIGHT);
	}

	GL::SetupOrtho();

	GL::DrawOutline(300, 300, 200, 200, 2.0f, rgb::red);

	float textPointX = glFont.centerText(300, 200, strlen(example) * FONT_WIDTH);
	float textPointY = 300 - FONT_HEIGHT / 2;

	glFont.Print(textPointX, textPointY, rgb::green, "%s", example);

	vec3 insideTextPoint = glFont.centerText(300, 300 + 100, 200, 200, strlen(example2) * FONT_WIDTH, FONT_HEIGHT);
	glFont.Print(insideTextPoint.x, insideTextPoint.y, rgb::green, "%s", example2);

	GL::RestoreGL();
}

BOOL __stdcall hkwglSwapBuffers(HDC hDc)
{
	if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		bHealth = !bHealth;

	if (GetAsyncKeyState(VK_NUMPAD2) & 1)
	{
		bAmmo = !bAmmo;
	}

	//no recoil NOP
	if (GetAsyncKeyState(VK_NUMPAD3) & 1)
	{
		bRecoil = !bRecoil;

		if (bRecoil)
		{
			mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
		}

		else
		{
			//50 8D 4C 24 1C 51 8B CE FF D2 the original stack setup and call
			mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
		}
	}

	//need to use uintptr_t for pointer arithmetic later
	uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10F4F4);

	//continuous writes / freeze
	if (localPlayerPtr)
	{
		if (bHealth)
		{
			*(int*)(*localPlayerPtr + 0xF8) = 1337;
		}

		if (bAmmo)
		{
			*(int*)mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 }) = 1337;
		}
	}

	Draw();

	return wglSwapBuffersGateway(hDc);
}

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "OG for a fee, stay sippin' fam\n";

	// Hook
	Hook SwapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);
	SwapBuffersHook.Enable();

	//Sleep(10000);
	//SwapBuffersHook.Disable();

	//

	fclose(f);
	FreeConsole();
	Sleep(1000);
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
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}