#include "stdafx.h"

#include "boHandler.h"

#include "AutoLogin.h"

#include "obj.h"

#include "thread_control.h"

#include "gMonitor.h"

#include "runeData.h"

#include "lodepng.h"

#include "MiaoSender.h"

#include "script.h"

#include "RuAdvancing.h"

#include "FileProc.h"

#include "SPUtils.h"

#include <sstream>

#include <iostream>

#include <string>

#include <map>

#include <mutex>

#include <vector>



namespace {
std::wstring Utf8ToWide(const char* value) {
	if (value == nullptr || *value == '\0') {
		return std::wstring();
	}

	int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value, -1, nullptr, 0);
	if (length <= 0) {
		length = MultiByteToWideChar(CP_UTF8, 0, value, -1, nullptr, 0);
	}
	if (length <= 0) {
		return std::wstring();
	}

	std::wstring result(static_cast<size_t>(length), L'\0');
	if (MultiByteToWideChar(CP_UTF8, 0, value, -1, &result[0], length) <= 0) {
		return std::wstring();
	}
	result.resize(static_cast<size_t>(length - 1));
	return result;
}
}

void keyDown(long index, const char* key) {
	const std::wstring converted = Utf8ToWide(key);
	keyDown(index, converted.c_str());
}

void keyUp(long index, const char* key) {
	const std::wstring converted = Utf8ToWide(key);
	keyUp(index, converted.c_str());
}

void holdKey(long index, const char* key, long holdTime, long interval = 133) {
	const std::wstring converted = Utf8ToWide(key);
	holdKey(index, converted.c_str(), holdTime, interval);
}

void press(long index, const char* key, int times = 1, long delay = 0) {
	const std::wstring converted = Utf8ToWide(key);
	press(index, converted.c_str(), times, delay);
}
gMonitor gMonitorInstance = gMonitor();

MiaoSender miaoSenderInstance = MiaoSender();

int rune_fail_time = 0;

int guild_skill_count = 0;

long autoOilCooldownRemainingMs[MAX_HWND] = {};
ULONGLONG autoOilLastOnlineTickMs[MAX_HWND] = {};



long buffTimeOut_buff_备用1 = 0;



long buffTimeOut_buff_钻机 = 0;

long buffTimeOut_buff_飞剑 = 0;

long buffTimeOut_buff_手雷 = 0;

long buffTimeOut_buff_轮回 = 0;

long buffTimeOut_buff_小立场 = 0;

long buffTimeOut_buff_大立场 = 0;

long buffTimeOut_buff_剑雨 = 0;

long buffTimeOut_buff_代码 = 0;

long buffTimeOut_buff_骰子 = 0;

long buffTimeOut_buff_能量补充 = 0;

long buffTimeOut_buff_艾尔达洗澡 = 0;

long buffTimeOut_buff_进阶女神 = 0;

long buffTimeOut_buff_寂静乱舞 = 0;

long buffTimeOut_buff_反抗放置 = 0;

long buffTimeOut_buff_黑球 = 0;

long buffTimeOut_buff_捡钱 = 0;



long buffTimeOut_buff_蜘蛛 = 0;

long buffTimeOut_buff_米特拉 = 0;



long buffTimeOut_buff_MVP = 0;

long buffTimeOut_buff_公园 = 0;

long buffTimeOut_buff_点火 = 0;

long buffTimeOut_buff_汽油 = 0;

long buffTimeOut_buff_经验秘药2h = 0;

long buffTimeOut_buff_经验秘药30 = 0;

long buffTimeOut_buff_经验exp30 = 0;

long buffTimeOut_buff_经验exp10 = 0;



long buffTimeOut_buff_bossChest = 0;



void UpdateCoords(int* nums);



extern int GetautoRuneSolver();

extern int GetfriendPlayerNotification();

extern int Gethunt();

extern int Getmap();

extern int GetEXP_PARK();

extern int Getkuxing();

extern int GethuangMen();

extern int GetautoWealth();

extern int GetExp10();

extern int GetExp30();

extern long GetExpBuffDuration();

extern int GetexpPot();

extern int GetautoOil();

extern int Getignite();

extern int GetWhiteDetect();
extern int GetLieSound();



namespace {

constexpr int kMiniMapTopLeftX = 0;

constexpr int kMiniMapTopLeftY = 0;

constexpr int kMiniMapBottomRightX = 350;

constexpr int kMiniMapBottomRightY = 300;



constexpr int kWhiteRoomTopLeftX = 0;

constexpr int kWhiteRoomTopLeftY = 0;

constexpr int kWhiteRoomBottomRightX = 350;

constexpr int kWhiteRoomBottomRightY = 300;constexpr ULONGLONG kOtherPlayerCheckIntervalMs = 500;

constexpr ULONGLONG kFriendGuildCheckIntervalMs = 500;

constexpr ULONGLONG kHuangmenCheckIntervalMs = 1000;

constexpr ULONGLONG kBossCheckIntervalMs = 2000;

constexpr ULONGLONG kRuneCheckIntervalMs = 3000;

constexpr ULONGLONG kPlayerCoordCheckIntervalMs = 50;

constexpr ULONGLONG kPlayerCoordCheckIdleIntervalMs = 250;

constexpr ULONGLONG kPlayerCoordHintTtlMs = 2000;
constexpr int kPlayerCoordRoiHalfSize = 48;
constexpr int kPlayerCoordRoiTemplatePad = 16;

constexpr ULONGLONG kWhiteCheckIntervalMs = 200;
constexpr ULONGLONG kLieCheckIntervalMs = 1000;

// External image mode cadence; Lie remains at 1000ms to preserve detection latency.
constexpr ULONGLONG kPlayerCoordCheckIntervalExtMs = 50;
constexpr ULONGLONG kPlayerCoordCheckIdleExtMs = 300;
constexpr ULONGLONG kWhiteCheckIntervalExtMs = 350;

ULONGLONG g_lastOtherPlayerCheckMs[MAX_HWND] = {};

ULONGLONG g_lastFriendGuildCheckMs[MAX_HWND] = {};

ULONGLONG g_lastHuangmenCheckMs[MAX_HWND] = {};

ULONGLONG g_lastBossCheckMs[MAX_HWND] = {};

ULONGLONG g_lastRuneCheckMs[MAX_HWND] = {};

ULONGLONG g_lastPlayerCoordCheckMs[MAX_HWND] = {};

ULONGLONG g_lastWhiteCheckMs[MAX_HWND] = {};
ULONGLONG g_lastLieCheckMs[MAX_HWND] = {};

int g_lastPlayerMatchX[MAX_HWND] = {};
int g_lastPlayerMatchY[MAX_HWND] = {};
ULONGLONG g_lastPlayerMatchTickMs[MAX_HWND] = {};
HWND g_lastPlayerMatchHwnd[MAX_HWND] = {};
long g_lastPlayerMatchPid[MAX_HWND] = {};

volatile LONG g_isGoToActive[MAX_HWND] = {};




std::wstring ToWideStringLocal(const TCHAR* value) {

	if (value == nullptr) return std::wstring();

#ifdef UNICODE

	return std::wstring(value);

#else

	int size = MultiByteToWideChar(CP_ACP, 0, value, -1, nullptr, 0);

	if (size <= 1) return std::wstring();

	std::wstring result(static_cast<size_t>(size - 1), L'\0');

	MultiByteToWideChar(CP_ACP, 0, value, -1, &result[0], size);

	return result;

#endif

}












bool CaptureMiniMapSnapshot(long index, SPUtils::ImageSnapshot& outSnapshot) {
	outSnapshot = SPUtils::ImageSnapshot{};

	HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[index].hwnd));
	if (!hwnd || !IsWindow(hwnd)) return false;

	return SPUtils::CaptureSnapshot(hwnd,
		kMiniMapTopLeftX, kMiniMapTopLeftY, kMiniMapBottomRightX, kMiniMapBottomRightY, outSnapshot);
}



bool FindIconInMiniMapSnapshot(const SPUtils::ImageSnapshot& snapshot, const TCHAR* iconPathList, double sim, int& outX, int& outY) {
	outX = -1;
	outY = -1;

	long matchX = -1;
	long matchY = -1;
	if (!SPUtils::FindPicInSnapshot(snapshot, ToWideStringLocal(iconPathList), sim, matchX, matchY)) {
		return false;
	}

	outX = static_cast<int>(matchX);
	outY = static_cast<int>(matchY);
	return true;
}

bool FindPlayerIconInMiniMapSnapshot(const SPUtils::ImageSnapshot& snapshot, long mainIndex,
	HWND hwnd, long pid, ULONGLONG nowMs, double sim, int& outX, int& outY) {
	outX = -1;
	outY = -1;
	if (!snapshot.IsValid()) return false;

	long matchX = -1;
	long matchY = -1;
	bool matched = false;
	if (mainIndex >= 0 && mainIndex < MAX_HWND &&
		g_lastPlayerMatchTickMs[mainIndex] != 0 &&
		nowMs >= g_lastPlayerMatchTickMs[mainIndex] &&
		nowMs - g_lastPlayerMatchTickMs[mainIndex] <= kPlayerCoordHintTtlMs &&
		g_lastPlayerMatchHwnd[mainIndex] == hwnd &&
		g_lastPlayerMatchPid[mainIndex] == pid) {
		const int roiX = g_lastPlayerMatchX[mainIndex] - kPlayerCoordRoiHalfSize;
		const int roiY = g_lastPlayerMatchY[mainIndex] - kPlayerCoordRoiHalfSize;
		const int roiSize = kPlayerCoordRoiHalfSize * 2 + kPlayerCoordRoiTemplatePad;
		matched = SPUtils::FindPicInSnapshotRegion(snapshot, ToWideStringLocal(playerIcon), sim,
			roiX, roiY, roiSize, roiSize, matchX, matchY);
	}

	if (!matched) {
		matched = SPUtils::FindPicInSnapshot(snapshot, ToWideStringLocal(playerIcon), sim, matchX, matchY);
	}
	if (!matched) return false;

	outX = static_cast<int>(matchX);
	outY = static_cast<int>(matchY);
	if (mainIndex >= 0 && mainIndex < MAX_HWND) {
		g_lastPlayerMatchX[mainIndex] = outX;
		g_lastPlayerMatchY[mainIndex] = outY;
		g_lastPlayerMatchTickMs[mainIndex] = nowMs;
		g_lastPlayerMatchHwnd[mainIndex] = hwnd;
		g_lastPlayerMatchPid[mainIndex] = pid;
	}
	return true;
}

int NormalizeMonitorMainIndex(long index) {

	if (index >= MAX_HWND * 2) index -= MAX_HWND * 2;

	if (index >= MAX_HWND) index -= MAX_HWND;

	if (index < 0 || index >= MAX_HWND) return -1;

	return static_cast<int>(index);

}

bool ShouldRunMonitorPeriodicCheck(int mainIndex, ULONGLONG* lastCheckMsArray, ULONGLONG intervalMs, ULONGLONG nowMs) {

	if (mainIndex < 0 || mainIndex >= MAX_HWND || lastCheckMsArray == nullptr) return true;

	ULONGLONG& lastMs = lastCheckMsArray[mainIndex];

	if (lastMs == 0 || nowMs < lastMs || (nowMs - lastMs) >= intervalMs) {

		lastMs = nowMs;

		return true;

	}

	return false;

}

} // namespace

