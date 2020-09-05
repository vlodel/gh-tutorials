#pragma once
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
//https://guidedhacking.com/threads/reverse-engineering-how-to-find-the-csgo-entity-list.13313/

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);