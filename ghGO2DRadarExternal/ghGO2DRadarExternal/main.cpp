#include "MemMan.h"
#include <Windows.h>

MemMan MemManager;

struct offsets {
	DWORD entityList = 0x4D5450C;
	DWORD isSpotted = 0x93D;
} offsets;

struct variables {
	DWORD gameModule;
} val;

int main(){

	int procId = MemManager.getProcess("csgo.exe");
	val.gameModule = MemManager.getModule(procId, "client.dll");

	while (true) {
		
		for (short int i = 0; i < 64; i++) {
			DWORD entity = MemManager.readMem<DWORD>(val.gameModule + offsets.entityList + i * 0x10);
			if (entity != NULL) {
				MemManager.writeMem<bool>(entity + offsets.isSpotted, true);
			}
		}

		Sleep(1);
	}

	return 0;
}