void clickRune(long index);



void UpdateAutoOilCooldown(long index, bool isOnline) {
	int mainIndex = NormalizeMonitorMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;

	if (!isOnline) {
		autoOilLastOnlineTickMs[mainIndex] = 0;
		return;
	}

	ULONGLONG nowMs = GetTickCount64();
	ULONGLONG lastMs = autoOilLastOnlineTickMs[mainIndex];
	autoOilLastOnlineTickMs[mainIndex] = nowMs;
	if (lastMs == 0 || nowMs <= lastMs) {
		return;
	}

	long& remainingMs = autoOilCooldownRemainingMs[mainIndex];
	if (remainingMs <= 0) {
		remainingMs = 0;
		return;
	}

	ULONGLONG elapsedMs = nowMs - lastMs;
	if (elapsedMs >= static_cast<ULONGLONG>(remainingMs)) {
		remainingMs = 0;
	}
	else {
		remainingMs -= static_cast<long>(elapsedMs);
	}
}

bool IsAutoOilReady(long index) {
	int mainIndex = NormalizeMonitorMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return true;
	return autoOilCooldownRemainingMs[mainIndex] <= 0;
}

void ArmAutoOilCooldown(long index, long cooldownMs) {
	int mainIndex = NormalizeMonitorMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
	autoOilCooldownRemainingMs[mainIndex] = cooldownMs > 0 ? cooldownMs : 0;
	autoOilLastOnlineTickMs[mainIndex] = GetTickCount64();
}

void subSoftStartPause() {

	gMonitorInstance.switchStatus();

	buffTimeOut_buff_轮回 = 0;

}



void subSoftPause() {

	gMonitorInstance.stopStatus();

}



void subSoftStart() {

	gMonitorInstance.startStatus();

	detectionStart();

}



void detectionStart() {

	gMonitorInstance.detectionStatus = 1;

}



void detectWhiteIcon() {

	gMonitorInstance.whiteIconUpdate = 1;

}



void startMiao(long index) {

	sptool* dm = g_info[index].dm;

	unsigned long currentTime;

	int send_status = 0;



	while (1) {

		currentTime = dm->GetTime();

		if (miaoSenderInstance.send_boss > 0) {

			if (currentTime - miaoSenderInstance.cooldown_boss >= 10000) {

				send_status = sendMiaoCodeByType(miaoSenderInstance.MiaoCode_boss, "boss", "boss alert");

				if (send_status == 200) {

					miaoSenderInstance.send_boss--;

					miaoSenderInstance.cooldown_boss = dm->GetTime();

				}

				ScriptDelay(index, 3000);

			}

		}

		if (miaoSenderInstance.send_others > 0) {

			if (currentTime - miaoSenderInstance.cooldown_others >= 10000) {

				send_status = sendMiaoCodeByType(miaoSenderInstance.MiaoCode_others, "others", "others alert");

				if (send_status == 200) {

					miaoSenderInstance.send_others--;

					miaoSenderInstance.cooldown_others = dm->GetTime();

				}

				ScriptDelay(index, 3000);

			}

		}

		if (miaoSenderInstance.send_white > 0) {

			if (currentTime - miaoSenderInstance.cooldown_white >= 10000) {

				send_status = sendMiaoCodeByType(miaoSenderInstance.MiaoCode_white, "white", "white alert");

				if (send_status == 200) {

					miaoSenderInstance.send_white--;

					miaoSenderInstance.cooldown_white = dm->GetTime();

				}

				ScriptDelay(index, 3000);

			}

		}

		if (miaoSenderInstance.send_huangmen > 0) {

			if (currentTime - miaoSenderInstance.cooldown_huangmen >= 10000) {

				send_status = sendMiaoCodeByType(miaoSenderInstance.Miaocode_huangmen, "huangmen", "huangmen alert");

				if (send_status == 200) {

					miaoSenderInstance.send_huangmen--;

					miaoSenderInstance.cooldown_huangmen = dm->GetTime();

				}

				ScriptDelay(index, 3000);

			}

		}

		ScriptDelay(index, 200);

	}

}



void gMonitorCheck(long index, long count)

