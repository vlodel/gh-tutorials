#include <cstdint>
#include <Windows.h>

#include "Player.h"
#include "Vector3.h"
#include "offsets.h"

Player* Player::getPlayer(int index) {
	static uint32_t moduleBase = (uint32_t)GetModuleHandle("client.dll");
	static uint32_t entityList = moduleBase + hazedumper::signatures::dwEntityList;

	return (Player*)(entityList + index * 0x10);
}

int* Player::getMaxPlayer() {
	static uint32_t moduleBase = (uintptr_t)GetModuleHandle("engine.dll");
	return (int*)(*(uint32_t*)(moduleBase + hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_MaxPlayer);
}

int* Player::getHealth() {
	return (int*)(*(uint32_t*)this + hazedumper::netvars::m_iHealth);
}

int* Player::getTeam() {
	return (int*)(*(uint32_t*)this + hazedumper::netvars::m_iTeamNum);

}

Vector3* Player::getOrigin() {
	return (Vector3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin);
}

Vector3* Player::getViewOffset() {
	return (Vector3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecViewOffset);
}

Vector3* Player::getBonePos(int boneId) {
	uint32_t boneMatrix = *(uint32_t*)(*(uint32_t*)this + hazedumper::netvars::m_dwBoneMatrix);
	static Vector3 bonePos;
	bonePos.x = *(float*)(boneMatrix + 0x30 * boneId + 0x0C);
	bonePos.y = *(float*)(boneMatrix + 0x30 * boneId + 0x1C);
	bonePos.z = *(float*)(boneMatrix + 0x30 * boneId + 0x2C);
	return &bonePos;

}