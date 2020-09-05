#include <Windows.h>
#include <iostream>
#include <cmath>
#include "MemMan.h"

#define KeyDOWN -32768
#define KeyUP 0

MemMan memManager;

struct offsets {
	DWORD localPlayer = 0xD3FC5C;
	DWORD forceLMB = 0x3185AA0;
	DWORD entityList = 0x4D5450C;
	DWORD crosshair = 0xB3E4;
	DWORD team = 0xF4;
	DWORD health = 0x100;
	DWORD vectorOrigin = 0x138;
	DWORD activeWeapon = 0x2EF8;
	DWORD isScoped = 0x3928;
	DWORD itemDefinitionIndex = 0x2FAA;
}offset;

struct variables {
	DWORD localPlayer;
	DWORD gameModule;
	int localPlayerTeam;
	int shootDelay;
	int localPlayerWeaponId;
}val;

struct Vector3 {
	float x, y, z;
};

bool checkIsScoped() {
	return memManager.readMem<bool>(val.localPlayer + offset.isScoped);
}

void setTriggerDelay(float distance) {
	float delay;

	switch (val.localPlayerWeaponId) {
	case 262204: 
		delay = 3;
		break;
	case 7:
		delay = 3.3;
		break;
	case 40:
		delay = 0.15;
		break;
	case 262184:
		delay = 0.15;
		break;
	default: delay = 0;
	}

	val.shootDelay = delay * distance;
}

void getWeapon() {
	int weapon = memManager.readMem<int>(val.localPlayer + offset.activeWeapon);
	int weaponEntity = memManager.readMem<int>(val.gameModule + offset.entityList + ((weapon & 0xFFF) - 1) * 0x10);

	if (weaponEntity != NULL) {
		val.localPlayerWeaponId = memManager.readMem<int>(weaponEntity + offset.itemDefinitionIndex);
	}
}

float getDistance(DWORD entity) {
	Vector3 localPlayerLocation = memManager.readMem<Vector3>(val.localPlayer + offset.vectorOrigin);
	Vector3 enemyLocation = memManager.readMem<Vector3>(entity + offset.vectorOrigin);

	float distance = sqrt(pow(localPlayerLocation.x - enemyLocation.x, 2) + pow(localPlayerLocation.y - enemyLocation.y, 2)
		+ pow(localPlayerLocation.z - enemyLocation.z, 2)) * 0.0254;
	
	return distance;
}

void shoot() {
	Sleep(val.shootDelay);
	memManager.writeMem<int>(val.gameModule + offset.forceLMB, 5);
	Sleep(20);
	memManager.writeMem<int>(val.gameModule + offset.forceLMB, 4);
}

bool checkTragaci() {
	int crosshair = memManager.readMem<int>(val.localPlayer + offset.crosshair);
	if (crosshair != 0 && crosshair < 64) {
		DWORD entity = memManager.readMem<DWORD>(val.gameModule + offset.entityList + ((crosshair - 1) * 0x10));

		int entityTeam = memManager.readMem<int>(entity + offset.team);
		int entityHealth = memManager.readMem<int>(entity + offset.health);
		
		if (entityTeam != val.localPlayerTeam && entityHealth > 0) {
			float distance = getDistance(entity);
			getWeapon();
			setTriggerDelay(distance);
			if (val.localPlayerWeaponId == 9 || val.localPlayerWeaponId == 262184) {
				return checkIsScoped();
			}
			return true;
		}
		else 
		return false;
	} else 
	return false;
}

void handleTragaci() {
	if (checkTragaci()) {
		shoot();
	}
}

int main() {
	bool isTragaciOn = false;
	bool isKeyHeld = false;

	int pid = memManager.getProcess("csgo.exe");
	val.gameModule = memManager.getModule(pid, "client.dll");

	val.localPlayer = memManager.readMem<DWORD>(val.gameModule + offset.localPlayer);

	if (val.localPlayer == NULL) {
		while (val.localPlayer == NULL) {
			val.localPlayer = memManager.readMem<DWORD>(val.gameModule + offset.localPlayer);
		}
	}

	while (true) {
		if (GetAsyncKeyState(VK_F2) & 1) {
			isTragaciOn = !isTragaciOn;
			val.localPlayerTeam = memManager.readMem<int>(val.localPlayer + offset.team);
		}

		if (GetAsyncKeyState(VK_XBUTTON1) == KeyDOWN && !isKeyHeld) {
			isKeyHeld = true;
			isTragaciOn = true;
		}

		if (GetAsyncKeyState(VK_XBUTTON1) == KeyUP && isKeyHeld) {
			isKeyHeld = false;
			isTragaciOn = false;
		}

		if (isTragaciOn) {
			handleTragaci();
		}

		Sleep(1);
	}

	return 0;
}