{

	sptool* dm = g_info[index].dm;

	if (dm == nullptr || dm->IsBind(g_info[index].hwnd) != 1) return;

	if (Gethunt() != 1 || gMonitorInstance.detectionStatus == 0)

	{

		ScriptDelay(index, 200);

		return;

	}



	ULONGLONG nowMs = GetTickCount64();

	int mainIndex = NormalizeMonitorMainIndex(index);

	bool runOtherPlayerCheck = ShouldRunMonitorPeriodicCheck(mainIndex, g_lastOtherPlayerCheckMs, kOtherPlayerCheckIntervalMs, nowMs);

	bool runFriendGuildCheck = ShouldRunMonitorPeriodicCheck(mainIndex, g_lastFriendGuildCheckMs, kFriendGuildCheckIntervalMs, nowMs);

	bool runHuangmenCheck = ShouldRunMonitorPeriodicCheck(mainIndex, g_lastHuangmenCheckMs, kHuangmenCheckIntervalMs, nowMs);

	bool runBossCheck = ShouldRunMonitorPeriodicCheck(mainIndex, g_lastBossCheckMs, kBossCheckIntervalMs, nowMs);

	bool runRuneCheck = ShouldRunMonitorPeriodicCheck(mainIndex, g_lastRuneCheckMs, kRuneCheckIntervalMs, nowMs);

	ULONGLONG playerCoordIntervalMs = SPUtils::SelectImageModeIntervalMs(kPlayerCoordCheckIdleIntervalMs, kPlayerCoordCheckIdleExtMs);

	if (mainIndex >= 0 && mainIndex < MAX_HWND && InterlockedCompareExchange(&g_isGoToActive[mainIndex], 0, 0) != 0) {

		playerCoordIntervalMs = SPUtils::SelectImageModeIntervalMs(kPlayerCoordCheckIntervalMs, kPlayerCoordCheckIntervalExtMs);

	}

	bool runPlayerCoordCheck = ShouldRunMonitorPeriodicCheck(mainIndex, g_lastPlayerCoordCheckMs, playerCoordIntervalMs, nowMs);

	bool runWhiteCheck = ShouldRunMonitorPeriodicCheck(mainIndex, g_lastWhiteCheckMs, SPUtils::SelectImageModeIntervalMs(kWhiteCheckIntervalMs, kWhiteCheckIntervalExtMs), nowMs);
	bool runLieCheck = ShouldRunMonitorPeriodicCheck(mainIndex, g_lastLieCheckMs, kLieCheckIntervalMs, nowMs);



	const bool autoRuneEnabled = (GetautoRuneSolver() == 1);

	const bool huangmenEnabled = (GethuangMen() != 0);

	const bool friendGuildEnabled = (GetfriendPlayerNotification() != 0);

	const bool whiteDetectEnabled = (GetWhiteDetect() != 0);



	SPUtils::ImageSnapshot miniMapSnapshot;

	const bool needMiniMapSnapshot =

		runPlayerCoordCheck ||

		runOtherPlayerCheck ||

		(autoRuneEnabled && runRuneCheck && *gMonitorInstance.getRuneCoords() == -1) ||

		(huangmenEnabled && runHuangmenCheck) ||

		(friendGuildEnabled && runFriendGuildCheck) ||

		(whiteDetectEnabled && runWhiteCheck);

	if (needMiniMapSnapshot) {

		CaptureMiniMapSnapshot(index, miniMapSnapshot);

	}



	auto findMiniMapIcon = [&](const TCHAR* iconPath, double sim, int& outX, int& outY) -> bool {

		if (miniMapSnapshot.IsValid()) {

			return FindIconInMiniMapSnapshot(miniMapSnapshot, iconPath, sim, outX, outY);

		}

		int* coords = findCoordsOnMiniMap(index, iconPath, sim);

		outX = coords[0];

		outY = coords[1];

		return (outX > 0 && outY > 0);

	};



	long x, y;

	long borderx, bordery;

	long findPicRet;

	auto detectWhiteRoom = [&](long& outWhiteX, long& outWhiteY) -> bool {

		if (miniMapSnapshot.IsValid()) {

			int sx = -1;

			int sy = -1;

			if (FindIconInMiniMapSnapshot(miniMapSnapshot, whiteIcon, 0.9, sx, sy)) {

				outWhiteX = sx;

				outWhiteY = sy;

				return true;

			}

			outWhiteX = -1;

			outWhiteY = -1;

			return false;

		}

		findPicRet = dm->FindPic(kWhiteRoomTopLeftX, kWhiteRoomTopLeftY,

			kWhiteRoomBottomRightX, kWhiteRoomBottomRightY,

			whiteIcon, _T("000000"), 0.9, 0, &outWhiteX, &outWhiteY);

		return outWhiteX > 0 && outWhiteY > 0;

	};



	if (gMonitorInstance.whiteIconUpdate > 0) {

		long whiteTopLeftX = 0;

		long whiteTopLeftY = 0;

		long whiteBottomRightX = whiteTopLeftX + 500;

		long whiteBottomRightY = 600;



		int detect_sum = 0;

		findPicRet = dm->FindPic(whiteTopLeftX, whiteTopLeftY, whiteBottomRightX, whiteBottomRightY, whiteIcon, _T("000000"), 0.9, 0, &x, &y);

		if (x > 0 && y > 0) {

			gMonitorInstance.setWhiteIconCoords(x, y);

			detect_sum++;



			findPicRet = dm->FindPic(whiteTopLeftX, y + 60, whiteBottomRightX, whiteBottomRightY, mapBorderIcon, _T("000000"), 0.9, 0, &borderx, &bordery);



			if (borderx > 0 && bordery > 0) {

				gMonitorInstance.mapBorderCoords[0] = borderx + 3;

				gMonitorInstance.mapBorderCoords[1] = bordery + 3;

				detect_sum++;

			}

		}



		if (detect_sum == 2)

		{

			gMonitorInstance.whiteIconUpdate = 0;

			SetTaskState(index - MAX_HWND, _T("地图边界已更新"));

		}

		else {

			SetTaskState(index - MAX_HWND, _T("地图边界更新失败"));

		}

	}



	if (runPlayerCoordCheck) {

		int playerX = -1;

		int playerY = -1;

		HWND playerHwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[index].hwnd));
		long playerPid = g_info[index].pid;
		bool playerFound = false;
		if (miniMapSnapshot.IsValid()) {
			playerFound = FindPlayerIconInMiniMapSnapshot(miniMapSnapshot, mainIndex, playerHwnd, playerPid, nowMs,
				0.95, playerX, playerY);
		}
		else {
			playerFound = findMiniMapIcon(playerIcon, 0.95, playerX, playerY);
		}
		if (playerFound) {

			int* currentWhiteIconCoords = gMonitorInstance.getWhiteIconCoords();

			int relativePlayerLocationX = playerX;

			int relativePlayerLocationY = playerY - 3 - *(currentWhiteIconCoords + 1);

			int relativePlayerLocation[2] = { relativePlayerLocationX , relativePlayerLocationY };

			gMonitorInstance.setPlayerCoords(relativePlayerLocationX, relativePlayerLocationY);

			UpdateCoords(relativePlayerLocation);

		}

	}



	if (runOtherPlayerCheck) {

		int randomX = -1;

		int randomY = -1;

		if (findMiniMapIcon(randomIcon, 0.95, randomX, randomY)) {

			if (gMonitorInstance.randomPlayerOldPosX != randomX && gMonitorInstance.randomPlayerOldPosY != randomY) {

				gMonitorInstance.randomPlayerOldPosX = randomX;

				gMonitorInstance.randomPlayerOldPosY = randomY;

				gMonitorInstance.randomPlayerSamePosTimer = dm->GetTime();

			}



			if (gMonitorInstance.getRandomPlayerTimer() == 0) {

				gMonitorInstance.setRandomPlayerTimer(dm->GetTime());

			}

			else {

				if (dm->GetTime() - RANDOM_PLAYER_NOTIFICATION > gMonitorInstance.getRandomPlayerTimer() &&

					(dm->GetTime() - 300000) < gMonitorInstance.randomPlayerSamePosTimer) {

					gMonitorInstance.setRandomPlayerInMap(1);

					miaoSenderInstance.setOthers(1);

				}

			}

		}

		else {

			gMonitorInstance.setRandomPlayerTimer(0);

			gMonitorInstance.setRandomPlayerInMap(0);

			gMonitorInstance.randomPlayerSamePosTimer = dm->GetTime();

			gMonitorInstance.randomPlayerOldPosX = -1;

			gMonitorInstance.randomPlayerOldPosY = -1;

		}

	}



	if (autoRuneEnabled && runRuneCheck && *gMonitorInstance.getRuneCoords() == -1) {

		int runeX = -1;

		int runeY = -1;

		if (findMiniMapIcon(runeIcon, 0.999, runeX, runeY) &&

			runeX < gMonitorInstance.mapBorderCoords[0] &&

			runeY < gMonitorInstance.mapBorderCoords[1]) {

			int* currentWhiteIconCoords = gMonitorInstance.getWhiteIconCoords();

			int relativeRuneLocationX = runeX - 2;

			int relativeRuneLocationY = runeY - 2 - *(currentWhiteIconCoords + 1);

			if (relativeRuneLocationY > 55) {

				gMonitorInstance.setRuneCoords(relativeRuneLocationX, relativeRuneLocationY);

			}

		}

	}



	if (runBossCheck) {

		long topLeftX, topLeftY, bottomRightX, bottomRightY;

		dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);

		long mapleWindowWidth = bottomRightX - topLeftX;

		long mapleWindowHeight = bottomRightY - topLeftY;

		if (mapleWindowWidth > 1400) {

			mapleWindowWidth = long(mapleWindowWidth / 2);

			mapleWindowHeight = long(mapleWindowHeight / 2);

		}



		long BOSS_WIDTH = 200;

		long bossTopLeftX = long((mapleWindowWidth / 2) - (BOSS_WIDTH / 2));

		long bossTopLeftY = long(20 + (mapleWindowHeight / 2));

		long bossBottomRightX = long((mapleWindowWidth / 2) + (BOSS_WIDTH / 2));

		long bossBottomRightY = long(120 + (mapleWindowHeight / 2));



		findPicRet = dm->FindPic(bossTopLeftX, bossTopLeftY, bossBottomRightX, bossBottomRightY, deadOKIcon, _T("000000"), 0.95, 0, &x, &y);

		if (x > 0 && y > 0) {

			Log(_T("????"));

			miaoSenderInstance.setBoss(1);

			subSoftPause();

		}

	}



	if (whiteDetectEnabled && runWhiteCheck) {

		long loginPendingMs = AutoLogin_GetLoginPendingMs(index);

		if (loginPendingMs > 0 && loginPendingMs < 600000)

		{

			if (detectWhiteRoom(x, y))

			{

				gMonitorInstance.setWhiteTimer(0);

				AutoLogin_ClearLoginPending(index);

			}

		}

		if (loginPendingMs <= 0 || loginPendingMs >= 600000)

		{

			if (detectWhiteRoom(x, y)) {

				gMonitorInstance.setWhiteTimer(0);

			}

			else if (1) {

				if (gMonitorInstance.getWhiteTimer() == 0) {

					gMonitorInstance.setWhiteTimer(dm->GetTime());

				}

				else {

					if (dm->GetTime() - 1000 > gMonitorInstance.getWhiteTimer()) {

						Log(_T("in white"));

						miaoSenderInstance.setWhite(1);

						string s = "C:\\sptool\\WhitePic";

						long a = dm->GetTime() % 10000;

						string s_type = ".png";

						string filePath = s + to_string(a) + s_type;

						CString filePathT(filePath.c_str());

						dm->CapturePng(0, 0, 2000, 1200, filePathT);

					}

					if (dm->GetTime() - WHITE_NOTIFICATION > gMonitorInstance.getWhiteTimer()) {

						subSoftPause();

					}

				}

			}

		}

	}



	if (whiteDetectEnabled && runLieCheck) {
		long topLeftX = 0, topLeftY = 0, bottomRightX = 0, bottomRightY = 0;
		long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);
		if (windowRet != 1) return;
		long mapleWindowWidth = bottomRightX - topLeftX;
		long mapleWindowHeight = bottomRightY - topLeftY;
		if (mapleWindowWidth > 1400) {
			mapleWindowWidth = long(mapleWindowWidth / 2);
			mapleWindowHeight = long(mapleWindowHeight / 2);
		}

		long lieX = -1, lieY = -1;
		long findPicRet = dm->FindPic(0, 0, mapleWindowWidth, mapleWindowHeight, lieIcon, _T("000000"), 0.98, 0, &lieX, &lieY);
		if (findPicRet == 1 && lieX >= 0 && lieY >= 0) {
			Log(_T("detect Lie, trigger white"));
			miaoSenderInstance.setWhite(1);
			if (GetLieSound()) MessageBeep(MB_ICONEXCLAMATION);
			string s = "C:\\sptool\\WhitePic";
			long a = dm->GetTime() % 10000;
			string s_type = ".png";
			string filePath = s + to_string(a) + s_type;
			CString filePathT(filePath.c_str());
			dm->CapturePng(0, 0, 2000, 1200, filePathT);
			subSoftPause();
		}
	}

	if (huangmenEnabled && runHuangmenCheck) {

		int huangmenX = -1;

		int huangmenY = -1;

		if (findMiniMapIcon(enchantportalIcon, 0.95, huangmenX, huangmenY)) {

			miaoSenderInstance.setHuangmen(1);

		}

	}



	if (friendGuildEnabled && runFriendGuildCheck) {

		int random_x = -1;

		int random_y = -1;

		int guild_x = -1;

		int guild_y = -1;

		findMiniMapIcon(friendIcon, 0.99, random_x, random_y);

		findMiniMapIcon(guildIcon, 0.99, guild_x, guild_y);



		if (guild_x > 0 || random_x > 0) {

			if (gMonitorInstance.getFriendPlayerTimer() == 0) {

				gMonitorInstance.setFriendPlayerTimer(dm->GetTime());

			}

			else {

				if (dm->GetTime() - FRIEND_PLAYER_NOTIFICATION > gMonitorInstance.getFriendPlayerTimer()) {

					miaoSenderInstance.setOthers(1);

				}

			}

		}

		else {

			gMonitorInstance.setFriendPlayerTimer(0);

		}

	}

}

int* findCoordsOnMiniMap(long index, const TCHAR* innerIcon, double sim) {

	sptool* dm = g_info[index].dm;
	if (dm == nullptr) {
		static int s_empty[2] = { -1, -1 };
		return s_empty;
	}

	long x, y;

	long findPicRet = dm->FindPic(kMiniMapTopLeftX, kMiniMapTopLeftY, kMiniMapBottomRightX, kMiniMapBottomRightY, innerIcon, _T("000000"), sim, 0, &x, &y);

	static int  results[2];

	if (x > 0 && y > 0)

	{

		//CString tips;

		//tips.Format(_T("???????:(%d,%d)"), x,y);

		//Log(tips);

		results[0] = x;

		results[1] = y;

	}

	else {

		results[0] = -1;

		results[1] = -1;

	}



	return results;

}



int* findPic(long index, long TopLeftX, long TopLeftY, long BottomRightX, long BottomRightY, const TCHAR* innerIcon, double sim , long dir) {

	sptool* dm = g_info[index].dm;

	long x, y;

	long findPicRet = dm->FindPic(TopLeftX, TopLeftY, BottomRightX, BottomRightY, innerIcon, _T("000000"), sim, dir, &x, &y);

	static int  results[2];

	if (x > 0 && y > 0)

	{

		results[0] = x;

		results[1] = y;

	}

	else {

		results[0] = -1;

		results[1] = -1;

	}



	return results;

}



void checkChatPop(long index) {

	sptool* dm = g_info[index].dm;

	long x = 0, y = 0;

	long findPicRet;



	findPicRet = dm->FindPic(418, 448, 534, 489, chatIcon, _T("000000"), 0.999, 0, &x, &y);

	if (x > 0 && y > 0) {

		ScriptDelay(index, 200);

		press(index, "esc", 1);

		ScriptDelay(index, 200);

	}

}



void checkBossChest(long index) {

	sptool* dm = g_info[index].dm;

	long x = 0, y = 0;

	long findPicRet;



	long topLeftX, topLeftY, bottomRightX, bottomRightY;

	long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);

	long mapleWindowWidth = bottomRightX - topLeftX;

	long mapleWindowHeight = bottomRightY - topLeftY;

	if (mapleWindowWidth > 1400) {

		mapleWindowWidth = long(mapleWindowWidth / 2);

		mapleWindowHeight = long(mapleWindowHeight / 2);

	}



	findPicRet = dm->FindPic(long(mapleWindowWidth / 2), mapleWindowHeight - 300, mapleWindowWidth, mapleWindowHeight, bossChestIcon, _T("000000"), 0.999, 0, &x, &y);

	if (x > 0 && y > 0) {

		dm->CapturePng(long(mapleWindowWidth / 2), mapleWindowHeight - 300, mapleWindowWidth, mapleWindowHeight, _T("C:\\sptool\\CapturedChest.png"));

		holdKey(index, Boss箱子, randomUniform(1800, 2000), 333);

	}



}



long randomUniform(long lower, long upper) {

	srand((unsigned)time(NULL));

	return (rand() % (upper - lower + 1)) + lower;

}



void keyDown(long index, const TCHAR* key) {

	if (key == _T("")) {

		return;

	}



	sptool* dm = g_info[index].dm;

	dm->KeyDownChar(key);

}



void keyUp(long index, const TCHAR* key) {

	if (key == _T("")) {

		return;

	}



	sptool* dm = g_info[index].dm;

	dm->KeyUpChar(key);

}



