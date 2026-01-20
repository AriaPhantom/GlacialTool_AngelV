#include "stdafx.h"
#include "gMonitor.h"

void gMonitor::setPlayerCoords(int x, int y) {
	currentPlayerCoords[0] = x;
	currentPlayerCoords[1] = y;
}
int* gMonitor::getPlayerCoords() {
	return currentPlayerCoords;
}

void gMonitor::setWhiteIconCoords(int x, int y) {
	whiteIconCoords[0] = x;
	whiteIconCoords[1] = y;
}
int* gMonitor::getWhiteIconCoords() {
	return whiteIconCoords;
}

void gMonitor::setWhiteIconUpdate(int x) {
	whiteIconUpdate = x;
}

void gMonitor::setPortalclick(int num) {
	portalclick = num;
}
void gMonitor::reducePortalclick() {
	portalclick--;
}

int gMonitor::getPortalclick() {
	return portalclick;
}

void gMonitor::setRunecapture(int num) {
	runecapture = num;
}

void gMonitor::setRuneclick(int num) {
	runeclick = num;
}
void gMonitor::reduceRuneclick() {
	runeclick--;
}
int gMonitor::getRuneclick() {
	return runeclick;
}

void gMonitor::setRuneCoords(int x, int y) {
	runeCoords[0] = x;
	runeCoords[1] = y;
}
int* gMonitor::getRuneCoords() {
	return runeCoords;
}

void gMonitor::setRandomPlayerInMap(int val) {
	randomPlayerInMap = val;
}
int gMonitor::isRandomPlayerInMap() {
	return randomPlayerInMap;
}

void gMonitor::setRandomPlayerTimer(long val) {
	randomPlayerTimer = val;
}

long gMonitor::getRandomPlayerTimer() {
	return randomPlayerTimer;
}

void gMonitor::setFriendPlayerInMap(int val) {
	friendPlayerInMap = val;
}

int gMonitor::isFriendPlayerInMap() {
	return friendPlayerInMap;
}

void gMonitor::setFriendPlayerTimer(long val) {
	friendPlayerTimer = val;
}

long gMonitor::getFriendPlayerTimer() {
	return friendPlayerTimer;
}

