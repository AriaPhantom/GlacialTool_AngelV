#include "stdafx.h"

#include "AutoLogin.h"

#include "SPUtils_legacy.h"
#include "SPUtils.h"
#include "thread_control.h"
#include "script.h"
#include "gMonitor.h"
#include "log.h"

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cwctype>
#include <string>
#include <thread>
#include <vector>

#include <shellapi.h>
#include <tlhelp32.h>

extern gMonitor gMonitorInstance;
extern int GetAutoLogin();
extern std::wstring GetAutoLoginKeys();
extern int GetAutoLoginDelay();
extern int GetAutoLoginChannel();
extern int GetAutoLoginMode();
extern void subSoftStart();
extern void subSoftPause();

#if defined(GT_LEGACY_NO_INPUT) && defined(_MBCS)
static std::string WideToAnsiLocal(const wchar_t* value) {
	if (value == nullptr || *value == L'\0') {
		return std::string();
	}
	int size = WideCharToMultiByte(CP_ACP, 0, value, -1, nullptr, 0, nullptr, nullptr);
	if (size <= 1) {
		return std::string();
	}
	std::string result(static_cast<size_t>(size - 1), '\0');
	WideCharToMultiByte(CP_ACP, 0, value, -1, &result[0], size, nullptr, nullptr);
	return result;
}

void keyDown(long index, const wchar_t* key) {
	std::string narrow = WideToAnsiLocal(key);
	if (!narrow.empty()) {
		keyDown(index, narrow.c_str());
	}
}

void keyUp(long index, const wchar_t* key) {
	std::string narrow = WideToAnsiLocal(key);
	if (!narrow.empty()) {
		keyUp(index, narrow.c_str());
	}
}

void press(long index, const wchar_t* key, int times, long delay) {
	std::string narrow = WideToAnsiLocal(key);
	if (!narrow.empty()) {
		press(index, narrow.c_str(), times, delay);
	}
}
#endif