void holdKey(long index, const TCHAR* key, long holdTime, long interval) {

	// 增加一个健壮性检查，防止 key 是空指针或空字符串

	if (key == nullptr || _tcscmp(key, _T("")) == 0) {

		return;

	}



	sptool* dm = g_info[index].dm;

	long safeInterval = min(interval, holdTime);



	// 判断key是否为方向键

	// 注意：这里的 "up", "down", "left", "right" 是示例

	// 您需要根据您实际传入的按键字符串进行调整

	if (_tcscmp(key, _T("up")) == 0 ||

		_tcscmp(key, _T("down")) == 0 ||

		_tcscmp(key, _T("left")) == 0 ||

		_tcscmp(key, _T("right")) == 0)

	{

		// 方向键的逻辑：按下 -> 等待(holdTime) -> 弹起

		dm->KeyDownChar(key);

		ScriptDelay(index, holdTime); // 直接延迟 holdTime

		dm->KeyUpChar(key);

	}

	else

	{

		// 其他按键的逻辑：保持原有行为

		long startTime = dm->GetTime();

		while (dm->GetTime() - startTime < holdTime) {

			dm->KeyDownChar(key);

			ScriptDelay(index, safeInterval);

		}

		dm->KeyUpChar(key);

	}

}



void press(long index, const TCHAR* key, int times, long delay) {

	if (key == _T("")) {

		return;

	}



	sptool* dm = g_info[index].dm;

	for (size_t i = 0; i < times; i++)

	{

		// Pause hotkey can arrive between ScriptDelay checkpoints; block any extra key injection immediately.

		if (g_info[index].is_pause ||

			g_info[index].thread_state == State_Pausing ||

			g_info[index].thread_state == State_Pause) {

			break;

		}

		dm->KeyPressChar(key);

		ScriptDelay(index, delay);

	}

}



bool isInRange(int targetX, int targetY, int* playerCoords, long wantedRange) {

	long xRange = abs(targetX - *playerCoords);

	long yRange = abs(targetY - *(playerCoords + 1));

	return (xRange < wantedRange&& yRange < wantedRange);

}



void JumpAttack(long index, bool rune) {

	sptool* dm = g_info[index].dm;



	long topLeftX, topLeftY, bottomRightX, bottomRightY;

	long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);

	long mapleWindowWidth = bottomRightX - topLeftX;

	long mapleWindowHeight = bottomRightY - topLeftY;

	if (mapleWindowWidth > 1400) {

		mapleWindowWidth = long(mapleWindowWidth / 2);

		mapleWindowHeight = long(mapleWindowHeight / 2);

	}



	if (gMonitorInstance.status) {

		holdKey(index, 轰炸, randomUniform(360, 400), 233);

	}

}





bool WaitForTargetLock(long index, long targetX, long targetY, long rangeX, long rangeY, long maxWaitMs, long pollMs, int stableCount) {

	sptool* dm = g_info[index].dm;

	if (dm == NULL || maxWaitMs <= 0 || pollMs <= 0) {

		return false;

	}



	int required = stableCount < 1 ? 1 : stableCount;

	int* initialCoords = gMonitorInstance.getPlayerCoords();

	int lastX = *initialCoords;

	int lastY = *(initialCoords + 1);

	bool seenCoordChange = false;

	int nearStable = 0;

	long startTime = dm->GetTime();



	while (gMonitorInstance.status > 0 && dm->GetTime() - startTime < maxWaitMs) {

		ScriptDelay(index, pollMs);



		int* currentCoords = gMonitorInstance.getPlayerCoords();

		int currentX = *currentCoords;

		int currentY = *(currentCoords + 1);



		if (currentX != lastX || currentY != lastY) {

			seenCoordChange = true;

		}



		if (currentX > 0 && currentY > 0 &&

			abs(targetX - currentX) <= rangeX &&

			abs(targetY - currentY) <= rangeY) {

			if (seenCoordChange || dm->GetTime() - startTime >= (pollMs * 2)) {

				nearStable++;

				if (nearStable >= required) {

					return true;

				}

			}

		}

		else {

			nearStable = 0;

		}



		lastX = currentX;

		lastY = currentY;

	}



	return false;

}

void goToDirection(long index, const TCHAR* direction, int distance, bool rune) {

	sptool* dm = g_info[index].dm;



	if (abs(distance) >= 20) {

		if (abs(distance) >= 40) {

			keyDown(index, direction);

			press(index, JUMP_KEY, 2, randomUniform(115,125));

			ScriptDelay(index, 580);

			keyUp(index, direction);

			ScriptDelay(index, 100);

		}

		#ifdef Adele

		else if (abs(distance) >= 30) {

			keyDown(index, direction);

			press(index, JUMP_KEY, 2, randomUniform(115, 125));

			ScriptDelay(index, 480);

			keyUp(index, direction);

			ScriptDelay(index, 200);

		}

		else {

			keyDown(index, direction);

			press(index, JUMP_KEY, 1, 90);

			ScriptDelay(index, 250);

			press(index, JUMP_KEY);

			ScriptDelay(index, 120);

			keyUp(index, direction);

			ScriptDelay(index, 350 + int(distance * 2));

		}

		#else

		else if (abs(distance) >= 30) {

			keyDown(index, direction);

			press(index, JUMP_KEY, 2, randomUniform(115, 125));

			ScriptDelay(index, 480);

			keyUp(index, direction);

			ScriptDelay(index, 200);

		}

		else {

			keyDown(index, direction);

			press(index, JUMP_KEY, 1, 70);

			ScriptDelay(index, 400);

			press(index, JUMP_KEY);

			ScriptDelay(index, 120);

			keyUp(index, direction);

			ScriptDelay(index, 350 + int(distance * 2));

		}

		#endif 



	}

	else if (abs(distance) == 1) {

		holdKey(index, direction, 20);

		ScriptDelay(index, 150);

	}

	else {

		holdKey(index, direction, 40 * abs(distance));

		ScriptDelay(index, 350);

	}



}



void goUp(long index, long distance, long wait) {

	if (abs(distance) <= 3) {

		//Log("短途");

		press(index, JUMP_KEY, 1, 150);

		ScriptDelay(index, wait);

	}

	else if (abs(distance) <= 15) {

		//Log("短途");

		press(index, JUMP_KEY, 1);

		ScriptDelay(index, 130);

		#ifdef NL

			press(index, 上跳, 1, 150);

			ScriptDelay(index, randomUniform(170, 190));

			ScriptDelay(index, wait);

		#else 

			keyDown(index, "up");

			press(index, JUMP_KEY, 1);

			ScriptDelay(index, randomUniform(165, 170));

			keyUp(index, "up");

		#endif 

	}

	else {

		//Log("长途");

		press(index, 钩子, 1, 250);

		ScriptDelay(index, abs(distance) * 10);

		ScriptDelay(index, randomUniform(300, 320));

		ScriptDelay(index, wait);

	}

}



void goDown(long index, long distance, long downoverride, bool rune) {

	keyDown(index, "down");

	holdKey(index, JUMP_KEY, 12 * distance, 66);

	keyUp(index, "down");

	ScriptDelay(index, max((long)0, downoverride - 12 * distance));

}



void rightJump(long index) {

	keyDown(index, "right");

	press(index, JUMP_KEY);

	keyUp(index, "right");

	ScriptDelay(index, 500);

}



void goTo(long index, long targetX, long targetY, long rangeFromCoords, bool isRune, long upoverride, long downoverride, bool randomWalk, long rangeY, bool preciseLock, long preciseMaxWaitMs, long precisePollMs, int preciseStableCount) {

	long WANTED_RANGE = rangeFromCoords;

	long WANTED_RANGEY = max(rangeFromCoords, rangeY);

	int* currentPlayerLocation = gMonitorInstance.getPlayerCoords();

	sptool* dm = g_info[index].dm;

	int goToMainIndex = NormalizeMonitorMainIndex(index);

	if (goToMainIndex >= 0 && goToMainIndex < MAX_HWND) {

		InterlockedExchange(&g_isGoToActive[goToMainIndex], 1);

	}

	struct ScopedGoToActiveFlag {

		int idx;

		~ScopedGoToActiveFlag() {

			if (idx >= 0 && idx < MAX_HWND) {

				InterlockedExchange(&g_isGoToActive[idx], 0);

			}

		}

	};

	ScopedGoToActiveFlag scopedGoToActiveFlag{ goToMainIndex };

	long startTime = dm->GetTime();

	long lastX = *currentPlayerLocation;

	long lastXMoveTime = startTime;

	long lastStuckJumpTime = 0;

	//CString tips;

	//tips.Format(_T("获取的坐标:(%d,%d)"), *currentPlayerLocation, *(currentPlayerLocation+1));

	//Log(tips);



	int success = 0;



	while (success == 0) {

		if (*currentPlayerLocation > 0) {

			int xDistance = targetX - *currentPlayerLocation;

			while (abs(xDistance) > WANTED_RANGE && gMonitorInstance.status > 0) {

				if (xDistance > 0) {

					goToDirection(index, _T("right"), xDistance, isRune);

				}

				else {

					goToDirection(index, _T("left"), xDistance, isRune);

				}



				if (dm->GetTime() > 3000 + startTime && gMonitorInstance.status) {

					checkChatPop(index);

				}



				if (dm->GetTime() > 12000 + startTime && gMonitorInstance.status) {

					rightJump(index);

					startTime = dm->GetTime();

				}



				currentPlayerLocation = gMonitorInstance.getPlayerCoords();

				xDistance = targetX - *currentPlayerLocation;

				long now = dm->GetTime();

				if (abs(xDistance) > 2) {

					if (*currentPlayerLocation == lastX) {

						if (now - lastXMoveTime > 800 && now - lastStuckJumpTime > 1500) {

							rightJump(index);

							lastStuckJumpTime = now;

							lastXMoveTime = now;

							currentPlayerLocation = gMonitorInstance.getPlayerCoords();

							xDistance = targetX - *currentPlayerLocation;

						}

					}

					else {

						lastX = *currentPlayerLocation;

						lastXMoveTime = now;

					}

				}

			}



			int yTrial = 0;



			int yDistance = targetY - *(currentPlayerLocation + 1);

			while (abs(yDistance) > WANTED_RANGEY && gMonitorInstance.status > 0) {

				if (yDistance < 0) {

					goUp(index, yDistance, upoverride);

				}

				else {

					goDown(index, abs(yDistance), downoverride, isRune);

				}



				if (dm->GetTime() > 3000 + startTime && gMonitorInstance.status) {

					checkChatPop(index);

				}



				currentPlayerLocation = gMonitorInstance.getPlayerCoords();

				yDistance = targetY - *(currentPlayerLocation + 1);



				if (yTrial >= 2) {

					break;

				}



				yTrial++;



			}



			if (yTrial < 2){

				success = 1;

			}



			

		}

	}





	if (preciseLock && gMonitorInstance.status > 0) {

		long lockRangeX = max((long)0, WANTED_RANGE);

		long lockRangeY = max((long)0, WANTED_RANGEY);

		WaitForTargetLock(index, targetX, targetY, lockRangeX, lockRangeY, preciseMaxWaitMs, precisePollMs, preciseStableCount);

	}

}



