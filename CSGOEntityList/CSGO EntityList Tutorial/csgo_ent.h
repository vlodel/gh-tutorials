#pragma once
#include <Windows.h>
#include <cstdint>
//https://guidedhacking.com/threads/reverse-engineering-how-to-find-the-csgo-entity-list.13313/

class clientInfo
{
public:
	class ent* entptr; //0x0000
	int32_t N00000262; //0x0004
	class clientInfo* blink; //0x0008
	class clientInfo* flink; //0x000C
}; //Size: 0x0010

class CBaseEntityList
{
public:
	char pad_0000[16]; //0x0000
	class clientInfo entList[64]; //0x0010
}; //Size: 0x0410

class ent
{
public:
	char pad_0000[256]; //0x0000
	int32_t health; //0x0100
}; //Size: 0x0104