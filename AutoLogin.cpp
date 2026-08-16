#include "stdafx.h"

#include "AutoLogin.h"
#include "MiaoSender.h"

#include "SPUtils_legacy.h"
#include "SPUtils.h"
#include "thread_control.h"
#include "script.h"
#include "gMonitor.h"
#include "log.h"

#include <atomic>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cwctype>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <shellapi.h>
#include <tlhelp32.h>

#include <curl/curl.h>

extern gMonitor gMonitorInstance;
extern int GetAutoLogin();
extern std::wstring GetAutoLoginKeys();
extern int GetAutoLoginDelay();
extern int GetAutoLoginChannel();
extern int GetAutoLoginMode();
extern int GetAutoRestEnabled();
extern int GetAutoRestRunMinutes();
extern int GetAutoRestRestMinutes();
extern int GetNetchGuard();
extern void subSoftStart();
extern void subSoftPause();
extern MiaoSender miaoSenderInstance;


namespace {

const wchar_t* kLoginIconPath = L"C:\\sptool\\login.bmp";
const wchar_t* kLoginIconFallbackPath = L"C:\\sptool\\login.png";
const wchar_t* kScaniaIconPath = L"C:\\sptool\\scania.bmp";
const wchar_t* kScaniaIconFallbackPath = L"C:\\sptool\\scania.png";
const wchar_t* kPlayIconPath = L"C:\\sptool\\play.bmp";
const wchar_t* kPlayIconFallbackPath = L"C:\\sptool\\play.png";
const wchar_t* kOkLoginIconPath = L"C:\\sptool\\okLogin.bmp";
const wchar_t* kOkLoginIconFallbackPath = L"C:\\sptool\\okLogin.png";
const wchar_t* kSettingIconPath = L"C:\\sptool\\setting.bmp";
const wchar_t* kSettingIconFallbackPath = L"C:\\sptool\\setting.png";
const wchar_t* kBigScaniaIconPath = L"C:\\sptool\\BigScania.bmp";
const wchar_t* kBigScaniaIconFallbackPath = L"C:\\sptool\\BigScania.png";
const wchar_t* kQuickLoginIconPath = L"C:\\sptool\\QuickLogin.bmp";
const wchar_t* kQuickLoginIconFallbackPath = L"C:\\sptool\\QuickLogin.png";
const wchar_t* kStuckIconPath = L"C:\\sptool\\Stuck.bmp";
const wchar_t* kStuckIconFallbackPath = L"C:\\sptool\\Stuck.png";
const wchar_t* kStuck2IconPath = L"C:\\sptool\\Stuck2.bmp";
const wchar_t* kStuck2IconFallbackPath = L"C:\\sptool\\Stuck2.png";
const double kQuickLoginSim = 0.985;
constexpr int kDisconnectWatcherActiveIntervalMs = 500;
constexpr int kDisconnectWatcherIdleIntervalMs = 1000;
constexpr long kDisconnectWatcherBaseCheckIntervalMs = 1000;
constexpr long kDisconnectWatcherImageCheckIntervalMs = 3000;
constexpr ULONGLONG kWhiteIconRecentTtlMs = 1000;
constexpr int kWhiteIconGateRight = 350;
constexpr int kWhiteIconGateBottom = 300;
constexpr long kNetchGuardCheckIntervalMs = 1000;
constexpr long kNetchGuardSoftPauseMs = 5000;
constexpr long kNetchGuardHardCloseMs = 10000;
constexpr long kNetchGuardConnectTimeoutMs = 800;
constexpr long kNetchGuardTimeoutMs = 1500;
constexpr long kIconHintTtlMs = 2000;
constexpr int kIconHintHalfWidth = 220;
constexpr int kIconHintHalfHeight = 150;
constexpr int kIconHintMinSearchArea = 500 * 320;
const wchar_t* kLaunchCommand = L"nxl://launch/10100";

std::atomic<bool> g_loginRunning[MAX_HWND];
std::atomic<long> g_lastCheckMs[MAX_HWND];
std::atomic<long> g_lastImageCheckMs[MAX_HWND];
std::atomic<ULONGLONG> g_lastWhiteIconSeenTick[MAX_HWND];
std::atomic<long> g_loginPendingSinceMs[MAX_HWND];
std::atomic<bool> g_loginNeedRestart[MAX_HWND];
std::atomic<bool> g_forceRelaunch[MAX_HWND];
std::atomic<unsigned long long> g_disconnectWatcherGeneration[MAX_HWND];
std::atomic<unsigned long long> g_autoRestAccumulatedMs[MAX_HWND];
std::atomic<unsigned long long> g_autoRestLastActiveTickMs[MAX_HWND];
std::atomic<unsigned long long> g_autoRestUntilTickMs[MAX_HWND];
std::atomic<int> g_stuckScreenConsecutiveCount[MAX_HWND];
std::atomic<int> g_disconnectDialogConsecutiveCount[MAX_HWND];
std::atomic<long> g_netchLastCheckMs[MAX_HWND];
std::atomic<long> g_netchDownSinceMs[MAX_HWND];
std::atomic<bool> g_netchSoftPaused[MAX_HWND];
std::atomic<bool> g_netchHardClosed[MAX_HWND];
struct IconFindHint {
bool valid = false;
std::wstring iconPath;
long x = -1;
long y = -1;
long width = 0;
long height = 0;
long tickMs = 0;
};
std::mutex g_iconFindHintMutex;
std::mutex g_disconnectWatcherActionMutex;
IconFindHint g_iconFindHints[MAX_HWND];
bool IsDisconnectWatcherCurrent(long mainIndex, unsigned long long generation) {
	if (generation == 0) return true;
	return mainIndex >= 0 && mainIndex < MAX_HWND &&
		g_disconnectWatcherGeneration[mainIndex].load() == generation &&
		!g_info[mainIndex].is_stop;
}
void TriggerAutoLogin(long mainIndex, unsigned long long generation = 0);
void KillMapleStoryProcesses();

size_t NetchGuardDiscardWrite(char* ptr, size_t size, size_t nmemb, void* userdata) {
	UNREFERENCED_PARAMETER(ptr);
	UNREFERENCED_PARAMETER(userdata);
	return size * nmemb;
}

void ResetNetchGuardState(long mainIndex) {
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
	g_netchLastCheckMs[mainIndex].store(0);
	g_netchDownSinceMs[mainIndex].store(0);
	g_netchSoftPaused[mainIndex].store(false);
	g_netchHardClosed[mainIndex].store(false);
}

bool ProbeNetchEndpoint(const char* url) {
	CURL* curl = curl_easy_init();
	if (curl == nullptr) return false;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:2801");
	curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, kNetchGuardConnectTimeoutMs);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, kNetchGuardTimeoutMs);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

	CURLcode code = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return code == CURLE_OK;
}

bool IsNetchSocksHealthy() {
	static std::once_flag curlInitFlag;
	std::call_once(curlInitFlag, []() {
		curl_global_init(CURL_GLOBAL_DEFAULT);
	});

	// Probe real MapleStory Scania game-server endpoints through Netch.
	// maple.watch currently checks Scania channel 9 at 34.215.85.101:8585 and channel 11 at 54.191.254.95:8585.
	// A local-only 127.0.0.1:2801 listener is not enough; either game endpoint must be reachable through the SOCKS path.
	const char* endpoints[] = {
		"http://34.215.85.101:8585/",
		"http://54.191.254.95:8585/",
	};
	for (const char* endpoint : endpoints) {
		if (ProbeNetchEndpoint(endpoint)) {
			return true;
		}
	}
	return false;
}