void cashShopTour(long index) {

	sptool* dm = g_info[index].dm;

	// 冒险窗口信息

	long topLeftX, topLeftY, bottomRightX, bottomRightY;

	long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);

	long mapleWindowWidth = bottomRightX - topLeftX;

	long mapleWindowHeight = bottomRightY - topLeftY;

	if (mapleWindowWidth > 1400) {

		mapleWindowWidth = long(mapleWindowWidth / 2);

		mapleWindowHeight = long(mapleWindowHeight / 2);

	}



	//暂停屏幕检测

	gMonitorInstance.detectionStatus = 0;





	//进入商城

	int successCashShop = 0;

	long x, y;

	dm->SetWindowState(g_info[index].hwnd, 12);

	while (successCashShop == 0) {

		holdKey(index, 商城, 960, 333);



		//查看是否进入成功

		long findPicRet = dm->FindPic(0, 20, mapleWindowWidth, mapleWindowHeight, okIcon, _T("000000"), 0.9, 0, &x, &y);

		if (x > 0 && y > 0) {

			//进入失败

			successCashShop = 0;

			press(index, "esc");

		}



		findPicRet = dm->FindPic(0, 300, 300, mapleWindowHeight, cashShopExitIcon, _T("000000"), 0.9, 0, &x, &y);

		if (x > 0 && y > 0) {

			//进入成功

			successCashShop = 1;

		}

	}



	//退出商城

	x = 0, y = 0;

	int exitCashShop = 0;

	while (exitCashShop < 1) {

		dm->SetWindowState(g_info[index].hwnd, 12);

		press(index, "esc");

		ScriptDelay(index, 200);

		SetTaskState(index, _T("查找商城退出标识"));

		//查找退出提示

		long findPicRet = dm->FindPic(0, 20, mapleWindowWidth, mapleWindowHeight, cashShopExitOKIcon, _T("000000"), 0.9, 0, &x, &y);

		if (x > 0 && y > 0) {

			dm->SetWindowState(g_info[index].hwnd, 12);

			ScriptDelay(index, 200);

			press(index, "enter");

			//long buff_location_x = x + 7;

			//long buff_location_y = y + 10;



			//dm->MoveTo(buff_location_x, buff_location_y);

			//dm->LeftClick();

			SetTaskState(index, _T("找到标识，退出"));

			ScriptDelay(index, 1500);

			exitCashShop = 1;





		}

		else {

			ScriptDelay(index, 300);

			SetTaskState(index, _T("未找到标识"));

		}

	}



	//回到地图

	int return2Map = 0;

	//等待地图标志出现

	SetTaskState(index, _T("等待地图标志出现"));

	int waitTime = 10;

	long whiteTopLeftX = 0;

	long whiteTopLeftY = 0;



	long whiteBottomRightX = whiteTopLeftX + 300;

	long whiteBottomRightY = 300;

	while (return2Map == 0) {

		ScriptDelay(index, 1000);

		long findPicRet = dm->FindPic(whiteTopLeftX, whiteTopLeftY, whiteBottomRightX, whiteBottomRightY, whiteIcon, _T("000000"), 0.9, 0, &x, &y);

		if (x > 0 && y > 0) {

			return2Map = 1;

			break;

		}

	}



	//恢复屏幕检测

	gMonitorInstance.setRuneCoords(-1, -1);

	gMonitorInstance.detectionStatus = 1;

	buffTimeOut_buff_轮回 = 0;

}



CString getArrowsKey(CString* arrowsKey) {

	return *(arrowsKey + 1);

}



CString findArrowDirection(std::vector<unsigned char> image, long x, long y, unsigned width, unsigned height) {



	for (int i = 0; i < 20; i++)

	{	

		// rightPixel

		if (x + i < static_cast<long>(width)) {



			long red = (long)image[y * width * 4 + (x + i) * 4];

			long green = (long)image[y * width * 4 + (x + i) * 4 + 1];

			long blue = (long)image[y * width * 4 + (x+ i) * 4 + 2];



			if (green >= 200 && 150 < red && red <= 255) {

				return (CString)"right";

			}

		}



		// leftPixel

		if (x - i >= 0) {



			long red = (long)image[y * width * 4 + (x - i) * 4];

			long green = (long)image[y * width * 4 + (x - i) * 4 + 1];

			long blue = (long)image[y * width * 4 + (x - i) * 4 + 2];



			if (green >= 200 && 150 < red && red <= 255) {

				return (CString)"left";

			}

		}



		// upPixel

		if (y - i >= 0) {



			long red = (long)image[(y - i) * width * 4 + x * 4];

			long green = (long)image[(y - i) * width * 4 + x * 4 + 1];

			long blue = (long)image[(y - i) * width * 4 + x * 4 + 2];



			if (green >= 200 && 150 < red && red <= 255) {

				return (CString)"up";

			}

		}



		// downPixel

		if (y + i < static_cast<long>(height)) {



			long red = (long)image[(y + i) * width * 4 + x * 4];

			long green = (long)image[(y + i) * width * 4 + x * 4 + 1];

			long blue = (long)image[(y + i) * width * 4 + x * 4 + 2];



			if (green >= 200 && 150 < red && red <= 255) {

				return (CString)"down";

			}

		}





	}



	return _T("");

}



runeData* solveRune() {



	runeData *data= new runeData[11];



	int count = 0;

	bool addable;



	const char* filename = "C:\\sptool\\RuPic.png";

	std::vector<unsigned char> image; //the raw pixels

	unsigned width, height;



	//decode

	unsigned error = lodepng::decode(image, width, height, filename);



	for (long currentX = 0; currentX < static_cast<long>(width); currentX++)

	{

		for (long currentY = 0; currentY < static_cast<long>(height); currentY++)

		{

			addable = true;

			// CString rgbPixel = dm->GetColor(currentX, currentY);



			long red, green, blue;



			red = (long)image[currentY * width * 4 + currentX * 4];

			green = (long)image[currentY * width * 4 + currentX * 4 + 1];

			blue = (long)image[currentY * width * 4 + currentX * 4 + 2];



			//red = strtoull(rgbPixel.Left(2), NULL, 16);

			//green = strtoull(rgbPixel.Mid(2, 2), NULL, 16);

			//blue = strtoull(rgbPixel.Right(2), NULL, 16);



			//CString tips;

			//tips.Format(_T("rbg:(%d,%d,%d),坐标 (%d,%d)"), red, green, blue, currentX, currentY);

			//Log(tips);



			//int red = std::stoi((string)rgbPixel.Left(2), nullptr, 16);

			//int green = std::stoi((string)rgbPixel.Mid(2,2), nullptr, 16);

			//int blue = std::stoi((string)rgbPixel.Right(2), nullptr, 16);



			if ( 235 <= green && green <= 255 && 0 <= red && red <= 70 && 0 <= blue && blue <= 80) {







				for (int i = 0; i < count; i++)

				{

					if (abs(currentX - data[i].x) <= 25) {

						addable = false;

					}

				}



				if (addable) {

					//CString tips;

					//tips.Format(_T("rbg:(%d,%d,%d),坐标 (%d,%d)"), red, green, blue, currentX, currentY);

					//Log(tips);

					CString direction = findArrowDirection(image, currentX, currentY, width, height);

					if (direction != "") {

						data[count].arrow = direction;

						data[count].x = currentX;

						count++;

					}

				}

			}

		}

	}



	data[10].arrow = "info";

	data[10].x = -count;



	return data;

}



void checkRune(long index, int* runeCoords) {

	if (*gMonitorInstance.getRuneCoords() > 0) {



		sptool* dm = g_info[index].dm;



		// 提醒

		if (rune_fail_time % 10 == 7) {

			rightJump(index);

			miaoSenderInstance.setHuangmen(1);

		}



		//进商城刷新符文

		if ((rune_fail_time > 0) && (rune_fail_time % 3 == 0)) {

			cashShopTour(index);

			ScriptDelay(index, 500);

		}



		long x, y;

		x = *runeCoords;

		y = *(runeCoords + 1);



		if (x == -1 && y == -1)

		{

			rune_fail_time++;

			return;

		}



		if (gMonitorInstance.status) {

			goTo(index, x, y, 1, true);

			ScriptDelay(index, 300);

		}

		if (gMonitorInstance.status) {

			goTo(index, x, y, 1, true);

			ScriptDelay(index, 200);

		}

		if (gMonitorInstance.status) {

			goTo(index, x, y, 1, true);

			ScriptDelay(index, 100);

		}



		if (gMonitorInstance.status && rune_fail_time == 0) {

			ScriptDelay(index, 500);

		}

		else if (gMonitorInstance.status) {

			ScriptDelay(index, 100);

		}

		int* currentPlayerCoords = gMonitorInstance.getPlayerCoords();

		if (!isInRange(x, y, currentPlayerCoords, 5)) {

			return;

		}

		press(index, OPEN_RUNE_KEY, 1, 200);

		SetTaskState(index, _T("截图符文"));

		if (gMonitorInstance.status) {

			ScriptDelay(index, 800);

		}





		// 冒险窗口信息

		long topLeftX, topLeftY, bottomRightX, bottomRightY;

		long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);

		long mapleWindowWidth = bottomRightX - topLeftX;

		long mapleWindowHeight = bottomRightY - topLeftY;

		if (mapleWindowWidth > 1400) {

			mapleWindowWidth = long(mapleWindowWidth / 2);

			mapleWindowHeight = long(mapleWindowHeight / 2);

		}



		long RUNE_WIDTH = 460;

		long runeTopLeftX = long((mapleWindowWidth / 2) - (RUNE_WIDTH / 2));

		long runeTopLeftY = long(mapleWindowHeight / 4) - 30;

		long runeBottomRightX = runeTopLeftX + RUNE_WIDTH;

		long runeBottomRightY = runeTopLeftY + 100;



		dm->CapturePng(runeTopLeftX, runeTopLeftY, runeBottomRightX, runeBottomRightY, _T("C:\\sptool\\RuPic.png"));



		SetTaskState(index, _T("解码符文"));

		runeData* resData = solveRune();

		// 原文件路径

		std::string original_path = "C:/sptool/RuPic.png";

		cv::Mat image = cv::imread(original_path);

		std::vector<std::string> directions = getArrowDirections(3, image);

		std::vector<std::string> directions2 = getArrowDirections(5, image);

		if (directions.size() != 4) {

			directions = directions2;

		}

		bool needRightJump = false;

		if (resData[10].x == -4) {

			for (int i = 0; i < 4; i++)

			{

				ScriptDelay(index, randomUniform(100, 250));

				press(index, resData[i].arrow);

			}





			gMonitorInstance.setRuneCoords(-1, -1);

			SetTaskState(index, _T("刷图"));



			if (resData[3].arrow == (CString)"up" | resData[3].arrow == (CString)"down") {

				ScriptDelay(index, 300);

				needRightJump = true;

			}

			else {

				ScriptDelay(index, 500);

			}

			rune_fail_time = 0;



			//for (size_t i = 0; i < 3; i++)

			//{

			//	clickRune(index);

			//	ScriptDelay(index, 200);

			//}



			if (needRightJump)

			{

				rightJump(index);

			}

		}

		else if (directions.size() == 4) {



			string s = "RuSuccessPic";

			long a = dm->GetTime() % 10000;

			string s_type = ".png";

			string new_filename = s + to_string(a) + s_type;

			copy_and_rename_file(original_path, new_filename);



			for (const string& direction : directions) {

				ScriptDelay(index, randomUniform(100, 250));

				press(index, direction.c_str());

			}



			gMonitorInstance.setRuneCoords(-1, -1);

			SetTaskState(index, _T("刷图"));



			if (directions[3].c_str() == (CString)"up" | resData[3].arrow == (CString)"down") {

				ScriptDelay(index, 300);

				needRightJump = true;

			}

			else {

				ScriptDelay(index, 300);

			}

			rune_fail_time = 0;



			if (needRightJump)

			{

				rightJump(index);

			}

		}

		else {

			string s = "RuFailPic";

			long a = dm->GetTime() % 10000;

			string s_type = ".png";

			string new_filename = s + to_string(a) + s_type;

			copy_and_rename_file(original_path, new_filename);



			rune_fail_time++;

			if (gMonitorInstance.status) {

				ScriptDelay(index, 500);

				for (size_t i = 0; i < 2; i++)

				{

					JumpAttack(index, false);

					ScriptDelay(index, 1500);

				}

			}

			SetTaskState(index, _T("符文失败"));





		}





		delete[] resData;

	}

}



