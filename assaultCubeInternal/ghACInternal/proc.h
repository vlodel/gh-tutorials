#pragma once

#include "pch.h"
#include <vector>

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* modName);