// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include "run.h"

#define KeyDOWN -32768
#define KeyUP 0

DWORD WINAPI OnDllAttach(PVOID base) {
#ifdef _DEBUG
    AllocConsole();
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Debug");
#endif
    bool bOn = false;
    bool isKeyHeld = false;

    while (!(GetAsyncKeyState(VK_F10) & 0x8000)) {

        if (GetAsyncKeyState(VK_F2) & 1) {
            bOn = !bOn;
        }

        if (GetAsyncKeyState(VK_XBUTTON1) == KeyDOWN && !isKeyHeld) {
            isKeyHeld = true;
            bOn = true;
        }

        if (GetAsyncKeyState(VK_XBUTTON1) == KeyUP && isKeyHeld) {
            isKeyHeld = false;
            bOn = false;
        }

        if (bOn) {
            Run();
            Sleep(1);
        }
    }
    FreeLibraryAndExitThread(static_cast<HMODULE>(base), 0);
}

VOID WINAPI OnDllDetach() {
#ifdef _DEBUG
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);

    HWND hw_ConsoleHwnd = GetConsoleWindow();
    FreeConsole();
    PostMessageW(hw_ConsoleHwnd, WM_CLOSE, 0, 0);
#endif
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnDllAttach, hModule, NULL, NULL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {

        OnDllDetach();
    }
    return TRUE;
}
