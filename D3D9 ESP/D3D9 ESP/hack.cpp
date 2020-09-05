#include "includes.h"

Hack::~Hack() {
	this->FontF->Release();
	this->LineL->Release();
}

void Hack::Init() {
	client = (uintptr_t)GetModuleHandle(L"client.dll");
	engine = (uintptr_t)GetModuleHandle(L"engine.dll");
	entList = (EntList*)(client + dwEntityList);
	localEnt = entList->ents[0].ent;
}

void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE*)(client + dwViewMatrix), sizeof(viewMatrix));
	this->CheckButtons();
}

void Hack::CheckButtons() {
	if (GetAsyncKeyState(this->button.showMenuBtn) & 1) {
		this->settings.showMenu = !this->settings.showMenu;
	}

	if (GetAsyncKeyState(this->button.showTeammatesBtn) & 1) {
		this->settings.showTeammates = !this->settings.showTeammates;
	}

	if (GetAsyncKeyState(this->button.snaplineBtn) & 1) {
		this->settings.snapline = !this->settings.snapline;
	}

	if (GetAsyncKeyState(this->button.box2DBtn) & 1) {
		this->settings.box2D = !this->settings.box2D;
	}

	if (GetAsyncKeyState(this->button.status2DBtn) & 1) {
		this->settings.status2D = !this->settings.status2D;
	}

	if (GetAsyncKeyState(this->button.statusTextBtn) & 1) {
		this->settings.statusText = !this->settings.statusText;
	}

	if (GetAsyncKeyState(this->button.box3DBtn) & 1) {
		this->settings.box3D = !this->settings.box3D;
	}

	if (GetAsyncKeyState(this->button.headlineEspBtn) & 1) {
		this->settings.headlineEsp = !this->settings.headlineEsp;
	}

	if (GetAsyncKeyState(this->button.rcsCrosshairBtn) & 1) {
		this->settings.rcsCrosshair = !this->settings.rcsCrosshair;
	}
}


bool Hack::CheckValidEnt(Ent* ent) {
	if (ent == nullptr) {
		return false;
	}

	if (ent == localEnt) {
		return false;
	}

	if (ent->iHealth <= 0) {
		return false;
	}

	if (ent->isDormant) {
		return false;
	}

	return true;
}

bool Hack::WorldToScreen(Vec3 pos, Vec2& screen) {
	Vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.w < 0.1f) {
		return false;
	}

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return true;
}

Vec3 Hack::GetBonePos(Ent* ent, int bone) {
	uintptr_t bonePtr = ent->boneMatrix;
	Vec3 bonePos;
	bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);
	bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);
	bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);

	return bonePos;
}

Vec3 Hack::TransformVec(Vec3 src, Vec3 ang, float d) {
	Vec3 newPos;
	newPos.x = src.x + (cosf(TORAD(ang.y)) * d);
	newPos.y = src.y + (sinf(TORAD(ang.y)) * d);
	newPos.z = src.z + (tanf(TORAD(ang.x)) * d);
	return newPos;
}