bool HandleNetchGuard(long mainIndex, unsigned long long generation = 0) {
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return true;
	if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return false;
	if (g_info[mainIndex].is_stop) return true;

	if (!GetNetchGuard()) {
		std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
		if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return false;
		bool softPaused = g_netchSoftPaused[mainIndex].exchange(false);
		bool hardClosed = g_netchHardClosed[mainIndex].exchange(false);
		g_netchDownSinceMs[mainIndex].store(0);
		g_netchLastCheckMs[mainIndex].store(0);
		if (softPaused && !hardClosed) {
			Log(_T("[NETCH] guard disabled, soft start idx=%ld"), mainIndex);
			subSoftStart();
		}
		return true;
	}

	long nowMs = GetTime();
	long lastCheckMs = g_netchLastCheckMs[mainIndex].load();
	if (nowMs - lastCheckMs < kNetchGuardCheckIntervalMs) {
		return !g_netchSoftPaused[mainIndex].load() && !g_netchHardClosed[mainIndex].load();
	}
	g_netchLastCheckMs[mainIndex].store(nowMs);

	bool healthy = IsNetchSocksHealthy();
	if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return false;
	nowMs = GetTime();
	if (healthy) {
		long downSinceMs = g_netchDownSinceMs[mainIndex].exchange(0);
		bool softPaused = g_netchSoftPaused[mainIndex].exchange(false);
		bool hardClosed = g_netchHardClosed[mainIndex].exchange(false);
		long downMs = (downSinceMs > 0 && nowMs >= downSinceMs) ? (nowMs - downSinceMs) : 0;

		if (hardClosed) {
			std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
			if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return false;
			Log(_T("[NETCH] Scania probe recovered after hard close idx=%ld downMs=%ld"), mainIndex, downMs);
			SetTaskState(mainIndex, _T("NETCH RECOVER"));
			g_forceRelaunch[mainIndex].store(true);
			TriggerAutoLogin(mainIndex, generation);
			return false;
		}

		if (softPaused) {
			std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
			if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return false;
			Log(_T("[NETCH] Scania probe recovered, soft start idx=%ld downMs=%ld"), mainIndex, downMs);
			SetTaskState(mainIndex, _T("NETCH OK"));
			subSoftStart();
		}
		return true;
	}

	long downSinceMs = g_netchDownSinceMs[mainIndex].load();
	if (downSinceMs <= 0) {
		downSinceMs = nowMs;
		g_netchDownSinceMs[mainIndex].store(downSinceMs);
		Log(_T("[NETCH] Scania probe via SOCKS 127.0.0.1:2801 down idx=%ld"), mainIndex);
	}

	long downMs = nowMs >= downSinceMs ? (nowMs - downSinceMs) : 0;
	if (downMs >= kNetchGuardHardCloseMs) {
		std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
		if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return false;
		if (!g_netchHardClosed[mainIndex].exchange(true)) {
			g_netchSoftPaused[mainIndex].store(true);
			subSoftPause();
			SetTaskState(mainIndex, _T("NETCH WAIT"));
			Log(_T("[NETCH] Scania probe down %ldms, kill Maple and wait recovery idx=%ld"), downMs, mainIndex);
			KillMapleStoryProcesses();
			g_forceRelaunch[mainIndex].store(true);
		}
		return false;
	}

	if (downMs >= kNetchGuardSoftPauseMs) {
		std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
		if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return false;
		if (!g_netchSoftPaused[mainIndex].exchange(true)) {
			subSoftPause();
			SetTaskState(mainIndex, _T("NETCH PAUSE"));
			Log(_T("[NETCH] Scania probe down %ldms, soft pause idx=%ld"), downMs, mainIndex);
		}
		return false;
	}

	return true;
}

bool ContainsIgnoreCase(const std::wstring& value, const std::wstring& needle) {
if (value.empty() || needle.empty()) return false;
auto lower = [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); };
std::wstring v;
v.reserve(value.size());
for (wchar_t ch : value) v.push_back(lower(ch));
std::wstring n;
n.reserve(needle.size());
for (wchar_t ch : needle) n.push_back(lower(ch));
return v.find(n) != std::wstring::npos;
}

void NotifyDisconnectMiao(const char* reason) {
if (reason == nullptr || *reason == '\0') return;

const std::string code =
	!miaoSenderInstance.MiaoCode_others.empty() ? miaoSenderInstance.MiaoCode_others :
	!miaoSenderInstance.MiaoCode_boss.empty() ? miaoSenderInstance.MiaoCode_boss :
	!miaoSenderInstance.MiaoCode_white.empty() ? miaoSenderInstance.MiaoCode_white :
	miaoSenderInstance.Miaocode_huangmen;
if (code.empty()) return;
const std::string reasonText(reason);

std::thread([code, reasonText]() {
	sendMiaoCodeByType(code, "others", reasonText);
}).detach();
}


std::wstring GetDesktopPath() {
	wchar_t* userProfile = nullptr;
	size_t len = 0;
	if (_wdupenv_s(&userProfile, &len, L"USERPROFILE") == 0 && userProfile != nullptr) {
		std::wstring desktop = std::wstring(userProfile) + L"\\Desktop";
		free(userProfile);
		return desktop;
	}
	return L"C:\\Users\\AriaP\\Desktop";
}

struct DisconnectDialogHit {
	DWORD pid;
	bool found;
	HWND hwnd;
};

BOOL CALLBACK EnumDisconnectDialogProc(HWND hwnd, LPARAM lParam) {
	DisconnectDialogHit* hit = reinterpret_cast<DisconnectDialogHit*>(lParam);
	if (!hit) return TRUE;

	wchar_t cls[64] = {};
	if (!GetClassNameW(hwnd, cls, static_cast<int>(std::size(cls)))) return TRUE;
	if (wcscmp(cls, L"#32770") != 0) return TRUE;

	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	if (hit->pid != 0 && pid != hit->pid) return TRUE;

	wchar_t title[256] = {};
	GetWindowTextW(hwnd, title, static_cast<int>(std::size(title)));
	std::wstring titleStr(title);
	if (!ContainsIgnoreCase(titleStr, L"MapleStory")) return TRUE;

	hit->found = true;
	hit->hwnd = hwnd;
	return TRUE;
}

HWND DismissDisconnectDialog(long mainIndex) {
	DisconnectDialogHit hit = {};
	hit.pid = static_cast<DWORD>(g_info[mainIndex].pid);
	hit.found = false;
	hit.hwnd = nullptr;
	EnumWindows(EnumDisconnectDialogProc, reinterpret_cast<LPARAM>(&hit));
	return hit.hwnd;
}

long NormalizeMainIndex(long index) {
if (index < 0) return -1;
if (index >= MAX_HWND * 2) index -= MAX_HWND * 2;
if (index >= MAX_HWND) index -= MAX_HWND;
return index;
}

