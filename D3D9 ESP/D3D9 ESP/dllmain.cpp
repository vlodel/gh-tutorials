#include "includes.h"
#include <sstream>
#include <string.h>

//data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene originalEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;


//hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice) {
		pDevice = o_pDevice;
	}

	//drawing stuff
	DrawText("Vlodel mi-l sugi", windowWidth / 2, windowHeight - 20, D3DCOLOR_ARGB(255, 255, 255, 255));

	int menuOffX = windowWidth / 2;
	int menuOffY = 50;
	D3DCOLOR enabled = D3DCOLOR_ARGB(255, 0, 255, 0);
	D3DCOLOR disabled = D3DCOLOR_ARGB(255, 255, 0, 0);

	if (hack->settings.showMenu) {
		DrawText("Show Menu (INSERT)", menuOffX, menuOffY, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else {


		DrawText("Show Teammates (F1)", menuOffX, menuOffY + 0 * 12, hack->settings.showTeammates ? enabled : disabled);
		DrawText("Snaplines (F2)", menuOffX, menuOffY + 1 * 12, hack->settings.snapline ? enabled : disabled);
		DrawText("2D Box (F3)", menuOffX, menuOffY + 2 * 12, hack->settings.box2D ? enabled : disabled);
		DrawText("2D Status Bars (F4)", menuOffX, menuOffY + 3 * 12, hack->settings.status2D ? enabled : disabled);
		DrawText("Status text (F5)", menuOffX, menuOffY + 4 * 12, hack->settings.statusText ? enabled : disabled);
		DrawText("3D Box (F6)", menuOffX, menuOffY + 5 * 12, hack->settings.box3D ? enabled : disabled);
		DrawText("Headline ESP (F7)", menuOffX, menuOffY + 6 * 12, hack->settings.headlineEsp ? enabled : disabled);
		DrawText("Recoil crosshair (F8)", menuOffX, menuOffY + 7 * 12, hack->settings.rcsCrosshair ? enabled : disabled);

		DrawText("Hide Menu (INS)", menuOffX, menuOffY + 8 * 12, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	


	for (int i = 1; i < 32; i++) {
		Ent* currEnt = hack->entList->ents[i].ent;
		if (!hack->CheckValidEnt(currEnt)) {
			continue;
		}

		D3DCOLOR espColor, snaplineColor, headlineColor;
		if (currEnt->iTeamNum == hack->localEnt->iTeamNum) {
			espColor = hack->color.team.esp;
			snaplineColor = hack->color.team.snapline;
			headlineColor = hack->color.team.headline;
		}
		else {
			espColor = hack->color.enemy.esp;
			snaplineColor = hack->color.enemy.snapline;
			headlineColor = hack->color.enemy.headline;
		}
	

		if (hack->settings.showTeammates && (currEnt->iTeamNum == hack->localEnt->iTeamNum))
			continue;


		Vec3 entHead3D = hack->GetBonePos(currEnt, 8);
		entHead3D.z += 8;
		Vec2 entPos2D, entHead2D;

		if (hack->WorldToScreen(currEnt->vecOrigin, entPos2D)) {

			if (hack->settings.snapline) {
				DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, snaplineColor);
			}

			if (hack->WorldToScreen(entHead3D, entHead2D)) {

				if (hack->settings.box2D) {
					DrawEspBox2D(entPos2D, entHead2D, 2, espColor);
				}

				if (hack->settings.box3D) {
					DrawEspBox3D(entHead3D, currEnt->vecOrigin, currEnt->angEyeAnglesY, 25, 2, espColor);
				}

				if (hack->settings.status2D) {
					int height = ABS(entPos2D.y - entHead2D.y);
					int dX = (entPos2D.x - entHead2D.x);

					float healthPercentage = currEnt->iHealth / 100.f;
					float armorPercentage = currEnt->armorValue / 100.f;

					Vec2 botHealth, topHealth, botArmor, topArmor;
					int healthHeight = height * healthPercentage;
					int armorHeight = height * armorPercentage;

					botHealth.x = entPos2D.x - (height / 4) - 2;
					botArmor.x = entPos2D.x - (height / 4) - 5;

					botHealth.y = botArmor.y = entPos2D.y;

					topHealth.x = entPos2D.x - (height / 4) - 2 - (dX * healthPercentage);
					topArmor.x = entPos2D.x - (height / 4) - 5 - (dX * armorPercentage);

					topHealth.y = entHead2D.y + height - healthHeight;
					topArmor.y = entHead2D.y + height - armorHeight;

					DrawLine(botHealth, topHealth, 2, hack->color.health);
					DrawLine(botArmor, topArmor, 2, hack->color.armor);
				}

				if (hack->settings.headlineEsp) {
					Vec3 head3D = hack->GetBonePos(currEnt, 8);
					Vec3 entAngles;
					entAngles.x = currEnt->angEyeAnglesX;
					entAngles.y = currEnt->angEyeAnglesY;
					entAngles.z = 0;
					Vec3 endPoint = hack->TransformVec(head3D, entAngles, 60);
					
					Vec2 endPoint2D, head2D;
					hack->WorldToScreen(head3D, head2D);
					if (hack->WorldToScreen(endPoint, endPoint2D)) {
						DrawLine(head2D, endPoint2D, 2, headlineColor);
					}
				}

				if (hack->settings.statusText) {
					std::stringstream s1, s2;
					s1 << currEnt->iHealth;
					s2 << currEnt->armorValue;
					std::string t1 = "hp: " + s1.str();
					std::string t2 = "ap: " + s2.str();
					char* healthMsg = (char*)t1.c_str();
					char* armorMsg = (char*)t2.c_str();

					DrawText(healthMsg, entPos2D.x, entPos2D.y, hack->color.crosshair);
					DrawText(armorMsg, entPos2D.x, entPos2D.y + 12, hack->color.crosshair);

					if (!currEnt->bHasHelmet) {
						DrawText("ez hs", entPos2D.x, entPos2D.y + 12, D3DCOLOR_ARGB(255, 255, 255, 255));

					}
				}
			}
		}
	}

	if (hack->settings.rcsCrosshair) {
		Vec2 l, r, t, b;
		l = r = t = b = hack->crosshair2D;
		l.x -= hack->crosshairSize;
		r.x += hack->crosshairSize;
		t.y -= hack->crosshairSize;
		b.y += hack->crosshairSize;

		DrawLine(l, r, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
		DrawLine(t, b, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
	}



	originalEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {

	//hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		originalEndScene = (tEndScene)TrampHook((char*)d3d9Device[42],(char*)hkEndScene,7);
	}

	hack = new Hack();
	hack->Init();

	//hack loop
	while (!GetAsyncKeyState(VK_F10)) {
		hack->Update();

		Vec3 pAngle = hack->localEnt->aimPunchAngle;

		hack->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * pAngle.y);
		hack->crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * pAngle.x);
	}

	//unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	Sleep(1000);

	//uninject
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH) {
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	}

	return TRUE;
}