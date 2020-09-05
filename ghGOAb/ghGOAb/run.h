#pragma once
#include "Player.h"
#include "LocalPlayer.h"

Player* getClosestEnemy() {
	LocalPlayer* localPlayer = LocalPlayer::get();

	float closestDistance = 1000000;
	int closestDistanceIndex = -1;

	for (int i = 1; i < *Player::getMaxPlayer(); i++) {

		Player* currentPlayer = Player::getPlayer(i);

		if (!currentPlayer || !(*(uint32_t*)currentPlayer) || (uint32_t)currentPlayer == (uint32_t)localPlayer) {
			continue;
		}

		if (*currentPlayer->getTeam() == *localPlayer->getTeam()) {
			continue;
		}

		if (*currentPlayer->getHealth() < 1 || *localPlayer->getHealth() < 1) {
			continue;
		}

		float currentDistance = localPlayer->getDistance(currentPlayer->getOrigin());
		if (currentDistance < closestDistance) {
			closestDistance = currentDistance;
			closestDistanceIndex = i;
		}
	}

	if (closestDistanceIndex == -1) {
		return NULL;
	}

	return Player::getPlayer(closestDistanceIndex);
}

void Run() {
	Player* closestEnemy = getClosestEnemy();

	if (closestEnemy) {
		LocalPlayer::get()->aimAt(closestEnemy->getBonePos(8));
	}
}