void ResetAutoRestState(long mainIndex, bool clearRestUntil = true) {
if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
g_autoRestAccumulatedMs[mainIndex].store(0);
g_autoRestLastActiveTickMs[mainIndex].store(0);
if (clearRestUntil) {
	g_autoRestUntilTickMs[mainIndex].store(0);
}
}

bool TryGetAutoRestConfig(int& runMinutes, int& restMinutes) {
if (!GetAutoLogin()) return false;
if (!GetAutoRestEnabled()) return false;

runMinutes = GetAutoRestRunMinutes();
restMinutes = GetAutoRestRestMinutes();
if (runMinutes <= 0 || restMinutes <= 0) return false;
if (runMinutes > 1440) runMinutes = 1440;
if (restMinutes > 1440) restMinutes = 1440;
return true;
}

void ClearWindowForAllThreads(long mainIndex) {
const long offsets[] = { 0, MAX_HWND, MAX_HWND * 2 };
for (long offset : offsets) {
	long idx = mainIndex + offset;
	if (idx < 0 || idx >= MAX_HWND * 3) continue;
	g_info[idx].hwnd = 0;
	g_info[idx].pid = 0;
}
}

void BeginAutoRest(long mainIndex, unsigned long long nowTick, unsigned long long restMs,
	unsigned long long generation = 0) {
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
	std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
	if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return;
ResetAutoRestState(mainIndex, false);
g_autoRestUntilTickMs[mainIndex].store(nowTick + restMs);
g_forceRelaunch[mainIndex].store(true);
subSoftPause();
SPUtils::ReleaseAllKeysFastKeyboardOnlySkipEnter();
SetTaskState(mainIndex, _T("REST"));
KillMapleStoryProcesses();
ClearWindowForAllThreads(mainIndex);
}

bool HandleAutoRest(long mainIndex, unsigned long long generation = 0) {
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return false;
	if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return true;

int runMinutes = 0;
int restMinutes = 0;
if (!TryGetAutoRestConfig(runMinutes, restMinutes)) {
	ResetAutoRestState(mainIndex);
	return false;
}

unsigned long long nowTick = GetTickCount64();
unsigned long long restUntilTick = g_autoRestUntilTickMs[mainIndex].load();
if (restUntilTick > nowTick) {
	ResetAutoRestState(mainIndex, false);
	return true;
}

if (restUntilTick != 0) {
	g_autoRestUntilTickMs[mainIndex].store(0);
	ResetAutoRestState(mainIndex, false);
	TriggerAutoLogin(mainIndex, generation);
	return true;
}

if (g_loginRunning[mainIndex].load()) {
	g_autoRestLastActiveTickMs[mainIndex].store(0);
	return false;
}

const bool activeEligible =
	!g_info[mainIndex].is_stop &&
	!g_info[mainIndex].is_pause &&
	g_info[mainIndex].handle != NULL &&
	g_info[mainIndex].thread_state == State_Runing &&
	gMonitorInstance.status == 1;

if (!activeEligible) {
	g_autoRestLastActiveTickMs[mainIndex].store(0);
	return false;
}

unsigned long long lastTick = g_autoRestLastActiveTickMs[mainIndex].load();
if (lastTick == 0 || nowTick < lastTick) {
	g_autoRestLastActiveTickMs[mainIndex].store(nowTick);
	return false;
}

unsigned long long deltaMs = nowTick - lastTick;
g_autoRestLastActiveTickMs[mainIndex].store(nowTick);
unsigned long long accumulatedMs = g_autoRestAccumulatedMs[mainIndex].load() + deltaMs;
g_autoRestAccumulatedMs[mainIndex].store(accumulatedMs);

const unsigned long long runLimitMs = static_cast<unsigned long long>(runMinutes) * 60ULL * 1000ULL;
const unsigned long long restMs = static_cast<unsigned long long>(restMinutes) * 60ULL * 1000ULL;
if (accumulatedMs < runLimitMs) {
	return false;
}

BeginAutoRest(mainIndex, nowTick, restMs, generation);
return true;
}

bool FileExists(const std::wstring& path) {
DWORD attrs = GetFileAttributesW(path.c_str());
return attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
}

std::wstring GetNexonLauncherPath() {
static std::wstring cached;
if (!cached.empty()) return cached;

const wchar_t* kCandidates[] = {
	L"C:\\Program Files (x86)\\Nexon\\Nexon Launcher\\nexon_launcher.exe",
	L"E:\\Program Files (x86)\\Nexon\\Nexon Launcher\\nexon_launcher.exe",
};
for (const auto* candidate : kCandidates) {
	if (FileExists(candidate)) {
		cached = candidate;
		break;
	}
}
return cached;
}

std::wstring Trim(const std::wstring& value) {
if (value.empty()) return std::wstring();
const wchar_t* spaces = L" \t\r\n";
size_t start = value.find_first_not_of(spaces);
if (start == std::wstring::npos) return std::wstring();
size_t end = value.find_last_not_of(spaces);
return value.substr(start, end - start + 1);
}

struct KeyAction {
std::wstring key;
std::vector<std::wstring> holdKeys;
int times;
bool ctrl;
bool alt;
bool shift;
bool win;
};

std::wstring ToLower(const std::wstring& value) {
std::wstring out = value;
for (auto& ch : out) {
	ch = static_cast<wchar_t>(std::towlower(ch));
}
return out;
}

bool ParseCountSuffix(std::wstring& token, int& timesOut) {
timesOut = 1;
if (token.size() < 2) return false;
size_t sep = token.find_last_of(L"*xX");
if (sep == std::wstring::npos || sep == 0 || sep + 1 >= token.size()) {
	return false;
}
std::wstring countPart = Trim(token.substr(sep + 1));
if (countPart.empty()) return false;
for (wchar_t ch : countPart) {
	if (ch < L'0' || ch > L'9') {
		return false;
	}
}
timesOut = _wtoi(countPart.c_str());
if (timesOut <= 0) timesOut = 1;
token = Trim(token.substr(0, sep));
return true;
}

bool ApplyModifier(const std::wstring& partLower, KeyAction& action) {
if (partLower == L"ctrl" || partLower == L"control") {
	action.ctrl = true;
	return true;
}
if (partLower == L"alt") {
	action.alt = true;
	return true;
}
if (partLower == L"shift") {
	action.shift = true;
	return true;
}
if (partLower == L"win" || partLower == L"lwin" || partLower == L"rwin") {
	action.win = true;
	return true;
}
return false;
}