namespace {

const wchar_t* kLoginIconPath = L"C:\\sptool\\login.bmp";
const wchar_t* kScaniaIconPath = L"C:\\sptool\\scania.png";
const wchar_t* kPlayIconPath = L"C:\\sptool\\play.png";
const wchar_t* kOkLoginIconPath = L"C:\\sptool\\okLogin.png";
const wchar_t* kSettingIconPath = L"C:\\sptool\\setting.png";
const wchar_t* kBigScaniaIconPath = L"C:\\sptool\\BigScania.bmp";
const wchar_t* kQuickLoginIconPath = L"C:\\sptool\\QuickLogin.png";
const wchar_t* kStuckIconPath = L"C:\\sptool\\Stuck.png";
const double kQuickLoginSim = 0.985;

const wchar_t* kLaunchCommand = L"nxl://launch/10100";

std::atomic<bool> g_loginRunning[MAX_HWND];
std::atomic<long> g_lastCheckMs[MAX_HWND];
std::atomic<long> g_loginPendingSinceMs[MAX_HWND];
std::atomic<bool> g_loginNeedRestart[MAX_HWND];
std::atomic<bool> g_forceRelaunch[MAX_HWND];

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

struct DisconnectDialogHit {
	DWORD pid;
	bool found;
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
	return TRUE;
}

bool DismissDisconnectDialog(long mainIndex) {
	DisconnectDialogHit hit = {};
	hit.pid = static_cast<DWORD>(g_info[mainIndex].pid);
	hit.found = false;
	EnumWindows(EnumDisconnectDialogProc, reinterpret_cast<LPARAM>(&hit));
	return hit.found;
}

long NormalizeMainIndex(long index) {
	if (index < 0) return -1;
	if (index >= MAX_HWND * 2) index -= MAX_HWND * 2;
	if (index >= MAX_HWND) index -= MAX_HWND;
	return index;
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

BOOL CALLBACK MinimizeNexonWindowProc(HWND hwnd, LPARAM) {
	if (!IsWindow(hwnd)) return TRUE;
	if (GetWindow(hwnd, GW_OWNER) != nullptr) return TRUE;
	if (!IsWindowVisible(hwnd)) return TRUE;

	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	if (pid == 0) return TRUE;

	std::wstring procName;
	if (!GetProcessNameByPid(pid, procName)) return TRUE;
	if (!IsNexonProcessName(procName)) return TRUE;

	ShowWindow(hwnd, SW_MINIMIZE);
	return TRUE;
}

void MinimizeNexonWindows() {
	EnumWindows(MinimizeNexonWindowProc, 0);
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

bool FindIconInWindow(long mainIndex, const std::wstring& iconPath, double sim, long& outx, long& outy) {
	long width = 0, height = 0;
	if (!GetWindowSizeForSearch(mainIndex, width, height)) return false;
	return findPicWithOpenCV(mainIndex, 0, 0, static_cast<int>(width), static_cast<int>(height), iconPath, sim, outx, outy);
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

bool TryQuickLoginClick(long mainIndex) {
	long x = -1, y = -1;
	if (!FindIconInWindow(mainIndex, kQuickLoginIconPath, kQuickLoginSim, x, y)) {
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
	while (std::chrono::steady_clock::now() - start < std::chrono::seconds(30)) {
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
	if (!WaitForIcon(mainIndex, kBigScaniaIconPath, 20000, 0.9, x0, y0, deadline)) {
		return false;
	}

	int channel = ClampChannel(GetAutoLoginChannel());
	int col = (channel - 1) % 5;
	int row = (channel - 1) / 5;
	long clickX = x0 + 82 * col;
	long clickY = y0 + 40 + 30 * row;

	HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
	if (hwnd && IsWindow(hwnd)) {
		SPUtils::LeftClick(hwnd, static_cast<int>(clickX), static_cast<int>(clickY), 1, 120, 80);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	return true;
}

bool HandleLoginPrompts(long mainIndex, const std::chrono::steady_clock::time_point& deadline) {
	long x = -1, y = -1;
	if (!WaitForIcon(mainIndex, kLoginIconPath, 60000, 0.9, x, y, deadline)) {
		// Fallback: already at character select or in game.
		if (FindIconInWindow(mainIndex, kPlayIconPath, 0.9, x, y)) return true;
		if (FindIconInWindow(mainIndex, whiteIcon, 0.9, x, y)) return true;
		return false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (TryQuickLoginUntilWhite(mainIndex, deadline)) {
		return true;
	}

	long scx = -1, scy = -1;
	if (WaitForIcon(mainIndex, kScaniaIconPath, 20000, 0.9, scx, scy, deadline)) {
		HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
		if (hwnd && IsWindow(hwnd)) {
			SPUtils::LeftClick(hwnd, static_cast<int>(scx), static_cast<int>(scy), 2, 150, 100);
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
		SelectChannel(mainIndex, deadline);
		press(mainIndex, L"enter", 1, 100);
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}

	if (FindIconInWindow(mainIndex, kOkLoginIconPath, 0.9, x, y)) {
		return false;
	}

	long playX = -1, playY = -1;
	if (!WaitForIcon(mainIndex, kPlayIconPath, 45000, 0.9, playX, playY, deadline)) {
		return false;
	}

	press(mainIndex, L"enter", 1, 100);
	std::this_thread::sleep_for(std::chrono::seconds(8));
	return true;
}

bool FinalizeLogin(long mainIndex, const std::chrono::steady_clock::time_point& deadline) {
	long x = -1, y = -1;
	bool whiteFound = WaitForIcon(mainIndex, whiteIcon, 60000, 0.9, x, y, deadline);
	std::this_thread::sleep_for(std::chrono::seconds(2));

	if (whiteFound) {
		press(mainIndex, L"a", 2, 120);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}

	for (int i = 0; i < 4; ++i) {
		press(mainIndex, L"esc", 1, 100);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	bool settingFound = WaitForIcon(mainIndex, kSettingIconPath, 20000, 0.9, x, y, deadline);
	if (settingFound) {
		press(mainIndex, L"esc", 1, 100);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
	return true;
}

bool LaunchGameAndWaitForWindow(long mainIndex, const std::chrono::steady_clock::time_point& deadline) {
	KillMapleStoryProcesses();
	std::this_thread::sleep_for(std::chrono::seconds(3));

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
		HWND found = findMSWindow();
		if (found && IsWindow(found)) {
			UpdateWindowForAllThreads(mainIndex, found);
			g_loginNeedRestart[mainIndex].store(true);
			MinimizeNexonWindows();
			std::this_thread::sleep_for(std::chrono::seconds(5));
			return true;
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
		waitSec -= 2;
	}

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
	SPUtils::ReleaseAllKeys();
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
		subSoftStart();
		if (g_loginNeedRestart[mainIndex].load()) {
			SPUtils::ReleaseAllKeys();
			ThreadRestart(mainIndex);
			g_loginNeedRestart[mainIndex].store(false);
		}
	} else {
		SetTaskState(mainIndex, _T("AUTO LOGIN FAIL"));
	}

	g_loginRunning[mainIndex].store(false);
	g_lastCheckMs[mainIndex].store(GetTime());
}

void TriggerAutoLogin(long mainIndex) {
	bool expected = false;
	if (!g_loginRunning[mainIndex].compare_exchange_strong(expected, true)) return;
	if (g_loginPendingSinceMs[mainIndex].load() <= 0) {
		g_loginPendingSinceMs[mainIndex].store(GetTime());
	}
	subSoftPause();
	SetTaskState(mainIndex, _T("AUTO LOGIN"));
	std::thread([mainIndex]() { AutoLoginThread(mainIndex); }).detach();
}

bool IsOnLoginScreen(long mainIndex) {
	long x = -1, y = -1;
	if (!FindIconInWindow(mainIndex, kLoginIconPath, 0.9, x, y)) return false;
	return x > 0 && y > 0;
}

bool IsStuckScreen(long mainIndex) {
	long x = -1, y = -1;
	if (!FindIconInWindow(mainIndex, kStuckIconPath, 0.9, x, y)) return false;
	return x > 0 && y > 0;
}

}

bool AutoLogin_IsActive(long index) {
	long mainIndex = NormalizeMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return false;
	return g_loginRunning[mainIndex].load();
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

void AutoLogin_CheckAndTrigger(long index) {
	long mainIndex = NormalizeMainIndex(index);
	if (mainIndex < 0 || mainIndex >= MAX_HWND) return;
	if (g_info[mainIndex].is_stop) return;
	if (!GetAutoLogin()) return;
	if (g_loginRunning[mainIndex].load()) return;

	long nowMs = GetTime();
	long last = g_lastCheckMs[mainIndex].load();
	if (nowMs - last < 1000) return;
	g_lastCheckMs[mainIndex].store(nowMs);

	HWND hwnd = reinterpret_cast<HWND>(static_cast<LONG_PTR>(g_info[mainIndex].hwnd));
	if (!hwnd || !IsWindow(hwnd)) {
		TriggerAutoLogin(mainIndex);
		return;
	}

	if (DismissDisconnectDialog(mainIndex)) {
		KillMapleStoryProcesses();
		g_forceRelaunch[mainIndex].store(true);
		TriggerAutoLogin(mainIndex);
		return;
	}

	if (IsStuckScreen(mainIndex)) {
		g_forceRelaunch[mainIndex].store(true);
		TriggerAutoLogin(mainIndex);
		return;
	}

	if (IsOnLoginScreen(mainIndex)) {
		TriggerAutoLogin(mainIndex);
	}
}