void checkDefaults(long index) {

	// Check if there is a rune

	checkRune(index, gMonitorInstance.getRuneCoords());



}



void startBo(long index) {

	sptool* dm = g_info[index].dm;



	long buffTimeOut = dm->GetTime() + BUFF_TIME;

	long buffTimeOut_2 = dm->GetTime() + BUFF_TIME_2;

	long tigerTimeOut = dm->GetTime();

	long lcdTimeOut = dm->GetTime();

	long timeTrack = dm->GetTime();

	long currentTime;

	buffTimeOut_buff_米特拉 = dm->GetTime() + 120000;

	int tiger = 0;



	long topLeftX, topLeftY, bottomRightX, bottomRightY;

	long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);

	long mapleWindowWidth = bottomRightX - topLeftX;

	long mapleWindowHeight = bottomRightY - topLeftY;

	if (mapleWindowWidth > 1400) {

		mapleWindowWidth = long(mapleWindowWidth / 2);

		mapleWindowHeight = long(mapleWindowHeight / 2);

	}



	long RUNE_WIDTH = 460;

	long runeTopLeftX = long((mapleWindowWidth / 2) - (RUNE_WIDTH / 2));

	long runeTopLeftY = long(mapleWindowHeight / 4) - 30;

	long runeBottomRightX = runeTopLeftX + RUNE_WIDTH;

	long runeBottomRightY = runeTopLeftY + 100;



	dm->CapturePng(0, 0, mapleWindowWidth, mapleWindowHeight, _T("C:\\sptool\\MyPic.png"));



	while (1) {

        if (AutoLogin_IsActive(index)) { ScriptDelay(index, 200); continue; }

		UpdateAutoOilCooldown(index, gMonitorInstance.status == 1);
		if (gMonitorInstance.status == 1) {

			currentTime = dm->GetTime();

			dm->SetWindowState(g_info[index].hwnd, 12);



			// 判断是不是刷图模式

			if (Gethunt()) {

				SetTaskState(index,_T("刷图"));

				if (GetautoRuneSolver() == 1) { checkDefaults(index); }

				if (dm->GetTime()> buffTimeOut_buff_bossChest) {

					checkBossChest(index);

					buffTimeOut_buff_bossChest = dm->GetTime() + 75000;

				}

				if (*gMonitorInstance.getPlayerCoords() > 0 && *gMonitorInstance.getRuneCoords() == -1) {

					if (Getmap() == 0) {

						UpperDeckMap(index);

					}



					if (Getmap() == 1) {

						Forest3Map(index);

					}



				}

				else {

					ScriptDelay(index, 250);

					SetTaskState(index, _T("待机"));

				}

			}

		}

		else {

			ScriptDelay(index, 250);

			SetTaskState(index, _T("待机"));

		}

	}

}



void clickRune(long index) {

	(void)index;

	return;



	sptool* dm = g_info[index].dm;



	// 冒险窗口信息

	long topLeftX, topLeftY, bottomRightX, bottomRightY;

	long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);

	long mapleWindowWidth = bottomRightX - topLeftX;

	long mapleWindowHeight = bottomRightY - topLeftY;

	if (mapleWindowWidth > 1400) {

		mapleWindowWidth = long(mapleWindowWidth / 2);

		mapleWindowHeight = long(mapleWindowHeight / 2);

	}



	long runestateTopLeftX = 0;

	long runestateTopLeftY = 0;



	long runestateBottomRightX = mapleWindowWidth;

	long runestateBottomRightY = 45;



	long x, y;

	long findPicRet = dm->FindPic(runestateTopLeftX, runestateTopLeftY,

		runestateBottomRightX, runestateBottomRightY, runestateIcon, _T("000000"), 0.8, 0, &x, &y);



	if (x > 0 && y > 0) {

		long buff_location_x = x + 10;

		long buff_location_y = y + 7;



		dm->MoveTo(buff_location_x, buff_location_y);

		dm->RightClick();

		dm->MoveTo(buff_location_x, buff_location_y + 60);

	}

}



void FieldCheck(long index, bool long_activate_gunboat_one, bool left = true, bool compulsory = false) {

	sptool* dm = g_info[index].dm;



	if ((gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_小立场) || compulsory)

	{

		ScriptDelay(index, 200);

		if (left) {

			holdKey(index, _T("left"), 30);

		}

		else

		{

			holdKey(index, _T("right"), 30);

		}

		holdKey(index, 小立场, 350);

		buffTimeOut_buff_小立场 = dm->GetTime() + BUFF_TIME_小立场;

		ScriptDelay(index, 630);

	}



}



void ResistCheck(long index, bool left = true, bool compulsory = false) {

	sptool* dm = g_info[index].dm;

	if ((gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_反抗放置) || compulsory)

	{

		holdKey(index, 反抗放置, 580, 233);

		buffTimeOut_buff_反抗放置 = dm->GetTime() + BUFF_TIME_反抗放置;

		ScriptDelay(index, 380);

	}

}



void CheckJanus(long index, bool compulsory = false, int order = 1) {

	sptool* dm = g_info[index].dm;

	if (gMonitorInstance.status && order == 1 && (compulsory || dm->GetTime() > buffTimeOut_buff_黑球))

	{

		holdKey(index, 黑球, 140, 55);

		buffTimeOut_buff_黑球 = dm->GetTime() + BUFF_TIME_黑球;

		ScriptDelay(index, 470);

	}

	else if (gMonitorInstance.status && order == 2)

	{

		holdKey(index, 黑球, 140, 55);

		ScriptDelay(index, 470);

	}

	else if (gMonitorInstance.status && order == 3)

	{

		holdKey(index, 黑球, 140, 55);

		ScriptDelay(index, 370);

	}

}



void CheckFountain(long index) {

	sptool* dm = g_info[index].dm;

	if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_艾尔达洗澡 )

	{

		holdKey(index, 艾尔达洗澡, 440, 233);

		buffTimeOut_buff_艾尔达洗澡 = dm->GetTime() + BUFF_TIME_艾尔达洗澡;

		ScriptDelay(index, 270);

	}

}



void CheckRoutineBuff(long index, bool left = true, bool spider = false, int job = 5) {

	sptool* dm = g_info[index].dm;



	#ifdef Adele

	if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_飞剑)

	{

		sptool* dm = g_info[index].dm;

		long startTime = dm->GetTime();

		while (dm->GetTime() - startTime < 3000) {

			dm->KeyDownChar(飞剑);

			dm->KeyDownChar(轰炸);

			ScriptDelay(index, 133);

		}

		dm->KeyUpChar(飞剑);

		dm->KeyUpChar(轰炸);

		buffTimeOut_buff_飞剑 = dm->GetTime() + BUFF_TIME_飞剑;

		ScriptDelay(index, 10);

	}



	#else 

	if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_骰子 && job >= 5)

	{

		holdKey(index, 骰子, 580, 233);

		buffTimeOut_buff_骰子 = dm->GetTime() + BUFF_TIME_骰子;

		ScriptDelay(index, 310);

	}

	#endif 







	//if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_代码 && job >= 5)

	//{

	//	holdKey(index, 能量补充, 540, 133);

	//	ScriptDelay(index, 70);

	//	holdKey(index, 代码, 680, 133);

	//	buffTimeOut_buff_代码 = dm->GetTime() + BUFF_TIME_代码;

	//	ScriptDelay(index, 110);

	//}



	if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_轮回 && job >= 5)

	{

		holdKey(index, 轮回, 680, 333);

		buffTimeOut_buff_轮回 = dm->GetTime() + BUFF_TIME_轮回;

		ScriptDelay(index, 110);

	}



	if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_蜘蛛 && spider  )

	{

		holdKey(index, 蜘蛛, 510, 233);

		buffTimeOut_buff_蜘蛛 = dm->GetTime() + BUFF_TIME_蜘蛛;

		ScriptDelay(index, 1170);

	}



	if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_米特拉 && job >= 5)

	{

		holdKey(index, 米特拉, 510, 233);

		buffTimeOut_buff_米特拉 = dm->GetTime() + BUFF_TIME_米特拉;

		ScriptDelay(index, 670);

	}



}



void OKDetector(long index) {

	sptool* dm = g_info[index].dm;
	if (dm == nullptr) return;



	long topLeftX, topLeftY, bottomRightX, bottomRightY;

	long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);
	if (windowRet != 1) return;

	long mapleWindowWidth = bottomRightX - topLeftX;

	long mapleWindowHeight = bottomRightY - topLeftY;
	if (mapleWindowWidth <= 0 || mapleWindowHeight <= 0) return;

	if (mapleWindowWidth > 1400) {

		mapleWindowWidth = long(mapleWindowWidth / 2);

		mapleWindowHeight = long(mapleWindowHeight / 2);

	}

	long BOSS_WIDTH = 200;



	long bossTopLeftX = long((mapleWindowWidth / 2) - (BOSS_WIDTH / 2));

	long bossTopLeftY = long((mapleWindowHeight / 2) - 20);

	long bossBottomRightX = long((mapleWindowWidth / 2) + (BOSS_WIDTH / 2));

	long bossBottomRightY = long(120 + (mapleWindowHeight / 2));

	auto findOk = [&](long& foundX, long& foundY) {
		foundX = -1;
		foundY = -1;
		dm->FindPic(bossTopLeftX, bossTopLeftY, bossBottomRightX, bossBottomRightY, okIcon, _T("000000"), 0.95, 0, &foundX, &foundY);
		if (foundX > 0 && foundY > 0) {
			return true;
		}

		foundX = -1;
		foundY = -1;
		dm->FindPic(bossTopLeftX, bossTopLeftY, bossBottomRightX, bossBottomRightY, okIcon, _T("000000"), 0.90, 0, &foundX, &foundY);
		return foundX > 0 && foundY > 0;
	};

	for (int detectAttempt = 0; detectAttempt < 12; ++detectAttempt) {
		long okX = -1;
		long okY = -1;
		if (findOk(okX, okY)) {
			press(index, "esc", 1, randomUniform(300, 410));
			ScriptDelay(index, 400);
			if (findOk(okX, okY)) {
				dm->MoveTo(okX, okY);
				ScriptDelay(index, 50);
				dm->LeftClick();
			}
			return;
		}
		ScriptDelay(index, 150);
	}

}