std::vector<KeyAction> ParseKeyActions(const std::wstring& raw) {
std::vector<KeyAction> actions;
std::wstring token;
auto pushToken = [&]() {
	std::wstring trimmed = Trim(token);
	token.clear();
	if (trimmed.empty()) return;

	int times = 1;
	ParseCountSuffix(trimmed, times);
	if (trimmed.empty()) return;

	KeyAction action = {};
	action.times = times;
	action.holdKeys.clear();
	action.ctrl = false;
	action.alt = false;
	action.shift = false;
	action.win = false;

	std::wstring part;
	std::vector<std::wstring> keyParts;
	for (wchar_t ch : trimmed) {
		if (ch == L'+') {
			std::wstring chunk = Trim(part);
			part.clear();
			if (chunk.empty()) continue;
			std::wstring lower = ToLower(chunk);
			if (!ApplyModifier(lower, action)) {
				keyParts.push_back(lower);
			}
		} else {
			part.push_back(ch);
		}
	}
	std::wstring chunk = Trim(part);
	if (!chunk.empty()) {
		std::wstring lower = ToLower(chunk);
		if (!ApplyModifier(lower, action)) {
			keyParts.push_back(lower);
		}
	}

	if (!keyParts.empty()) {
		action.key = keyParts.back();
		if (keyParts.size() > 1) {
			action.holdKeys.assign(keyParts.begin(), keyParts.end() - 1);
		}
	}

	if (!action.key.empty()) {
		actions.push_back(action);
	}
};

for (wchar_t ch : raw) {
	if (ch == L',' || ch == L';' || ch == L'|' || ch == L' ' || ch == L'\t') {
		pushToken();
	} else {
		token.push_back(ch);
	}
}
pushToken();
return actions;
}

int ClampDelay(int delay) {
if (delay < 0) return 0;
if (delay > 5000) return 5000;
return delay;
}

int ClampChannel(int channel) {
if (channel < 1) return 1;
if (channel > 30) return 30;
return channel;
}

void PressAutoLoginKey(long mainIndex, const wchar_t* key, int times, long delay) {
if (key == nullptr || *key == L'\0' || times <= 0) return;
ActivateWindow(mainIndex);
std::this_thread::sleep_for(std::chrono::milliseconds(60));
SPUtils::KeyPress(std::wstring(key), times, delay);
}

void RunPostLoginActions(long mainIndex) {
std::wstring keys = Trim(GetAutoLoginKeys());
if (keys.empty()) return;

int delay = ClampDelay(GetAutoLoginDelay());
auto actions = ParseKeyActions(keys);
if (actions.empty()) return;

for (const auto& action : actions) {
	if (action.ctrl) keyDown(mainIndex, L"ctrl");
	if (action.alt) keyDown(mainIndex, L"alt");
	if (action.shift) keyDown(mainIndex, L"shift");
	if (action.win) keyDown(mainIndex, L"win");
	for (const auto& holdKey : action.holdKeys) {
		if (!holdKey.empty()) {
			keyDown(mainIndex, holdKey.c_str());
		}
	}

	for (int i = 0; i < action.times; ++i) {
		press(mainIndex, action.key.c_str(), 1, delay);
	}

	for (auto it = action.holdKeys.rbegin(); it != action.holdKeys.rend(); ++it) {
		if (!it->empty()) {
			keyUp(mainIndex, it->c_str());
		}
	}
	if (action.win) keyUp(mainIndex, L"win");
	if (action.shift) keyUp(mainIndex, L"shift");
	if (action.alt) keyUp(mainIndex, L"alt");
	if (action.ctrl) keyUp(mainIndex, L"ctrl");
}
}

bool IsProcessNameMatch(const wchar_t* exeName, const wchar_t* target) {
return exeName && target && _wcsicmp(exeName, target) == 0;
}

bool IsAnyProcessRunning(const wchar_t* const* names, size_t count) {
HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
if (snapshot == INVALID_HANDLE_VALUE) return false;

PROCESSENTRY32W entry = {};
entry.dwSize = sizeof(entry);
if (!Process32FirstW(snapshot, &entry)) {
	CloseHandle(snapshot);
	return false;
}

bool found = false;
do {
	for (size_t i = 0; i < count; ++i) {
		if (IsProcessNameMatch(entry.szExeFile, names[i])) {
			found = true;
			break;
		}
	}
	if (found) break;
} while (Process32NextW(snapshot, &entry));

CloseHandle(snapshot);
return found;
}

