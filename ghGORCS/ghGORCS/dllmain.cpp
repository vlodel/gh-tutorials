#include <Windows.h>

//vec3 class
struct Vector3 { 
    float x, y, z;

    Vector3 operator+(Vector3 d) {
        return { x + d.x,y + d.y,z + d.z };
    }

    Vector3 operator-(Vector3 d) {
        return { x - d.x,y - d.y,z - d.z };
    }

    Vector3 operator*(float d) {
        return { x * d,y * d,z * d };
    }

    void normalize() {
        while (y < -180) { y += 360; }
        while (y > 180) { y -= 360; }
        if (x > 89) { x = 89; }
        if (x < -89) { x = -89; }
    }
};

//offsets
uintptr_t dwLocalPlayer = 0xD3FC5C;
uintptr_t dwClientState = 0x58ADD4;
uintptr_t dwClientState_viewAngles = 0x4D88;
uintptr_t m_iShotsFired = 0xA390;
uintptr_t m_aimPunchAngle = 0x302C;

//settings
uintptr_t exitKey = VK_F10;

int WINAPI thread(HMODULE hModule) {

    //data
    //clientModule
    uintptr_t clientModule = (uintptr_t)GetModuleHandle(L"client.dll");
    //engineModule
    uintptr_t engineModule = (uintptr_t)GetModuleHandle(L"engine.dll");
    //localPlayer ptr
    uintptr_t localPlayer = *(uintptr_t*)(clientModule + dwLocalPlayer);
    //viewAngles ptr
    Vector3* viewAngles = (Vector3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_viewAngles);
    //shotsFired ptr
    int* iShotsFired = (int*)(localPlayer + m_iShotsFired);
    //aimPunch ptr
    Vector3* aimPunchAngle = (Vector3*)(localPlayer + m_aimPunchAngle);

    Vector3 oldPunch{ 0,0,0 };

    //loop
    while (!GetAsyncKeyState(exitKey)) {
        Vector3 punchAngle = *aimPunchAngle * 2;
        if (*iShotsFired>1) {
            //calculate rcs
            Vector3 newAngle = *viewAngles + oldPunch - punchAngle;
            //normalize
            newAngle.normalize();
            //set
            *viewAngles = newAngle;
        }
        //fix
        oldPunch = punchAngle;
    }

    FreeLibraryAndExitThread(hModule, 0);
    CloseHandle(hModule);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    if (ul_reason_for_call==DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)thread, hModule, 0, 0);
    }
    return TRUE;
}

