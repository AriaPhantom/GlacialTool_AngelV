#include "stdafx.h"
#include "boHandler.h"
#include "obj.h"
#include "thread_control.h"
#include "gMonitor.h"
#include "runeData.h"
#include "lodepng.h"
#include "MiaoSender.h"
#include "script.h"
#include "RuAdvancing.h"
#include "FileProc.h"
#include <sstream>
#include <iostream>
#include <string>

gMonitor gMonitorInstance = gMonitor();
MiaoSender miaoSenderInstance = MiaoSender();
int rune_fail_time = 0;
int guild_skill_count = 0;
unsigned long oilTimeOut = 0;

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
extern int GetexpPot();
extern int GetautoOil();
extern int Getignite();
extern int GetWhiteDetect();

void clickRune(long index);

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
				send_status = sendMiaoCode(miaoSenderInstance.MiaoCode_boss);
				if (send_status == 200) {
					miaoSenderInstance.send_boss--;
					miaoSenderInstance.cooldown_boss = dm->GetTime();
				}
				ScriptDelay(index, 3000);
			}
		}
		if (miaoSenderInstance.send_others > 0) {
			if (currentTime - miaoSenderInstance.cooldown_others >= 10000) {
				send_status = sendMiaoCode(miaoSenderInstance.MiaoCode_others);
				if (send_status == 200) {
					miaoSenderInstance.send_others--;
					miaoSenderInstance.cooldown_others = dm->GetTime();
				}
				ScriptDelay(index, 3000);
			}
		}
		if (miaoSenderInstance.send_white > 0) {
			if (currentTime - miaoSenderInstance.cooldown_white >= 10000) {
				send_status = sendMiaoCode(miaoSenderInstance.MiaoCode_white);
				if (send_status == 200) {
					miaoSenderInstance.send_white--;
					miaoSenderInstance.cooldown_white = dm->GetTime();
				}
				ScriptDelay(index, 3000);
			}
		}
		if (miaoSenderInstance.send_huangmen > 0) {
			if (currentTime - miaoSenderInstance.cooldown_huangmen >= 10000) {
				send_status = sendMiaoCode(miaoSenderInstance.Miaocode_huangmen);
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

	if (Gethunt() != 1 || gMonitorInstance.detectionStatus == 0)
	{
		ScriptDelay(index, 200);
		return;
	}

	long x, y;
	long borderx, bordery;
	long findPicRet;

	if (gMonitorInstance.whiteIconUpdate > 0) {
		// white icon Coords
		long whiteTopLeftX = 0;
		long whiteTopLeftY = 0;

		long whiteBottomRightX = whiteTopLeftX + 500;
		long whiteBottomRightY = 600;

		int detect_sum = 0;
		//SetTaskState(index - MAX_HWND, _T("地图边界搜索中"));
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

	// Player Coords
	int* currentPlayerLocation = findCoordsOnMiniMap(index, playerIcon);
	if (*(currentPlayerLocation) != -1 && *(currentPlayerLocation + 1) != -1) {
		int* currentWhiteIconCoords = gMonitorInstance.getWhiteIconCoords();
		int relativePlayerLocationX = *(currentPlayerLocation);
		int relativePlayerLocationY = *(currentPlayerLocation + 1) - 3 - *(currentWhiteIconCoords + 1);
		int relativePlayerLocation[2] = { relativePlayerLocationX , relativePlayerLocationY };


		gMonitorInstance.setPlayerCoords(relativePlayerLocationX, relativePlayerLocationY);
		//gui.updateCurrentCoordinate(currentPlayerLocation)  //Update the live coords in gui
		UpdateCoords(relativePlayerLocation);
		//Log(_T("已发现玩家坐标"));
	}


	// 其他玩家
	if (count % 2 == 1) {
		int* randomPlayerCoords = findCoordsOnMiniMap(index, randomIcon);
		if (*(randomPlayerCoords) != -1 && *(randomPlayerCoords + 1) != -1) {

			if (gMonitorInstance.randomPlayerOldPosX != *(randomPlayerCoords) && gMonitorInstance.randomPlayerOldPosY != *(randomPlayerCoords + 1)) {
				gMonitorInstance.randomPlayerOldPosX = *(randomPlayerCoords);
				gMonitorInstance.randomPlayerOldPosY = *(randomPlayerCoords + 1);
				gMonitorInstance.randomPlayerSamePosTimer = dm->GetTime();
			}

			if (gMonitorInstance.getRandomPlayerTimer() == 0) {
				gMonitorInstance.setRandomPlayerTimer(dm->GetTime());
			}
			else {
				if (dm->GetTime() - RANDOM_PLAYER_NOTIFICATION > gMonitorInstance.getRandomPlayerTimer() && (dm->GetTime() - 300000) < gMonitorInstance.randomPlayerSamePosTimer) {
					gMonitorInstance.setRandomPlayerInMap(1);
					//Log(_T("发现其他玩家"));
					miaoSenderInstance.setOthers(1);
				}
			}
		}
		else {
			//Log(_T("未发现其他玩家"));
			gMonitorInstance.setRandomPlayerTimer(0);
			gMonitorInstance.setRandomPlayerInMap(0);
			gMonitorInstance.randomPlayerSamePosTimer = dm->GetTime();
			gMonitorInstance.randomPlayerOldPosX = -1;
			gMonitorInstance.randomPlayerOldPosY = -1;
		}
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

	// 符文查找
	if (GetautoRuneSolver() == 1 && count == 0 && *gMonitorInstance.getRuneCoords() == -1) {
		int* runeCoords = findCoordsOnMiniMap(index, runeIcon, 0.999);
		if (*(runeCoords) != -1 && *(runeCoords + 1) != -1 && *(runeCoords) < gMonitorInstance.mapBorderCoords[0] && *(runeCoords + 1) < gMonitorInstance.mapBorderCoords[1]) {
			int* currentWhiteIconCoords = gMonitorInstance.getWhiteIconCoords();
			int relativeRuneLocationX = *(runeCoords)-2;
			int relativeRuneLocationY = *(runeCoords + 1) - 2 - *(currentWhiteIconCoords + 1);

			if (relativeRuneLocationY > 55) {
				gMonitorInstance.setRuneCoords(relativeRuneLocationX, relativeRuneLocationY);
			}

		}
	}



	// 精英boss检测
	if (count % 20 == 0) {
		long BOSS_WIDTH = 200;

		long bossTopLeftX = long((mapleWindowWidth / 2) - (BOSS_WIDTH / 2));
		long bossTopLeftY = long(20 + (mapleWindowHeight / 2));
		long bossBottomRightX = long((mapleWindowWidth / 2) + (BOSS_WIDTH / 2));
		long bossBottomRightY = long(120 + (mapleWindowHeight / 2));

		findPicRet = dm->FindPic(bossTopLeftX, bossTopLeftY, bossBottomRightX, bossBottomRightY, deadOKIcon, _T("000000"), 0.95, 0, &x, &y);
		if (x > 0 && y > 0) {
			Log(_T("挂了"));
			miaoSenderInstance.setBoss(1);
			subSoftPause();
		}
	}


	// 白屋检测
	if (GetWhiteDetect() && count % 3 == 0) {
		long whiteTopLeftX = 0;
		long whiteTopLeftY = 0;

		long whiteBottomRightX = whiteTopLeftX + 300;
		long whiteBottomRightY = 500;

		findPicRet = dm->FindPic(whiteTopLeftX, whiteTopLeftY, whiteBottomRightX, whiteBottomRightY, whiteIcon, _T("000000"), 0.9, 0, &x, &y);
		//CString tips;
		//tips.Format(_T("白屋坐标:(%d,%d)"), x,y);
		//Log(tips);
		if (x > 0 && y > 0) {
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
					const char* cstr = filePath.c_str();

					dm->CapturePng(0, 0, 2000, 1200, _T(cstr));
				}

				if (dm->GetTime() - WHITE_NOTIFICATION > gMonitorInstance.getWhiteTimer()) {
					subSoftPause();
				}
			}
		}
	}


	// 黄门检测
	if (GethuangMen() && count % 5 == 0) {
		if (*findCoordsOnMiniMap(index, enchantportalIcon) > 0) {
			miaoSenderInstance.setHuangmen(1);
		}
	}
	// 好友检测
	if (GetfriendPlayerNotification() && count % 2 == 0) {
		int random_x, random_y, guild_x, guild_y;

		int* friendPlayerCoords = findCoordsOnMiniMap(index, friendIcon);
		random_x = *(friendPlayerCoords);
		random_y = *(friendPlayerCoords + 1);

		int* guildPlayerCoords = findCoordsOnMiniMap(index, guildIcon);
		guild_x = *(guildPlayerCoords);
		guild_y = *(guildPlayerCoords + 1);

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

	// 其他检测


}

int* findCoordsOnMiniMap(long index, const TCHAR* innerIcon, double sim) {
	sptool* dm = g_info[index].dm;
	long x, y;
	long findPicRet = dm->FindPic(0, 20, 350, 300, innerIcon, _T("000000"), sim, 0, &x, &y);
	static int  results[2];
	if (x > 0 && y > 0)
	{
		//CString tips;
		//tips.Format(_T("玩家坐标:(%d,%d)"), x,y);
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

void goDown(long index, long distance, long downoverride) {
	keyDown(index, "down");
	holdKey(index, JUMP_KEY, 12 * distance, 133);
	keyUp(index, "down");
	ScriptDelay(index, max((long)0, downoverride - 12 * distance));
}

void rightJump(long index) {
	keyDown(index, "right");
	press(index, JUMP_KEY);
	keyUp(index, "right");
	ScriptDelay(index, 500);
}

void goTo(long index, long targetX, long targetY, long rangeFromCoords, bool isRune, long upoverride, long downoverride, bool randomWalk, long rangeY) {
	long WANTED_RANGE = rangeFromCoords;
	long WANTED_RANGEY = max(rangeFromCoords, rangeY);
	int* currentPlayerLocation = gMonitorInstance.getPlayerCoords();
	sptool* dm = g_info[index].dm;
	long startTime = dm->GetTime();
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

				if (dm->GetTime() > 6000 + startTime && gMonitorInstance.status) {
					checkChatPop(index);
				}

				if (dm->GetTime() > 12000 + startTime && gMonitorInstance.status) {
					rightJump(index);
					startTime = dm->GetTime();
				}

				currentPlayerLocation = gMonitorInstance.getPlayerCoords();
				xDistance = targetX - *currentPlayerLocation;
			}

			int yTrial = 0;

			int yDistance = targetY - *(currentPlayerLocation + 1);
			while (abs(yDistance) > WANTED_RANGEY && gMonitorInstance.status > 0) {
				if (yDistance < 0) {
					goUp(index, yDistance, upoverride);
				}
				else {
					goDown(index, abs(yDistance), downoverride);
				}

				if (dm->GetTime() > 6000 + startTime && gMonitorInstance.status) {
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
		if (x + i < width) {

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
		if (y + i < height) {

			long red = (long)image[(y + i) * width * 4 + x * 4];
			long green = (long)image[(y + i) * width * 4 + x * 4 + 1];
			long blue = (long)image[(y + i) * width * 4 + x * 4 + 2];

			if (green >= 200 && 150 < red && red <= 255) {
				return (CString)"down";
			}
		}


	}

	return "";
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

	for (long currentX = 0; currentX < width; currentX++)
	{
		for (long currentY = 0; currentY < height; currentY++)
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

	long topLeftX, topLeftY, bottomRightX, bottomRightY;
	long windowRet = dm->GetWindowRect(g_info[index].hwnd, &topLeftX, &topLeftY, &bottomRightX, &bottomRightY);
	long mapleWindowWidth = bottomRightX - topLeftX;
	long mapleWindowHeight = bottomRightY - topLeftY;
	if (mapleWindowWidth > 1400) {
		mapleWindowWidth = long(mapleWindowWidth / 2);
		mapleWindowHeight = long(mapleWindowHeight / 2);
	}
	long BOSS_WIDTH = 200;

	long bossTopLeftX = long((mapleWindowWidth / 2) - (BOSS_WIDTH / 2));
	long bossTopLeftY = long((mapleWindowHeight / 2) - 20);
	long bossBottomRightX = long((mapleWindowWidth / 2) + (BOSS_WIDTH / 2));
	long bossBottomRightY = long(120 + (mapleWindowHeight / 2));
	long x, y;

	int findPicRet = dm->FindPic(bossTopLeftX, bossTopLeftY, bossBottomRightX, bossBottomRightY, okIcon, _T("000000"), 0.95, 0, &x, &y);
	if (x > 0 && y > 0) {
		press(index, "esc", 1, randomUniform(300, 410));
	}
}

void CheckUseBuff(long index) {

	sptool* dm = g_info[index].dm;


	//消耗品buff
	if (Getignite() & (buffTimeOut_buff_点火 <= dm->GetTime())) {
		dm->SetWindowState(g_info[index].hwnd, 12);
		ScriptDelay(index, 50);
		holdKey(index, 点火, randomUniform(500, 610), 233);
		buffTimeOut_buff_点火 = dm->GetTime() + 1810000;
	}

	if (GetautoOil() & (oilTimeOut <= dm->GetTime())) {
		holdKey(index, 汽油, randomUniform(200, 220));
		oilTimeOut = dm->GetTime() + 3700000;
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
		buffTimeOut_buff_经验exp30 = dm->GetTime() + BUFF_TIME_经验exp30;
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
			if (gMonitorInstance.status) { goTo(index, 17, 127, 5, true); }
			if (gMonitorInstance.status) { ScriptDelay(index, 30); }
			if (gMonitorInstance.status) { goTo(index, 17, 127, 3, true); }
			if (gMonitorInstance.status) { ScriptDelay(index, 30); }
			if (gMonitorInstance.status) { goTo(index, 17, 127, 1, true); }
			if (gMonitorInstance.status) { ScriptDelay(index, 50); }
			if (gMonitorInstance.status) { goTo(index, 17, 127, 0, true); }
			if (gMonitorInstance.status) { ScriptDelay(index, 100); }
			if (gMonitorInstance.status) { goTo(index, 17, 127, 0, true); }
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

		if (gMonitorInstance.status) { goTo(index, 26, 124, 3, true); }
		if (gMonitorInstance.status) { ScriptDelay(index, 30); }
		if (gMonitorInstance.status) { goTo(index, 26, 124, 3, true); }
		if (gMonitorInstance.status) { ScriptDelay(index, 30); }
		if (gMonitorInstance.status) { goTo(index, 26, 124, 1, true); }
		if (gMonitorInstance.status) { ScriptDelay(index, 50); }
		if (gMonitorInstance.status) { goTo(index, 26, 124, 0, true); }
		if (gMonitorInstance.status) { ScriptDelay(index, 100); }
		if (gMonitorInstance.status) { goTo(index, 26, 124, 0, true); }
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