void KillProcessesByName(const wchar_t* const* names, size_t count) {
HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
if (snapshot == INVALID_HANDLE_VALUE) return;

PROCESSENTRY32W entry = {};
entry.dwSize = sizeof(entry);
if (!Process32FirstW(snapshot, &entry)) {
	CloseHandle(snapshot);
	return;
}

do {
	for (size_t i = 0; i < count; ++i) {
		if (!IsProcessNameMatch(entry.szExeFile, names[i])) continue;
		HANDLE proc = OpenProcess(PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
		if (proc) {
			TerminateProcess(proc, 0);
			CloseHandle(proc);
		}
	}
} while (Process32NextW(snapshot, &entry));

CloseHandle(snapshot);
}

void KillMapleStoryProcesses() {
const wchar_t* targets[] = {
	L"MapleStory.exe",
	L"nexon_runtime.exe",
	L"nexon_client.exe",
};
KillProcessesByName(targets, sizeof(targets) / sizeof(targets[0]));
}

void KillNexonProcesses() {
const wchar_t* targets[] = {
	L"nexon_launcher.exe",
	L"nexon_runtime.exe",
	L"nexon_client.exe",
};
KillProcessesByName(targets, sizeof(targets) / sizeof(targets[0]));
}

bool GetProcessNameByPid(DWORD pid, std::wstring& outName) {
outName.clear();
HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
if (snapshot == INVALID_HANDLE_VALUE) return false;

PROCESSENTRY32W entry = {};
entry.dwSize = sizeof(entry);
if (!Process32FirstW(snapshot, &entry)) {
	CloseHandle(snapshot);
	return false;
}

bool found = false;
do {
	if (entry.th32ProcessID == pid) {
		outName = entry.szExeFile;
		found = true;
		break;
	}
} while (Process32NextW(snapshot, &entry));

CloseHandle(snapshot);
return found;
}

bool IsNexonProcessName(const std::wstring& name) {
if (name.empty()) return false;
return _wcsicmp(name.c_str(), L"nexon_launcher.exe") == 0
	|| _wcsicmp(name.c_str(), L"nexon_runtime.exe") == 0
	|| _wcsicmp(name.c_str(), L"nexon_client.exe") == 0;
}

BOOL CALLBACK CleanupNexonWindowProc(HWND hwnd, LPARAM) {
if (!IsWindow(hwnd)) return TRUE;
if (!IsWindowVisible(hwnd)) return TRUE;

DWORD pid = 0;
GetWindowThreadProcessId(hwnd, &pid);
if (pid == 0) return TRUE;

std::wstring procName;
if (!GetProcessNameByPid(pid, procName)) return TRUE;
if (!IsNexonProcessName(procName)) return TRUE;

wchar_t className[64] = {};
if (GetClassNameW(hwnd, className, static_cast<int>(sizeof(className) / sizeof(className[0])))
	&& wcscmp(className, L"#32770") == 0) {
	PostMessageW(hwnd, WM_CLOSE, 0, 0);
	return TRUE;
}

ShowWindow(hwnd, SW_HIDE);
return TRUE;
}

void CleanupNexonWindows() {
EnumWindows(CleanupNexonWindowProc, 0);
}

void CleanupNexonWindowsFor(std::chrono::milliseconds duration) {
const auto end = std::chrono::steady_clock::now() + duration;
do {
	CleanupNexonWindows();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
} while (std::chrono::steady_clock::now() < end);
CleanupNexonWindows();
}

void UpdateWindowForAllThreads(long mainIndex, HWND hwnd) {
long hwndLong = static_cast<long>(reinterpret_cast<intptr_t>(hwnd));
long pid = GetWindowProcessId(hwndLong);
const long offsets[] = { 0, MAX_HWND, MAX_HWND * 2 };
for (long offset : offsets) {
	long idx = mainIndex + offset;
	if (idx < 0 || idx >= MAX_HWND * 3) continue;
	g_info[idx].hwnd = hwndLong;
	g_info[idx].pid = pid;
}
}

bool EnsureWindowBinding(long mainIndex) {
HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
if (hwnd && IsWindow(hwnd)) return true;

HWND found = findMSWindow();
if (found && IsWindow(found)) {
	UpdateWindowForAllThreads(mainIndex, found);
	g_loginNeedRestart[mainIndex].store(true);
	return true;
}

return false;
}

bool GetWindowSizeForSearch(long mainIndex, long& outWidth, long& outHeight) {
long x1 = 0, y1 = 0, x2 = 0, y2 = 0;
if (!GetWindowRectFromAPI(g_info[mainIndex].hwnd, &x1, &y1, &x2, &y2)) return false;
long width = x2 - x1;
long height = y2 - y1;
if (width > 1400) {
	width = static_cast<long>(width / 2);
	height = static_cast<long>(height / 2);
}
outWidth = width;
outHeight = height;
return true;
}

bool TryGetIconHint(long mainIndex, const std::wstring& iconPath, long width, long height, long& outX, long& outY) {
if (mainIndex < 0 || mainIndex >= MAX_HWND) return false;
long nowMs = GetTime();
std::lock_guard<std::mutex> lock(g_iconFindHintMutex);
IconFindHint& hint = g_iconFindHints[mainIndex];
if (!hint.valid) return false;
if (hint.iconPath != iconPath) return false;
if (hint.width != width || hint.height != height) return false;
if (nowMs < hint.tickMs || (nowMs - hint.tickMs) > kIconHintTtlMs) return false;
outX = hint.x;
outY = hint.y;
return outX >= 0 && outY >= 0;
}

void UpdateIconHint(long mainIndex, const std::wstring& iconPath, long width, long height, long x, long y) {
if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
std::lock_guard<std::mutex> lock(g_iconFindHintMutex);
IconFindHint& hint = g_iconFindHints[mainIndex];
hint.valid = true;
hint.iconPath = iconPath;
hint.x = x;
hint.y = y;
hint.width = width;
hint.height = height;
hint.tickMs = GetTime();
}

bool FindIconInWindow(long mainIndex, const std::wstring& iconPath, double sim, long& outx, long& outy) {
long width = 0, height = 0;
if (!GetWindowSizeForSearch(mainIndex, width, height)) return false;

const long fullArea = width * height;
if (fullArea >= kIconHintMinSearchArea) {
	long hintX = -1;
	long hintY = -1;
	if (TryGetIconHint(mainIndex, iconPath, width, height, hintX, hintY)) {
		int roiX1 = static_cast<int>(std::max(0L, hintX - kIconHintHalfWidth));
		int roiY1 = static_cast<int>(std::max(0L, hintY - kIconHintHalfHeight));
		int roiX2 = static_cast<int>(std::min(width, hintX + kIconHintHalfWidth));
		int roiY2 = static_cast<int>(std::min(height, hintY + kIconHintHalfHeight));
		if (roiX2 > roiX1 + 24 && roiY2 > roiY1 + 24) {
			if (findPicWithOpenCV(mainIndex, roiX1, roiY1, roiX2, roiY2, iconPath, sim, outx, outy)) {
				UpdateIconHint(mainIndex, iconPath, width, height, outx, outy);
				return true;
			}
		}
	}
}

if (findPicWithOpenCV(mainIndex, 0, 0, static_cast<int>(width), static_cast<int>(height), iconPath, sim, outx, outy)) {
	UpdateIconHint(mainIndex, iconPath, width, height, outx, outy);
	return true;
}
return false;
}

bool FindIconInWindowBmpFirst(long mainIndex, const wchar_t* bmpPath, const wchar_t* pngPath, double sim, long& outx, long& outy) {
// Search both templates in a single engine pass ("bmp|png"): one capture and
// one coarse gate per call instead of two independent full-window searches
// when the first template misses (and one batched CPFindPicN in external mode).
std::wstring joined;
if (bmpPath && FileExists(bmpPath)) joined = bmpPath;
if (pngPath && FileExists(pngPath)) {
	if (!joined.empty()) joined += L"|";
	joined += pngPath;
}
if (joined.empty()) return false;
return FindIconInWindow(mainIndex, joined, sim, outx, outy);
}

bool WaitForIcon(long mainIndex, const std::wstring& iconPath, int timeoutMs, double sim,
long& outx, long& outy, const std::chrono::steady_clock::time_point& deadline) {
auto start = std::chrono::steady_clock::now();
while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(timeoutMs)) {
	if (std::chrono::steady_clock::now() > deadline) return false;
	if (FindIconInWindow(mainIndex, iconPath, sim, outx, outy)) return true;
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
return false;
}

bool WaitForIconBmpFirst(long mainIndex, const wchar_t* bmpPath, const wchar_t* pngPath, int timeoutMs, double sim,
long& outx, long& outy, const std::chrono::steady_clock::time_point& deadline) {
auto start = std::chrono::steady_clock::now();
while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(timeoutMs)) {
	if (std::chrono::steady_clock::now() > deadline) return false;
	if (FindIconInWindowBmpFirst(mainIndex, bmpPath, pngPath, sim, outx, outy)) return true;
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
return false;
}

bool WaitForStableWhite(long mainIndex, int timeoutMs, int stableMs, double sim,
long& outx, long& outy, const std::chrono::steady_clock::time_point& deadline,
int pollMs = 200, int initialDelayMs = 0) {
const auto start = std::chrono::steady_clock::now();
const int safePollMs = std::max(10, pollMs);
if (initialDelayMs > 0) {
	auto delayStart = std::chrono::steady_clock::now();
	while (std::chrono::steady_clock::now() - delayStart < std::chrono::milliseconds(initialDelayMs)) {
		if (std::chrono::steady_clock::now() > deadline) return false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
bool whiteVisible = false;
std::chrono::steady_clock::time_point whiteSince = std::chrono::steady_clock::now();

while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(timeoutMs)) {
	if (std::chrono::steady_clock::now() > deadline) return false;

	long x = -1, y = -1;
	if (FindIconInWindow(mainIndex, whiteIcon, sim, x, y)) {
		if (!whiteVisible) {
			whiteVisible = true;
			whiteSince = std::chrono::steady_clock::now();
		}
		outx = x;
		outy = y;
		auto stableElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - whiteSince).count();
		if (stableElapsed >= stableMs) {
			return true;
		}
	}
	else {
		whiteVisible = false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(safePollMs));
}

return false;
}

bool WaitForPlayOrWhite(long mainIndex, int timeoutMs, long& outx, long& outy, bool& playFound, bool& whiteFound,
const std::chrono::steady_clock::time_point& deadline) {
playFound = false;
whiteFound = false;
auto start = std::chrono::steady_clock::now();
bool playVisible = false;
bool whiteVisible = false;
std::chrono::steady_clock::time_point playSince = start;
std::chrono::steady_clock::time_point whiteSince = start;
const int playStableMs = 900;
const int whiteStableMs = 500;
while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(timeoutMs)) {
	if (std::chrono::steady_clock::now() > deadline) return false;
	long px = -1, py = -1;
	if (FindIconInWindowBmpFirst(mainIndex, kPlayIconPath, kPlayIconFallbackPath, 0.9, px, py)) {
		if (!playVisible) {
			playVisible = true;
			playSince = std::chrono::steady_clock::now();
		}
		auto playStableElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - playSince).count();
		if (playStableElapsed >= playStableMs) {
			playFound = true;
			outx = px;
			outy = py;
			return true;
		}
	}
	else {
		playVisible = false;
	}

	long wx = -1, wy = -1;
	if (FindIconInWindow(mainIndex, whiteIcon, 0.9, wx, wy)) {
		if (!whiteVisible) {
			whiteVisible = true;
			whiteSince = std::chrono::steady_clock::now();
		}
		auto whiteStableElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - whiteSince).count();
		if (whiteStableElapsed >= whiteStableMs) {
			whiteFound = true;
			outx = wx;
			outy = wy;
			return true;
		}
	}
	else {
		whiteVisible = false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
return false;
}

