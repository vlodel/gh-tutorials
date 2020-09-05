#pragma once
#include <Windows.h>
#include <cstdio>
#include <cstdint>

// Created with ReClass.NET 1.2 by KN4CK3R

struct vec3 { float x, y, z; };
using Vector3 = vec3;

class entity
{
public:
	char pad_0000[4]; //0x0000
	Vector3 headPos; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 position; //0x0034
	Vector3 angles; //0x0040
	char pad_004C[172]; //0x004C
	int32_t health; //0x00F8
	char pad_00FC[296]; //0x00FC
	int8_t bAttack; //0x0224
	char pad_0225[263]; //0x0225
	int32_t team; //0x032C
	char pad_0330[68]; //0x0330
	class weapon* currentWeapon; //0x0374
	char pad_0378[1248]; //0x0378
}; //Size: 0x0858
static_assert(sizeof(entity) == 0x858);

class weapon
{
public:
	char pad_0000[4]; //0x0000
	int32_t weaponId; //0x0004
	class entity* owner; //0x0008
	char pad_000C[4]; //0x000C
	class ammoPointer* ammoReserve; //0x0010
	class ammoPointer* ammoClip; //0x0014
	char pad_0018[44]; //0x0018
}; //Size: 0x0044
static_assert(sizeof(weapon) == 0x44);

class ammoPointer
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof(ammoPointer) == 0x4);