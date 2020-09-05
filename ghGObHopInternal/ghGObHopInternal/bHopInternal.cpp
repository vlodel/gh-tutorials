#include "pch.h"
#include <Windows.h>
#include <iostream>

struct values {
    DWORD localPlayer;
    DWORD gameModule;
    BYTE flag;
}val;

struct gameOffsets {
    DWORD lPlayer = 0xD3EC6C;
    DWORD jump = 0x51FD20C;
    DWORD flag = 0x104;
    DWORD velocity = 0x114;
}offsets;

struct Vector3 {
    float x, y, z;
};

bool isPlayerMoving() {

    Vector3 playerVelocity=*(Vector3*)(val.localPlayer+offsets.velocity);

    int velocity = playerVelocity.x + playerVelocity.y + playerVelocity.z;
    if (velocity != 0) {
        return true;
    }

    return false;
}

void main() {

    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    val.gameModule = (DWORD)GetModuleHandle("client.dll");
    val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);

    if (val.localPlayer == NULL) {
        while (val.localPlayer == NULL) {
            val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);
        }
    }

    while (true) {
        val.flag = *(BYTE*)(val.localPlayer + offsets.flag);

        if (isPlayerMoving()) {
            if (GetAsyncKeyState(VK_LCONTROL) && val.flag & (1 << 0)) {
                *(DWORD*)(val.gameModule + offsets.jump) = 6;
            }
        }

        if (GetAsyncKeyState(VK_F10) & 1)
        {
            break;
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL,(LPTHREAD_START_ROUTINE)main,NULL,NULL,NULL);
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}