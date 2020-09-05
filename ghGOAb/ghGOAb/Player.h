#pragma once

class Vector3;
class Player {
private:
	Player();

public:
	static int* getMaxPlayer();
	static Player* getPlayer(int index);

	int* getHealth();
	int* getTeam();
	Vector3* getOrigin();
	Vector3* getViewOffset();
	Vector3* getBonePos(int boneId);
};