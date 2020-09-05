#pragma once

class Vector3;
class LocalPlayer {
private:
	LocalPlayer();

public:
	static LocalPlayer* get();

	Vector3* getOrigin();
	Vector3* getViewOffset();
	int* getHealth();
	int* getTeam();
	void aimAt(Vector3* target);
	float getDistance(Vector3*);
};