void CheckUseBuff(long index) {



	sptool* dm = g_info[index].dm;





	//消耗品buff

	if (Getignite() & (buffTimeOut_buff_点火 <= dm->GetTime())) {

		dm->SetWindowState(g_info[index].hwnd, 12);

		ScriptDelay(index, 50);

		holdKey(index, 点火, randomUniform(500, 610), 233);
		ScriptDelay(index, randomUniform(400, 500));
		OKDetector(index);

		buffTimeOut_buff_点火 = dm->GetTime() + 1810000;

	}



	if (GetautoOil() && IsAutoOilReady(index)) {

		holdKey(index, 汽油, randomUniform(200, 220));
		ScriptDelay(index, randomUniform(400, 500));
		OKDetector(index);

		ArmAutoOilCooldown(index, 3700000);

	}



	if (GetexpPot() & (buffTimeOut_buff_经验秘药2h <= dm->GetTime())) {

		dm->SetWindowState(g_info[index].hwnd, 12);

		ScriptDelay(index, 50);

		holdKey(index, 经验秘药2h, randomUniform(900, 920), 333);

		OKDetector(index);

		holdKey(index, 聚财秘药2h, randomUniform(900, 920), 333);

		OKDetector(index);

		buffTimeOut_buff_经验秘药2h = dm->GetTime() + BUFF_TIME_经验秘药2h;

	}



	if (GetautoWealth() & (buffTimeOut_buff_经验秘药30 <= dm->GetTime())) {

		dm->SetWindowState(g_info[index].hwnd, 12);

		ScriptDelay(index, 50);

		holdKey(index, 经验秘药30, randomUniform(800, 920), 333);

		OKDetector(index);

		holdKey(index, 聚财秘药30, randomUniform(800, 920), 333);

		OKDetector(index);

		buffTimeOut_buff_经验秘药30 = dm->GetTime() + BUFF_TIME_经验秘药30;

	}



	if (GetExp30() & (buffTimeOut_buff_经验exp30 <= dm->GetTime())) {

		dm->SetWindowState(g_info[index].hwnd, 12);

		ScriptDelay(index, 50);

		holdKey(index, 经验exp30, randomUniform(700, 820), 333);

		//ScriptDelay(index,300);

		OKDetector(index);

		buffTimeOut_buff_经验exp30 = dm->GetTime() + GetExpBuffDuration();

	}







	//if (GetExp10() & !GetExp30() & (buffTimeOut_buff_经验exp10 <= dm->GetTime())) {

	//	holdKey(index, 经验exp15, randomUniform(700, 820), 133);

	//	//ScriptDelay(index, 300);

	//	OKDetector(index);

	//	buffTimeOut_buff_经验exp10 = dm->GetTime() + BUFF_TIME_经验exp10;

	//}



	//if (GetExp30() & GetExp10() & (buffTimeOut_buff_经验exp30 <= dm->GetTime())) {

	//	holdKey(index, 经验exp30, randomUniform(700, 820), 133);

	//	//ScriptDelay(index, 300);

	//	OKDetector(index);

	//	buffTimeOut_buff_经验exp30 = dm->GetTime() + BUFF_TIME_经验exp20;

	//}



	if (Getkuxing() & (buffTimeOut_buff_MVP <= dm->GetTime())) {

		dm->SetWindowState(g_info[index].hwnd, 12);

		ScriptDelay(index, 50);

		holdKey(index, MVP, randomUniform(700, 820), 333);

		//ScriptDelay(index, 300);

		OKDetector(index);

		buffTimeOut_buff_MVP = dm->GetTime() + BUFF_TIME_MVP;

	}



	if (GetEXP_PARK() & (buffTimeOut_buff_公园 <= dm->GetTime())) {

		dm->SetWindowState(g_info[index].hwnd, 12);

		ScriptDelay(index, 50);

		ScriptDelay(index, 400);

		press(index, 公园, 1, randomUniform(200, 320));

		buffTimeOut_buff_公园 = dm->GetTime() + BUFF_TIME_公园;

	}



	if (GetExp10() & (buffTimeOut_buff_经验exp10 <= dm->GetTime())) {

		dm->SetWindowState(g_info[index].hwnd, 12);

		ScriptDelay(index, 50);

		ScriptDelay(index, 400);

		press(index, 家族1, 1, randomUniform(500, 620));

		press(index, 家族2, 1, randomUniform(200, 320));

		buffTimeOut_buff_经验exp10 = dm->GetTime() + BUFF_TIME_经验exp30;

		guild_skill_count++;

	}

}



void SpringMap(long index) {

	sptool* dm = g_info[index].dm;



	CheckUseBuff(index);

	if (gMonitorInstance.status && dm->GetTime() + 2500 > buffTimeOut_buff_小立场)

	{



		int* currentPlayerLocation = gMonitorInstance.getPlayerCoords();

		if (*currentPlayerLocation >= 62 && *currentPlayerLocation <= 72 && *(1 + currentPlayerLocation) == 97) {

			if (gMonitorInstance.status) { ResistCheck(index,false,true); }

			if (gMonitorInstance.status) { ScriptDelay(index, 350); }

			if (gMonitorInstance.status) { goTo(index, 27, 105, 10, true); }

			if (gMonitorInstance.status) { ScriptDelay(index, 100); }

		}



		if (gMonitorInstance.status) { goTo(index, 133, 108, 5, true, 500, 700); }

		if (gMonitorInstance.status) { goTo(index, 134, 108, 5, true, 500, 700); }

		if (gMonitorInstance.status) { FieldCheck(index, true, false, true); }



		if (gMonitorInstance.status) { goTo(index, 150, 108, 4, true, 500, 700); }

		if (gMonitorInstance.status) { goTo(index, 150, 108, 4, true, 500, 700); }

		if (gMonitorInstance.status) { ScriptDelay(index, 300); }

		if (gMonitorInstance.status) { keyDown(index, _T("left")); }

		if (gMonitorInstance.status) { ScriptDelay(index, 50); }

		if (gMonitorInstance.status) { holdKey(index, 斜冲, 350); }

		if (gMonitorInstance.status) { keyUp(index, _T("left")); }

		if (gMonitorInstance.status) { ScriptDelay(index, 300); }





		if (gMonitorInstance.status) { goTo(index, 116, 86, 3, true); }

		holdKey(index, _T("left"), 40);

		if (gMonitorInstance.status) { CheckFountain(index); }

		if (gMonitorInstance.status) { ScriptDelay(index, 300); }



		if (gMonitorInstance.status) { goTo(index, 116, 97, 3, true, 500, 700); }

		if (gMonitorInstance.status) { CheckRoutineBuff(index, false, false, 5); }





	}



	if (gMonitorInstance.status &&  dm->GetTime() + 2500 < buffTimeOut_buff_小立场) {

		if (gMonitorInstance.status) { goTo(index, 70, 95, 6, true, 500, 700); }

		if (gMonitorInstance.status) { goTo(index, 67, 97, 3, true, 500, 700); }

		holdKey(index, _T("left"), 30);

	}



	while (gMonitorInstance.status && dm->GetTime() + 2500 < buffTimeOut_buff_小立场) {

		if (gMonitorInstance.status) { JumpAttack(index, false); }

		ScriptDelay(index, 200);

		if (gMonitorInstance.status) { CheckRoutineBuff(index, false, true, 5); }

		if (gMonitorInstance.status) { CheckUseBuff(index); }

		if (gMonitorInstance.status && dm->GetTime()%10000 < 1000) {

			checkChatPop(index);

		}

	}



	}



void ForestMap(long index) {

	sptool* dm = g_info[index].dm;

	long 放置流程耗时 = 5000;



	if (gMonitorInstance.status) { CheckRoutineBuff(index, false, false, 5); }

	if (gMonitorInstance.status) { CheckUseBuff(index); }

	

	if (gMonitorInstance.status && dm->GetTime() + 放置流程耗时 < buffTimeOut_buff_小立场 && dm->GetTime() < buffTimeOut_buff_大立场) {

		if (gMonitorInstance.status) { goTo(index, 46, 126, 6, true, 500, 700); }

		if (gMonitorInstance.status) { goTo(index, 46, 126, 3, true, 500, 700); }

		if (gMonitorInstance.status) {

			ScriptDelay(index, 180);

			holdKey(index, _T("right"), 55);

		}

	}



	while (gMonitorInstance.status && dm->GetTime() + 放置流程耗时 < buffTimeOut_buff_小立场 && dm->GetTime() < buffTimeOut_buff_大立场) {

		if (gMonitorInstance.status) { JumpAttack(index, false); }

		ScriptDelay(index, 200);

		if (gMonitorInstance.status) { CheckRoutineBuff(index, false, true, 5); }

		if (gMonitorInstance.status) { CheckUseBuff(index); }

		//if (gMonitorInstance.status && dm->GetTime() % 10000 < 1000) {

		//	checkChatPop(index);

		//}

	}



	if (gMonitorInstance.status && dm->GetTime() + 放置流程耗时 > buffTimeOut_buff_小立场)

	{

		int* currentPlayerLocation = gMonitorInstance.getPlayerCoords();

		if (*currentPlayerLocation >= 40 && *currentPlayerLocation <= 60) {

			if (gMonitorInstance.status) { goTo(index, 50, 125, 4, true, 500, 700); }

			if (gMonitorInstance.status) { ResistCheck(index, false, true); }

			if (gMonitorInstance.status) { ScriptDelay(index, 350); }

			if (gMonitorInstance.status) {

				keyDown(index, "left");

				press(index, JUMP_KEY, 1, 90);

				ScriptDelay(index, 320);

				press(index, JUMP_KEY);

				ScriptDelay(index, 50);

				keyUp(index, "left");

				ScriptDelay(index, 450);

			}

			// rope precision: single precise goTo, keep up as separate action

 if (gMonitorInstance.status) { goTo(index, 17, 127, 1, true, 500, 600, false, 0, true, 420, 60, 2); }

			if (gMonitorInstance.status) { holdKey(index, "up", 100, 133); }

		}

		if (gMonitorInstance.status) { ScriptDelay(index, 350); }

		if (gMonitorInstance.status) { goTo(index, 141, 123, 7, true, 500, 600); }

		if (gMonitorInstance.status) { goTo(index, 141, 123, 7, true, 500, 600); }

		if (gMonitorInstance.status) { holdKey(index, _T("right"), 40); }

		if (gMonitorInstance.status) { CheckFountain(index); }

		if (gMonitorInstance.status) { ScriptDelay(index, 150); }

		if (gMonitorInstance.status) { holdKey(index, _T("right"), 500); }

		if (gMonitorInstance.status) {

			ScriptDelay(index, 500);

			keyDown(index, "left");

			press(index, JUMP_KEY, 2, 120);

			ScriptDelay(index, 25);

			keyUp(index, "left");

			ScriptDelay(index, 600);

		}





		if (gMonitorInstance.status) { goTo(index, 112, 121, 4, true, 500, 600); }

		if (gMonitorInstance.status) { goTo(index, 112, 121, 2, true, 500, 600); }

		if (gMonitorInstance.status) { ScriptDelay(index, 80); }

		if (gMonitorInstance.status) { FieldCheck(index, true, true, true); }

		if (gMonitorInstance.status) {

			ScriptDelay(index, 300);

			keyDown(index, "left");

			press(index, JUMP_KEY, 1, 90);

			ScriptDelay(index, 250);

			press(index, JUMP_KEY);

			ScriptDelay(index, 50);

			keyUp(index, "left");

			ScriptDelay(index, 470);

		}



		if (gMonitorInstance.status) { goTo(index, 79, 140, 8, true, 500, 600); }



	}



	if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_大立场) {

		if (gMonitorInstance.status) { goTo(index, 49, 138, 6, true, 500, 700); }

		if (gMonitorInstance.status) { 

			ScriptDelay(index, 280);

			holdKey(index, _T("left"), 55);

		}

		if (gMonitorInstance.status) {

			holdKey(index, 大立场, 450);

			buffTimeOut_buff_大立场 = dm->GetTime() + BUFF_TIME_大立场;

			ScriptDelay(index, 530);

		}

	}







}



