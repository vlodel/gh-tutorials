#include <iostream>
#include <Windows.h>
#include "MemMan.h"

MemMan MemManager;

struct values {
    DWORD localPlayer;
    DWORD proccess;
    DWORD gameModule;
    BYTE flag;
}val;

struct gameOffsets {
    DWORD lPlayer = 0xD3FC5C;
    DWORD jump = 0x51FE22C;
    DWORD flag = 0x104;
}offsets;

int main()
{
    val.proccess = MemManager.getProcess("csgo.exe");
    val.gameModule = MemManager.getModule(val.proccess,"client.dll");
    //std::cout << std::hex << val.gameModule << std::endl;

    val.localPlayer = MemManager.readMem<DWORD>(val.gameModule + offsets.lPlayer);
    //std::cout << std::hex << val.localPlayer << std::endl;

    if (val.localPlayer == NULL) {
        while (val.localPlayer == NULL) {
            val.localPlayer = MemManager.readMem<DWORD>(val.gameModule + offsets.lPlayer);
        }
    }

    while (true) {
        val.flag = MemManager.readMem<BYTE>(val.localPlayer + offsets.flag);
        if (GetAsyncKeyState(VK_LCONTROL) && val.flag & (1 << 0)) {
            MemManager.writeMem<DWORD>(val.gameModule + offsets.jump, 6);
        }

        Sleep(1);
    }


    return 0;
}