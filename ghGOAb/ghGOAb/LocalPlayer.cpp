#include <cstdint>
#include <Windows.h>
#include <math.h>

#include "LocalPlayer.h"
#include "offsets.h"
#include "Vector3.h"

#define M_PI           3.14159265358979323846  /* pi */

LocalPlayer* LocalPlayer::get() {
	static uint32_t clientModule = (uint32_t)(GetModuleHandle("client.dll"));
	static LocalPlayer* localPlayer = (LocalPlayer*)(clientModule + hazedumper::signatures::dwLocalPlayer);

	return localPlayer;
}

int* LocalPlayer::getHealth() {
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iHealth);
}

int* LocalPlayer::getTeam() {
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iTeamNum);
}

Vector3* LocalPlayer::getOrigin() {
	return (Vector3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin);
}

Vector3* LocalPlayer::getViewOffset() {
	return (Vector3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecViewOffset);
}

float LocalPlayer::getDistance(Vector3* other) {
	Vector3* localPlayerPos = getOrigin();
	Vector3 delta = Vector3(other->x - localPlayerPos->x, other->y - localPlayerPos->y, other->z - localPlayerPos->z);

	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

double PI = 3.14159265358;

void LocalPlayer::aimAt(Vector3* target) {
	static uint32_t engineModule = (uint32_t)GetModuleHandle("engine.dll");
	static Vector3* viewAngles = (Vector3*)(*(uint32_t*)(engineModule + hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_ViewAngles);

	Vector3 origin = *getOrigin();
	Vector3 viewOffset = *getViewOffset();
	Vector3* localPlayerPosition = &(origin + viewOffset);

	Vector3 deltaVec = { target->x - localPlayerPosition->x, target->y - localPlayerPosition->y, target->z - localPlayerPosition->z };
	float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	float pitch = -asin(deltaVec.z / deltaVecLength)* (180 / PI);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);

	float smoothPitch = viewAngles->x + (pitch - viewAngles->x) / 20;
	float smoothYaw = viewAngles->y + (yaw - viewAngles->y) / 20;

	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) {
		viewAngles->x = smoothPitch;
		viewAngles->y = smoothYaw;
	}
}