void Forest3Map(long index) {

	sptool* dm = g_info[index].dm;



	if (gMonitorInstance.status) { CheckRoutineBuff(index, false, false, 5); }

	if (gMonitorInstance.status) { CheckUseBuff(index); }

	int count = 1;



	if (gMonitorInstance.status && dm->GetTime() + 4000 < buffTimeOut_buff_黑球) {

		if (gMonitorInstance.status) { goTo(index, 153, 124, 5); }

		if (gMonitorInstance.status) { goTo(index, 153, 124, 3); }

		if (gMonitorInstance.status) { holdKey(index, "left", 100, 133); }

		if (gMonitorInstance.status) { holdKey(index, "up", 150, 133); }

	}



	while (gMonitorInstance.status && dm->GetTime() + 4000 < buffTimeOut_buff_黑球) {

		if (gMonitorInstance.status) { checkChatPop(index); }

		if (gMonitorInstance.status) { CheckRoutineBuff(index, false); }

		if (gMonitorInstance.status) { CheckUseBuff(index); }

		if (gMonitorInstance.status) { JumpAttack(index, true); }

		if (gMonitorInstance.status) { ScriptDelay(index, 150); }

		if (gMonitorInstance.status && count % 48 == 47) { 

			ScriptDelay(index, 250);

			holdKey(index, "left", 60); 

		}



		if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_艾尔达洗澡) { 

			ScriptDelay(index, 100);

			CheckFountain(index); 

		}

		count++;

	}



	if (gMonitorInstance.status && dm->GetTime() + 4000 > buffTimeOut_buff_黑球)

	{



		if (gMonitorInstance.status) { ResistCheck(index); }

		if (gMonitorInstance.status) { ScriptDelay(index, 100); }

		if (gMonitorInstance.status) { goTo(index, 105, 122, 5); }

		if (gMonitorInstance.status) { goTo(index, 105, 122, 3); }

		if (gMonitorInstance.status) {

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			ScriptDelay(index, 450);

			CheckJanus(index, true, 1);

		}

		if (gMonitorInstance.status) {

			ScriptDelay(index, 150);

			keyDown(index, "left");

			press(index, JUMP_KEY, 3, randomUniform(95, 105));

			ScriptDelay(index, 280);

			keyUp(index, "left");

			ScriptDelay(index, 200);

		}

		if (gMonitorInstance.status) { goTo(index, 69, 120, 4); }

		//if (gMonitorInstance.status) { holdKey(index, "up", 150, 133); }

		if (gMonitorInstance.status) { checkChatPop(index); }

		if (gMonitorInstance.status) {

			ScriptDelay(index, 100);

			CheckJanus(index, true, 2);

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			ScriptDelay(index, 450);

		}

		if (gMonitorInstance.status) { goTo(index, 36, 121, 5); }

		if (gMonitorInstance.status) {

			ScriptDelay(index, 100);

			CheckJanus(index, true, 3);

			ScriptDelay(index, 350);

		}



		// rope precision: single precise goTo, keep up as separate action

 if (gMonitorInstance.status) { goTo(index, 26, 124, 1, true, 500, 600, false, 0, true, 420, 60, 2); }

		if (gMonitorInstance.status) { holdKey(index, "up", 100, 133); }



		if (gMonitorInstance.status) { ScriptDelay(index, 350); }

		if (gMonitorInstance.status) { goTo(index, 152, 124, 7, true, 500, 600); }

	}





	//if (gMonitorInstance.status && dm->GetTime() > buffTimeOut_buff_小立场)

	//{

	//	int* currentPlayerLocation = gMonitorInstance.getPlayerCoords();

	//	if (abs(*currentPlayerLocation - 53) <= 8) {

	//		if (gMonitorInstance.status) { ResistCheck(index, false, true); }

	//		if (gMonitorInstance.status) { ScriptDelay(index, 350); }

	//		if (gMonitorInstance.status) { goTo(index, 28, 124, 3, true); }

	//		if (gMonitorInstance.status) { ScriptDelay(index, 30); }

	//		if (gMonitorInstance.status) { goTo(index, 28, 124, 3, true); }

	//		if (gMonitorInstance.status) { ScriptDelay(index, 30); }

	//		if (gMonitorInstance.status) { goTo(index, 28, 124, 1, true); }

	//		if (gMonitorInstance.status) { ScriptDelay(index, 50); }

	//		if (gMonitorInstance.status) { goTo(index, 28, 124, 0, true); }

	//		if (gMonitorInstance.status) { ScriptDelay(index, 100); }

	//		if (gMonitorInstance.status) { goTo(index, 28, 124, 0, true); }

	//		if (gMonitorInstance.status) { holdKey(index, "up", 100, 133); }

	//	}

	//	if (gMonitorInstance.status) { ScriptDelay(index, 350); }

	//	if (gMonitorInstance.status) { goTo(index, 152, 124, 7, true, 500, 600); }

	//	if (gMonitorInstance.status) { goTo(index, 152, 124, 7, true, 500, 600); }

	//	if (gMonitorInstance.status) { holdKey(index, _T("left"), 40); }

	//	if (gMonitorInstance.status) { CheckFountain(index); }

	//	if (gMonitorInstance.status) { JumpAttack(index, false); }

	//	if (gMonitorInstance.status) { ScriptDelay(index, 200); }

	//	if (gMonitorInstance.status) {

	//		ScriptDelay(index, 500);

	//		keyDown(index, "left");

	//		press(index, JUMP_KEY, 2, 120);

	//		ScriptDelay(index, 25);

	//		keyUp(index, "left");

	//		ScriptDelay(index, 300);

	//	}





	//	if (gMonitorInstance.status) { goTo(index, 116, 122, 4, true, 500, 600); }

	//	if (gMonitorInstance.status) { goTo(index, 116, 124, 2, true, 500, 600); }

	//	if (gMonitorInstance.status) { ScriptDelay(index, 80); }

	//	if (gMonitorInstance.status) { FieldCheck(index, true, true, true); }

	//	if (gMonitorInstance.status) {

	//		ScriptDelay(index, 300);

	//	}

	//	if (gMonitorInstance.status) { goTo(index, 101, 124, 8, true, 500, 600); }



	//	if (gMonitorInstance.status) {

	//		keyDown(index, "down");

	//		holdKey(index, JUMP_KEY, 250, 133);

	//		keyUp(index, "down");

	//		ScriptDelay(index, 500);

	//	}



	//	if (*gMonitorInstance.getRuneCoords() < 0) {

	//		if (gMonitorInstance.status) { goTo(index, 52, 138, 6, true, 500, 600); }

	//		if (gMonitorInstance.status) {

	//			holdKey(index, 钩子, randomUniform(925, 975), 133);

	//			ScriptDelay(index, 300);

	//		}

	//	}





	//}



}



void UpperDeckMap(long index) {

	sptool* dm = g_info[index].dm;

	long 放置流程耗时 = 1000;

	#ifdef Adele

	放置流程耗时 = 2500;

	#endif 



	int count = 1;

	if (gMonitorInstance.status) { CheckRoutineBuff(index, false); }



	if (gMonitorInstance.status && dm->GetTime() + 放置流程耗时 < buffTimeOut_buff_黑球) {

		if (gMonitorInstance.status) { goTo(index, 55, 115, 5); }

		if (gMonitorInstance.status) { goTo(index, 55, 115, 5); }

		if (gMonitorInstance.status) { holdKey(index, "left", 200, 133); }

	}



	while (gMonitorInstance.status && dm->GetTime() + 1000 < buffTimeOut_buff_黑球) {

		if (gMonitorInstance.status) { CheckRoutineBuff(index, false); }

		if (gMonitorInstance.status) { CheckUseBuff(index); }

		if (gMonitorInstance.status) { JumpAttack(index, true); }

		count++;

	}



	if (gMonitorInstance.status && dm->GetTime() + 放置流程耗时 > buffTimeOut_buff_黑球)

	{



		int* currentPlayerLocation = gMonitorInstance.getPlayerCoords();

		if (abs(*currentPlayerLocation - 55) <= 12 && abs(*(currentPlayerLocation+1) - 123) <= 3) {

			#ifdef Adele

			ScriptDelay(index, 250);

			if (gMonitorInstance.status) { goTo(index, 42, 115, 6); }

			if (gMonitorInstance.status) { goTo(index, 67, 115, 5); }

			#else 

			if (gMonitorInstance.status) { goTo(index, 67, 115, 6); }

			if (gMonitorInstance.status) { ResistCheck(index); }

			#endif 



		}



		if (gMonitorInstance.status) { goTo(index, 101, 114, 5); }

		if (gMonitorInstance.status) { goTo(index, 101, 114, 3); }

		if (gMonitorInstance.status) { 

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			ScriptDelay(index, 750);

			press(index, JUMP_KEY, 1, 40);

			CheckJanus(index, true, 1); 





		}

		#ifdef Adele

		if (gMonitorInstance.status) { goTo(index, 101, 114, 3); }

		#endif 



		if (gMonitorInstance.status) { goTo(index, 139, 114, 5); }

		if (gMonitorInstance.status) {

			ScriptDelay(index, 100);

			press(index, JUMP_KEY, 1, 40);

			CheckJanus(index, true, 2);

			if (gMonitorInstance.status) { JumpAttack(index, true); }

			ScriptDelay(index, 550);

		}

		if (gMonitorInstance.status) { goTo(index, 145, 88, 8); }

		if (gMonitorInstance.status) { holdKey(index, "left", 400, 133); }



		if (gMonitorInstance.status) { goTo(index, 67, 101, 6); }

		if (gMonitorInstance.status) { CheckFountain(index); }

	}









}



void leftRight(long index, int& count) {

	if (count % 2 == 0) {

		holdKey(index, _T("left"), 400);

	}

	else {

		holdKey(index, _T("right"), 400);

	}

}
