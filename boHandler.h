#pragma once
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "stdafx.h"
#include "log.h"
#include "KyStting.h"
using namespace std;

#define playerIcon _T("C:\\sptool\\Chara.bmp")
#define friendIcon _T("C:\\sptool\\FriendIcon.bmp")
#define guildIcon _T("C:\\sptool\\Guild.bmp")
#define runeIcon _T("C:\\sptool\\Rune.bmp")
#define randomIcon _T("C:\\sptool\\Others.bmp")
#define whiteIcon _T("C:\\sptool\\White.bmp")
#define mapBorderIcon _T("C:\\sptool\\MapBorder.bmp")
#define bossIcon _T("C:\\sptool\\Eb1.bmp")
#define deadOKIcon _T("C:\\sptool\\DeadOK.bmp")
#define bossChestIcon _T("C:\\sptool\\BossChest.bmp")
#define runestateIcon _T("C:\\sptool\\Runestate.bmp")
#define runestateDarkIcon _T("C:\\sptool\\RunestateDark.bmp|C:\\sptool\\RunestateDark2.bmp")
#define enchantportalIcon _T("C:\\sptool\\Enchantportal.bmp")
#define portalIcon _T("C:\\sptool\\Portal.bmp")
#define portalDarkIcon _T("C:\\sptool\\PortalDark.bmp")
#define portalDarkIcon2 _T("C:\\sptool\\PortalDark2.bmp")
#define chatIcon _T("C:\\sptool\\EndChat.bmp")

#define disassembleIcon _T("C:\\sptool\\disassemble.bmp")
#define stoneokIcon _T("C:\\sptool\\stoneok.bmp")
#define okIcon _T("C:\\sptool\\ok.bmp")
#define extractionIcon _T("C:\\sptool\\extraction.bmp")
#define selectAllIcon _T("C:\\sptool\\selectAll.bmp")

#define cashShopExitIcon _T("C:\\sptool\\CashShopExit.bmp")
#define cashShopExitOKIcon _T("C:\\sptool\\CashShopExitOK.bmp")

#define RANDOM_PLAYER_NOTIFICATION 5000
#define FRIEND_PLAYER_NOTIFICATION 20000
#define GUILD_PLAYER_NOTIFICATION 3000
#define WHITE_NOTIFICATION 2000

#define BUFF_TIME 230000  // 蜘蛛
#define BUFF_TIME_2 180000  // 泰山

void gMonitorCheck(long index, long count);
int* findCoordsOnMiniMap(long index, const TCHAR* innerIcon, double sim = 0.95);
int* findPic(long index, long TopLeftX, long TopLeftY, long BottomRightX, long BottomRightY, const TCHAR* innerIcon, double sim = 0.9, long dir = 0);

long randomUniform(long lower, long upper);

void keyDown(long index, const TCHAR* key);
void keyUp(long index, const TCHAR* key);
void holdKey(long index, const TCHAR* key, long holdTime, long interval = 133);
void press(long index, const TCHAR* key, int times = 1, long delay = 0);


void startBo(long index);

bool isInRange(int targetX, int targetY, int* playerCoords, long wantedRange);
void goToDirection(long index, const TCHAR* direction, int distance, bool rune = false);
void goUp(long index, long distance, long wait = 800);
void goDown(long index, long distance, long downoverride);
void goTo(long index, long targetX, long targetY, long rangeFromCoords, bool isRune = false, long upoverride = 500, long downoverride = 600, bool randomWalk = false, long rangeY = 0);

void rightJump(long index);

void startMiao(long index);

void lampMap(long index);
void desertMap(long index);
void birdMap(long index);
void timeMap(long index);
void alienMap(long index);
void coffeeMap(long index);
void mouseMap(long index);
void lionMap(long index);
void rp3Map(long index);
void mtsMap(long index);
void squidMap(long index);
void vc3Map(long index);
void labyMap(long index);
void libMap(long index);
void soldierMap(long index);
void SpringMap(long index);
void ForestMap(long index);
void Forest3Map(long index);
void UpperDeckMap(long index);
void mageMap(long index);
void sss4Map(long index);

void subSoftStartPause();
void subSoftPause();
void subSoftStart();
void detectionStart();
void detectWhiteIcon();

#define BUFF_TIME_手雷 4000  // 
#define BUFF_TIME_小立场 57000  // 
#define BUFF_TIME_飞剑 54000  // 
#define BUFF_TIME_大立场 87000  // 
#define BUFF_TIME_剑雨 11000  // 
#define BUFF_TIME_钻机 30000  // 
#define BUFF_TIME_能量补充 60000  // 
#define BUFF_TIME_代码 90000  // 
#define BUFF_TIME_骰子 90000  // 
#define BUFF_TIME_艾尔达洗澡 57000
#ifdef NL
#define BUFF_TIME_黑球 58000
#elif defined(Adele)
#define BUFF_TIME_黑球 58000
#else 
#define BUFF_TIME_黑球 58000
#endif 

#define BUFF_TIME_进阶女神 180000  // 
#define BUFF_TIME_寂静乱舞 120000  //
#define BUFF_TIME_反抗放置 26000  //
#define BUFF_TIME_轮回 601000
#define BUFF_TIME_捡钱 90000

#define BUFF_TIME_蜘蛛 238000  // 
#define BUFF_TIME_米特拉 238000  // 

#define BUFF_TIME_经验秘药2h 7210000  // 
#define BUFF_TIME_经验秘药30 1810000  // 
#define BUFF_TIME_经验exp30 1808000  // 
#define BUFF_TIME_经验exp20 1208000  // 
#define BUFF_TIME_经验exp10 908000  // 

#define BUFF_TIME_MVP 1806000  // 
#define BUFF_TIME_公园 1808000  // 