bool TryQuickLoginClick(long mainIndex) {
long x = -1, y = -1;
if (!FindIconInWindowBmpFirst(mainIndex, kQuickLoginIconPath, kQuickLoginIconFallbackPath, kQuickLoginSim, x, y)) {
	return false;
}

HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
if (hwnd && IsWindow(hwnd)) {
	SPUtils::LeftClick(hwnd, static_cast<int>(x), static_cast<int>(y), 2, 150, 100);
	SPUtils::MoveTo(hwnd, 10, 10, 80);
}
return true;
}

bool TryQuickLoginUntilWhite(long mainIndex, const std::chrono::steady_clock::time_point& deadline) {
if (GetAutoLoginMode() != 1) return false;

const auto start = std::chrono::steady_clock::now();
while (std::chrono::steady_clock::now() - start < std::chrono::seconds(120)) {
	if (std::chrono::steady_clock::now() > deadline) return false;

	long wx = -1, wy = -1;
	if (FindIconInWindow(mainIndex, whiteIcon, 0.9, wx, wy)) {
		return true;
	}

	TryQuickLoginClick(mainIndex);
	std::this_thread::sleep_for(std::chrono::seconds(2));
}
return false;
}

bool SelectChannel(long mainIndex, const std::chrono::steady_clock::time_point& deadline) {
long x0 = -1, y0 = -1;
if (!WaitForIconBmpFirst(mainIndex, kBigScaniaIconPath, kBigScaniaIconFallbackPath, 20000, 0.9, x0, y0, deadline)) {
	return false;
}

int channel = ClampChannel(GetAutoLoginChannel());
int col = (channel - 1) % 5;
int row = (channel - 1) / 5;
long clickX = x0 + 82 * col;
long clickY = y0 + 40 + 30 * row;

HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
if (hwnd && IsWindow(hwnd)) {
	SPUtils::LeftClick(hwnd, static_cast<int>(clickX), static_cast<int>(clickY), 2, 120, 80);
}
std::this_thread::sleep_for(std::chrono::milliseconds(300));
return true;
}

