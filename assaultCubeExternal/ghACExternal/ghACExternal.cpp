#include "stdafx.h"
#include "proc.h"
#include "mem.h"

int main()
{
    ////Get PID of the target process
    //DWORD pid = GetProcId(L"ac_client.exe");

    ////GetModuleBaseAddress
    //uintptr_t moduleBase = GetModuleBaseAddress(pid, L"ac_client.exe");

    ////Get Handle to the Process
    //HANDLE hProcess = 0;
    //hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid); 

    ////Resolve base address of the pointer chaing
    //uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10F4F4;

    //std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    ////Resolve our ammo pointer cchain
    //std::vector<unsigned int> ammoOffsets = { 0x374,0x14,0x0 };
    //uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    //std::cout << "AmmoAddr = " << "0x" << std::hex << ammoAddr << std::endl;

    ////Read ammo value
    //int ammoValue = 0;

    //ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    //std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;

    ////Write to it
    //int newAmmo = 999;
    //WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    ////Read out again
    //ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    //std::cout << "New ammo = " << std::dec << ammoValue << std::endl;


    //getchar();
    //return 0;

    HANDLE hProcess = 0;

    uintptr_t moduleBase = 0;
    uintptr_t localPlayerPtr = 0;
    uintptr_t healthAddr = 0;

    bool bHealth = false;
    bool bAmmo = false;
    bool bRecoil = false;
    bool bHack = false;
    BYTE spectateOn = 5;
    BYTE spectateOff = 0;
    BYTE invisibleOn = 11;
    BYTE invisibleOff = 0;

    uintptr_t entAddr = 0;
    uintptr_t spectateAddr = 0;
    uintptr_t invisibleAddr = 0;

    const int newValue = 999;

    DWORD pid = GetProcId(L"ac_client.exe");

    if (pid) {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

        moduleBase = GetModuleBaseAddress(pid, L"ac_client.exe");

        localPlayerPtr = moduleBase + 0x10f4f4;

        healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xf8 });
    }
    else {
        std::cout << "Process not found." << std::endl;
        getchar();
        return 0;
    }

    DWORD dwExit = 0;

    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {

        if (GetAsyncKeyState(VK_F1) & 1) {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_F2) & 1) {
            bAmmo = !bAmmo;

            if (bAmmo) {
                //FF 06 = inc[esi];
                PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
            }
            else {
                //ff 0E = dec[esi];
                PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_F3) & 1) {
            bRecoil = !bRecoil;

            if (bRecoil) {
                NopEx((BYTE*)(moduleBase + 0x63786), 10, hProcess);
            }
            else {
                PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_F4) & 1) {
            return 0;
        }

        if (GetAsyncKeyState(VK_F5) & 1) {
            bHack = !bHack;

            ReadProcessMemory(hProcess, (BYTE*)localPlayerPtr, &entAddr, sizeof(entAddr), nullptr);
            spectateAddr = entAddr + 0x338;
            invisibleAddr = entAddr + 0x82;

            if (bHack) {
                WriteProcessMemory(hProcess, (BYTE*)spectateAddr, &spectateOn, sizeof(spectateOn), nullptr);
                WriteProcessMemory(hProcess, (BYTE*)invisibleAddr, &invisibleOn, sizeof(invisibleOn), nullptr);

            }
            else {
                WriteProcessMemory(hProcess, (BYTE*)spectateAddr, &spectateOff, sizeof(spectateOff), nullptr);
                WriteProcessMemory(hProcess, (BYTE*)invisibleAddr, &invisibleOff, sizeof(invisibleOff), nullptr);
            }
        }

        //contionus write or freeze
        if (bHealth) {
            PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
        }

        Sleep(10);
    }

    std::cout << "Process not found, press enter to exit...\n";
    getchar();
    
}