bool HandleLoginPrompts(long mainIndex, const std::chrono::steady_clock::time_point& deadline) {
long x = -1, y = -1;
if (!WaitForIconBmpFirst(mainIndex, kLoginIconPath, kLoginIconFallbackPath, 60000, 0.9, x, y, deadline)) {
	// Fallback: already at character select or in game.
	if (FindIconInWindowBmpFirst(mainIndex, kPlayIconPath, kPlayIconFallbackPath, 0.9, x, y)) return true;
	if (FindIconInWindow(mainIndex, whiteIcon, 0.9, x, y)) return true;
	return false;
}

std::this_thread::sleep_for(std::chrono::milliseconds(500));

if (TryQuickLoginUntilWhite(mainIndex, deadline)) {
	return true;
}

long scx = -1, scy = -1;
if (WaitForIconBmpFirst(mainIndex, kScaniaIconPath, kScaniaIconFallbackPath, 20000, 0.9, scx, scy, deadline)) {
	HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
	if (hwnd && IsWindow(hwnd)) {
		SPUtils::LeftClick(hwnd, static_cast<int>(scx), static_cast<int>(scy), 2, 150, 100);
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
	SelectChannel(mainIndex, deadline);
	PressAutoLoginKey(mainIndex, L"enter", 2, 320);
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

if (FindIconInWindowBmpFirst(mainIndex, kOkLoginIconPath, kOkLoginIconFallbackPath, 0.9, x, y)) {
	return false;
}

long playX = -1, playY = -1;
bool playFound = false;
bool whiteBeforePlay = false;
if (!WaitForPlayOrWhite(mainIndex, 45000, playX, playY, playFound, whiteBeforePlay, deadline)) {
	return false;
}
if (!playFound && !whiteBeforePlay) return false;
if (whiteBeforePlay) return true;

HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
if (hwnd && IsWindow(hwnd)) {
	SPUtils::LeftClick(hwnd, static_cast<int>(playX), static_cast<int>(playY), 1, 120, 80);
}
std::this_thread::sleep_for(std::chrono::milliseconds(300));

PressAutoLoginKey(mainIndex, L"enter", 2, 300);
long wx = -1, wy = -1;
WaitForStableWhite(mainIndex, 80000, 500, 0.9, wx, wy, deadline, 50, 4000);
return true;
}

bool FinalizeLogin(long mainIndex, const std::chrono::steady_clock::time_point& deadline) {
long x = -1, y = -1;
bool whiteFound = WaitForStableWhite(mainIndex, 60000, 500, 0.9, x, y, deadline);
std::this_thread::sleep_for(std::chrono::milliseconds(200));

if (whiteFound) {
	press(mainIndex, L"a", 2, 120);
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

bool settingFound = FindIconInWindowBmpFirst(mainIndex, kSettingIconPath, kSettingIconFallbackPath, 0.88, x, y);
const auto settingSearchEnd = std::chrono::steady_clock::now() + std::chrono::seconds(20);
while (!settingFound && std::chrono::steady_clock::now() < settingSearchEnd) {
	if (std::chrono::steady_clock::now() > deadline) return false;
	press(mainIndex, L"esc", 1, 100);
	settingFound = WaitForIconBmpFirst(mainIndex, kSettingIconPath, kSettingIconFallbackPath, 1200, 0.88, x, y, deadline);
	if (!settingFound) {
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
	}
}
if (settingFound) {
	press(mainIndex, L"esc", 1, 100);
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
}
return true;
}

bool LaunchGameAndWaitForWindow(long mainIndex, const std::chrono::steady_clock::time_point& deadline) {
KillMapleStoryProcesses();
std::this_thread::sleep_for(std::chrono::seconds(3));
CleanupNexonWindows();

HINSTANCE hInstance = ShellExecuteW(nullptr, L"open", kLaunchCommand, nullptr, nullptr, SW_SHOWNORMAL);
if ((intptr_t)hInstance <= 32) {
	std::wstring launcher = GetNexonLauncherPath();
	if (!launcher.empty()) {
		hInstance = ShellExecuteW(nullptr, L"open", launcher.c_str(), kLaunchCommand, nullptr, SW_SHOWNORMAL);
	}
}
if ((intptr_t)hInstance <= 32) return false;

int waitSec = 120;
while (waitSec > 0) {
	if (std::chrono::steady_clock::now() > deadline) return false;
	CleanupNexonWindows();
	HWND found = findMSWindow();
	if (found && IsWindow(found)) {
		UpdateWindowForAllThreads(mainIndex, found);
		g_loginNeedRestart[mainIndex].store(true);
		CleanupNexonWindowsFor(std::chrono::seconds(5));
		return true;
	}
	std::this_thread::sleep_for(std::chrono::seconds(2));
	waitSec -= 2;
}
CleanupNexonWindows();

return false;
}

bool RunAutoLoginOnce(long mainIndex) {
const auto deadline = std::chrono::steady_clock::now() + std::chrono::minutes(5);

if (g_forceRelaunch[mainIndex].exchange(false)) {
	if (!LaunchGameAndWaitForWindow(mainIndex, deadline)) return false;
}

if (!EnsureWindowBinding(mainIndex)) {
	if (!LaunchGameAndWaitForWindow(mainIndex, deadline)) return false;
}

ActivateWindow(mainIndex);
if (!HandleLoginPrompts(mainIndex, deadline)) return false;
if (!FinalizeLogin(mainIndex, deadline)) return false;
RunPostLoginActions(mainIndex);
return true;
}

void AutoLoginThread(long mainIndex) {
SPUtils::ReleaseAllKeysFastKeyboardOnlySkipEnter();
gMonitorInstance.setRuneCoords(-1, -1);
g_loginNeedRestart[mainIndex].store(false);

bool success = false;
for (int attempt = 0; attempt < 2 && !success; ++attempt) {
	success = RunAutoLoginOnce(mainIndex);
	if (!success) {
		KillMapleStoryProcesses();
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

if (success) {
	gMonitorInstance.whiteIconUpdate = 1;
	SetTaskState(mainIndex, _T("AUTO LOGIN OK"));
	g_loginPendingSinceMs[mainIndex].store(0);
	if (g_loginNeedRestart[mainIndex].load()) {
		SPUtils::ReleaseAllKeysFastKeyboardOnlySkipEnter();
		ThreadRestart(mainIndex);
		g_loginNeedRestart[mainIndex].store(false);
	}
	subSoftStart();
} else {
	SetTaskState(mainIndex, _T("AUTO LOGIN FAIL"));
}

g_loginRunning[mainIndex].store(false);
g_lastCheckMs[mainIndex].store(GetTime());
}

void TriggerAutoLogin(long mainIndex, unsigned long long generation) {
if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return;
bool expected = false;
if (!g_loginRunning[mainIndex].compare_exchange_strong(expected, true)) return;
ResetAutoRestState(mainIndex);
if (g_loginPendingSinceMs[mainIndex].load() <= 0) {
	g_loginPendingSinceMs[mainIndex].store(GetTime());
}
subSoftPause();
SetTaskState(mainIndex, _T("AUTO LOGIN"));
std::thread([mainIndex]() { AutoLoginThread(mainIndex); }).detach();
}

bool IsOnLoginScreen(long mainIndex) {
long x = -1, y = -1;
if (!FindIconInWindowBmpFirst(mainIndex, kLoginIconPath, kLoginIconFallbackPath, 0.9, x, y)) return false;
return x > 0 && y > 0;
}

bool IsStuckScreen(long mainIndex) {
	constexpr double kDarkTextSim = 0.995;

	if (!EnsureWindowBinding(mainIndex)) return false;
	HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
	if (!hwnd || !IsWindow(hwnd)) return false;

	auto selectExistingPath = [](const wchar_t* bmpPath, const wchar_t* pngPath) {
		if (bmpPath && FileExists(bmpPath)) return std::wstring(bmpPath);
		if (pngPath && FileExists(pngPath)) return std::wstring(pngPath);
		return std::wstring();
	};
	const std::wstring stuckPath = selectExistingPath(kStuckIconPath, kStuckIconFallbackPath);
	const std::wstring stuck2Path = selectExistingPath(kStuck2IconPath, kStuck2IconFallbackPath);
	if (stuckPath.empty() || stuck2Path.empty()) return false;

	long width = 0;
	long height = 0;
	if (!GetWindowSizeForSearch(mainIndex, width, height) || width <= 0 || height <= 0) return false;

	const std::wstring iconPaths = stuckPath + L"|" + stuck2Path;
	long x = -1;
	long y = -1;
	return SPUtils::FindPicNative(hwnd, 0, 0, static_cast<int>(width), static_cast<int>(height),
		iconPaths, kDarkTextSim, x, y);
}
void DisconnectWatcherLoop(long mainIndex, unsigned long long generation) {
while (true) {
	if (mainIndex < 0 || mainIndex >= MAX_HWND) {
		break;
	}
	if (g_disconnectWatcherGeneration[mainIndex].load() != generation) {
		break;
	}
	if (g_info[mainIndex].is_stop) {
		break;
	}

	if (HandleNetchGuard(mainIndex, generation) &&
		IsDisconnectWatcherCurrent(mainIndex, generation)) {
		AutoLogin_CheckAndTrigger(mainIndex, generation);
	}
	if (AutoLogin_IsActive(mainIndex)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(kDisconnectWatcherActiveIntervalMs));
	}
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(kDisconnectWatcherIdleIntervalMs));
	}
}
}

bool HasRecentWhiteIcon(long mainIndex, ULONGLONG nowTick) {
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return false;
	ULONGLONG lastSeenTick = g_lastWhiteIconSeenTick[mainIndex].load(std::memory_order_relaxed);
	return lastSeenTick != 0 && nowTick >= lastSeenTick && nowTick - lastSeenTick <= kWhiteIconRecentTtlMs;
}

bool TryRecordWhiteIconGate(long mainIndex, unsigned long long generation) {
	long x = -1;
	long y = -1;
	if (!findPicWithOpenCV(mainIndex, 0, 0, kWhiteIconGateRight, kWhiteIconGateBottom,
		whiteIcon, 0.9, x, y)) {
		return false;
	}
	{
		std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
		if (!IsDisconnectWatcherCurrent(mainIndex, generation)) return false;
		AutoLogin_RecordWhiteIconSeen(mainIndex);
	}
	return true;
}

}

bool AutoLogin_IsActive(long index) {
long mainIndex = NormalizeMainIndex(index);
if (mainIndex < 0 || mainIndex >= MAX_HWND) return false;
return g_loginRunning[mainIndex].load();
}

void AutoLogin_RecordWhiteIconSeen(long index) {
	long mainIndex = NormalizeMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
	g_lastWhiteIconSeenTick[mainIndex].store(GetTickCount64(), std::memory_order_relaxed);
}

bool AutoLogin_RunStartup(long index) {
long mainIndex = NormalizeMainIndex(index);
if (mainIndex < 0 || mainIndex >= MAX_HWND) return false;

SPUtils::ReleaseAllKeysFastKeyboardOnlySkipEnter();
gMonitorInstance.setRuneCoords(-1, -1);
g_loginNeedRestart[mainIndex].store(false);

if (g_loginPendingSinceMs[mainIndex].load() <= 0) {
	g_loginPendingSinceMs[mainIndex].store(GetTime());
}

subSoftPause();
SetTaskState(mainIndex, _T("AUTO LOGIN"));

bool success = false;
for (int attempt = 0; attempt < 2 && !success; ++attempt) {
	success = RunAutoLoginOnce(mainIndex);
	if (!success) {
		KillMapleStoryProcesses();
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

if (success) {
	gMonitorInstance.whiteIconUpdate = 1;
	g_loginPendingSinceMs[mainIndex].store(0);
	g_loginNeedRestart[mainIndex].store(false);
	SetTaskState(mainIndex, _T("AUTO LOGIN OK"));
	return true;
}

SetTaskState(mainIndex, _T("AUTO LOGIN FAIL"));
return false;
}

long AutoLogin_GetLoginPendingMs(long index) {
long mainIndex = NormalizeMainIndex(index);
if (mainIndex < 0 || mainIndex >= MAX_HWND) return 0;
long start = g_loginPendingSinceMs[mainIndex].load();
if (start <= 0) return 0;
long now = GetTime();
if (now < start) return 0;
return now - start;
}

void AutoLogin_ClearLoginPending(long index) {
long mainIndex = NormalizeMainIndex(index);
if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
g_loginPendingSinceMs[mainIndex].store(0);
}

void AutoLogin_CheckAndTrigger(long index, unsigned long long generation) {
	long mainIndex = NormalizeMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
	auto watcherIsCurrent = [mainIndex, generation]() {
		return IsDisconnectWatcherCurrent(mainIndex, generation);
	};
	if (!watcherIsCurrent()) return;
	if (g_info[mainIndex].is_stop) return;
	if (HandleAutoRest(mainIndex, generation)) return;
	if (!GetAutoLogin()) return;
	if (g_loginRunning[mainIndex].load()) return;

	long nowMs = GetTime();
	long last = g_lastCheckMs[mainIndex].load();
	if (nowMs - last < kDisconnectWatcherBaseCheckIntervalMs) return;
	g_lastCheckMs[mainIndex].store(nowMs);

	HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
	if (!hwnd || !IsWindow(hwnd)) {
		if (!watcherIsCurrent()) return;
		g_stuckScreenConsecutiveCount[mainIndex].store(0);
		g_disconnectDialogConsecutiveCount[mainIndex].store(0);
		NotifyDisconnectMiao("window missing");
		TriggerAutoLogin(mainIndex, generation);
		return;
	}

	HWND dialogHwnd = DismissDisconnectDialog(mainIndex);
	if (!watcherIsCurrent()) return;
	if (dialogHwnd != nullptr) {
		int count = g_disconnectDialogConsecutiveCount[mainIndex].fetch_add(1) + 1;
		if (count >= 1) {
			g_disconnectDialogConsecutiveCount[mainIndex].store(0);
			g_stuckScreenConsecutiveCount[mainIndex].store(0);

			// Save screenshot of the dialog window to desktop
			std::wstring desktopPath = GetDesktopPath();
			std::wstring filename = desktopPath + L"\\DisconnectDialog_" + std::to_wstring(mainIndex) + L"_" + std::to_wstring(GetTime()) + L".png";
			#ifdef UNICODE
			SPUtils::CapturePng(dialogHwnd, 0, 0, 0, 0, filename.c_str());
			#else
			std::string filenameA = WideToAnsiLocal(filename.c_str());
			SPUtils::CapturePng(dialogHwnd, 0, 0, 0, 0, filenameA.c_str());
			#endif

			NotifyDisconnectMiao("disconnect dialog");
			{
				std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
				if (!watcherIsCurrent()) return;
				KillMapleStoryProcesses();
				g_forceRelaunch[mainIndex].store(true);
			}
			TriggerAutoLogin(mainIndex, generation);
			return;
		}
	} else {
		g_disconnectDialogConsecutiveCount[mainIndex].store(0);
	}

	if (!HasRecentWhiteIcon(mainIndex, GetTickCount64())) {
		TryRecordWhiteIconGate(mainIndex, generation);
	}
	if (!watcherIsCurrent()) return;
	if (HasRecentWhiteIcon(mainIndex, GetTickCount64())) {
		g_stuckScreenConsecutiveCount[mainIndex].store(0);
		return;
	}

	long lastImageCheck = g_lastImageCheckMs[mainIndex].load();
	if (nowMs - lastImageCheck < kDisconnectWatcherImageCheckIntervalMs) return;
	g_lastImageCheckMs[mainIndex].store(nowMs);

	bool stuckScreen = IsStuckScreen(mainIndex);
	if (!watcherIsCurrent()) return;
	if (stuckScreen) {
		int count = g_stuckScreenConsecutiveCount[mainIndex].fetch_add(1) + 1;
		if (count >= 1) {
			g_stuckScreenConsecutiveCount[mainIndex].store(0);
			g_disconnectDialogConsecutiveCount[mainIndex].store(0);
			NotifyDisconnectMiao("stuck screen");
			{
				std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
				if (!watcherIsCurrent()) return;
				g_forceRelaunch[mainIndex].store(true);
			}
			TriggerAutoLogin(mainIndex, generation);
			return;
		}
	} else {
		g_stuckScreenConsecutiveCount[mainIndex].store(0);
	}

	bool onLoginScreen = IsOnLoginScreen(mainIndex);
	if (!watcherIsCurrent()) return;
	if (onLoginScreen) {
		g_stuckScreenConsecutiveCount[mainIndex].store(0);
		g_disconnectDialogConsecutiveCount[mainIndex].store(0);
		NotifyDisconnectMiao("login screen");
		TriggerAutoLogin(mainIndex, generation);
	}
}


void AutoLogin_StartDisconnectWatcher(long index) {
	long mainIndex = NormalizeMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
unsigned long long generation;
{
	std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
	generation = g_disconnectWatcherGeneration[mainIndex].fetch_add(1) + 1;
}
	g_stuckScreenConsecutiveCount[mainIndex].store(0);
	g_disconnectDialogConsecutiveCount[mainIndex].store(0);
g_lastWhiteIconSeenTick[mainIndex].store(0, std::memory_order_relaxed);
ResetAutoRestState(mainIndex);
ResetNetchGuardState(mainIndex);

std::thread([mainIndex, generation]() {
	DisconnectWatcherLoop(mainIndex, generation);
}).detach();
}

void AutoLogin_StopDisconnectWatcher(long index) {
	long mainIndex = NormalizeMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
{
	std::lock_guard<std::mutex> actionLock(g_disconnectWatcherActionMutex);
	g_disconnectWatcherGeneration[mainIndex].fetch_add(1);
}
	g_stuckScreenConsecutiveCount[mainIndex].store(0);
	g_disconnectDialogConsecutiveCount[mainIndex].store(0);
g_lastWhiteIconSeenTick[mainIndex].store(0, std::memory_order_relaxed);
ResetAutoRestState(mainIndex);
ResetNetchGuardState(mainIndex);
}


