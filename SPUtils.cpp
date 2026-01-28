#include "stdafx.h"

#include "SPUtils.h"
#include "HKM.h"

#include <ShellScalingApi.h>
#pragma comment(lib, "Shcore.lib")

#include <algorithm>
#include <atomic>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <cwctype>
#include <map>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>
#include <vector>

extern int GetWuyaInputMode();

namespace {

#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#endif

using SetThreadDpiAwarenessContextFn = DPI_AWARENESS_CONTEXT(WINAPI*)(DPI_AWARENESS_CONTEXT);

SetThreadDpiAwarenessContextFn GetSetThreadDpiAwarenessContextFn() {
	static SetThreadDpiAwarenessContextFn fn = []() -> SetThreadDpiAwarenessContextFn {
		HMODULE user32 = GetModuleHandleW(L"user32.dll");
		if (!user32) return nullptr;
		return reinterpret_cast<SetThreadDpiAwarenessContextFn>(GetProcAddress(user32, "SetThreadDpiAwarenessContext"));
	}();
	return fn;
}

std::string WideToAnsi(const wchar_t* wide) {
	if (wide == nullptr || *wide == L'\0') return {};
	int bytes = WideCharToMultiByte(CP_ACP, 0, wide, -1, nullptr, 0, nullptr, nullptr);
	if (bytes <= 0) return {};
	std::string ansi(bytes, '\0');
	WideCharToMultiByte(CP_ACP, 0, wide, -1, &ansi[0], bytes, nullptr, nullptr);
	ansi.erase(std::find(ansi.begin(), ansi.end(), '\0'), ansi.end());
	return ansi;
}

std::string TCharPathToAnsi(const TCHAR* path) {
	if (path == nullptr || *path == _T('\0')) return {};
#ifdef UNICODE
	return WideToAnsi(path);
#else
	return std::string(path);
#endif
}

struct HkmContext {
	std::mutex mutex;
	LPVOID dev = nullptr;
	ULONGLONG lastInitAttemptMs = 0;

	~HkmContext() {
		if (dev) {
			HKMClose(dev);
			dev = nullptr;
		}
	}
};

HkmContext& GetHkmContext() {
	static HkmContext ctx;
	return ctx;
}

DWORD FindHkmDeviceId() {
	const DWORD kAnyVidPid = 0x10000;
	DWORD devId = HKMSearchDevice(kAnyVidPid, kAnyVidPid, HDT_KM);
	if (devId == HKM_FAIL) devId = HKMSearchDevice(kAnyVidPid, kAnyVidPid, HDT_KEY);
	if (devId == HKM_FAIL) devId = HKMSearchDevice(kAnyVidPid, kAnyVidPid, HDT_ALL);
	return devId;
}

bool EnsureHkmDeviceLocked(HkmContext& ctx) {
	if (ctx.dev) return true;

	ULONGLONG now = GetTickCount64();
	if (ctx.lastInitAttemptMs != 0 && (now - ctx.lastInitAttemptMs) < 2000) return false;
	ctx.lastInitAttemptMs = now;

	DWORD devId = FindHkmDeviceId();
	if (devId == HKM_FAIL) return false;

	ctx.dev = HKMOpen(devId, DPIM_PHYSICAL);
	return ctx.dev != nullptr;
}

LPCWSTR VkToHkmParam(WORD vkCode) {
	return reinterpret_cast<LPCWSTR>(static_cast<ULONG_PTR>(vkCode));
}

bool SendHkmKey(WORD vkCode, bool isKeyUp) {
	auto& ctx = GetHkmContext();
	std::lock_guard<std::mutex> lock(ctx.mutex);
	if (!EnsureHkmDeviceLocked(ctx)) return false;
	LPCWSTR param = VkToHkmParam(vkCode);
	return isKeyUp ? HKMKeyUp(ctx.dev, param) : HKMKeyDown(ctx.dev, param);
}

bool EnsureHkmMouseLocked(HkmContext& ctx) {
	if (!EnsureHkmDeviceLocked(ctx)) return false;
	DWORD mode = HKMGetMouseMode(ctx.dev);
	if (mode == HKM_FAIL || mode == HMT_NONE) return false;
	return true;
}

bool SendHkmMouseMove(long x, long y) {
	auto& ctx = GetHkmContext();
	std::lock_guard<std::mutex> lock(ctx.mutex);
	if (!EnsureHkmMouseLocked(ctx)) return false;
	return HKMMoveTo(ctx.dev, x, y) != FALSE;
}

bool SendHkmLeftClick() {
	auto& ctx = GetHkmContext();
	std::lock_guard<std::mutex> lock(ctx.mutex);
	if (!EnsureHkmMouseLocked(ctx)) return false;
	return HKMLeftClick(ctx.dev) != FALSE;
}

bool SendHkmRightClick() {
	auto& ctx = GetHkmContext();
	std::lock_guard<std::mutex> lock(ctx.mutex);
	if (!EnsureHkmMouseLocked(ctx)) return false;
	return HKMRightClick(ctx.dev) != FALSE;
}

bool ReleaseHkmKeyboard() {
	auto& ctx = GetHkmContext();
	std::lock_guard<std::mutex> lock(ctx.mutex);
	if (!EnsureHkmDeviceLocked(ctx)) return false;
	return HKMReleaseKeyboard(ctx.dev) != FALSE;
}

bool ReleaseHkmMouse() {
	auto& ctx = GetHkmContext();
	std::lock_guard<std::mutex> lock(ctx.mutex);
	if (!EnsureHkmDeviceLocked(ctx)) return false;
	return HKMReleaseMouse(ctx.dev) != FALSE;
}

constexpr int kWuyaModeExternalOnly = 0;
constexpr int kWuyaModePreferExternal = 1;
constexpr int kWuyaModeNativeOnly = 2;
constexpr int kWuyaBackendNone = 0;
constexpr int kWuyaBackendExternal = 1;
constexpr int kWuyaBackendNative = 2;

std::atomic<int> g_lastInputBackend{kWuyaBackendNone};
std::mutex g_keyStateMutex;
std::set<std::wstring> g_pressedKeys;

int GetWuyaInputModeSafe() {
	int mode = GetWuyaInputMode();
	if (mode < kWuyaModeExternalOnly || mode > kWuyaModeNativeOnly) {
		return kWuyaModeNativeOnly;
	}
	return mode;
}

bool IsExtendedKey(WORD vkCode) {
	switch (vkCode) {
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_PRIOR:  // Page Up
	case VK_NEXT:   // Page Down
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_RCONTROL:
	case VK_RMENU:
	case VK_NUMLOCK:
	case VK_DIVIDE:
		return true;
	default:
		return false;
	}
}

WORD VirtualKeyToScanCode(WORD virtualKeyCode) {
	return MapVirtualKey(virtualKeyCode, MAPVK_VK_TO_VSC);
}

void SendVirtualKeyUp(WORD virtualKeyCode) {
	WORD scanCode = VirtualKeyToScanCode(virtualKeyCode);
	if (scanCode == 0) return;
	INPUT input = {};
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = scanCode;
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	if (IsExtendedKey(virtualKeyCode)) {
		input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}
	SendInput(1, &input, sizeof(INPUT));
}

void ReleaseNativeMouseButtons() {
	INPUT inputs[3] = {};
	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	inputs[1].type = INPUT_MOUSE;
	inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	inputs[2].type = INPUT_MOUSE;
	inputs[2].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
	SendInput(3, inputs, sizeof(INPUT));
}

WORD GetVirtualKeyCode(const std::wstring& key) {
	static std::map<std::wstring, WORD> specialKeys;
	if (specialKeys.empty()) {
		specialKeys[L"f1"] = VK_F1;     specialKeys[L"f2"] = VK_F2;
		specialKeys[L"f3"] = VK_F3;     specialKeys[L"f4"] = VK_F4;
		specialKeys[L"f5"] = VK_F5;     specialKeys[L"f6"] = VK_F6;
		specialKeys[L"f7"] = VK_F7;     specialKeys[L"f8"] = VK_F8;
		specialKeys[L"f9"] = VK_F9;     specialKeys[L"f10"] = VK_F10;
		specialKeys[L"f11"] = VK_F11;   specialKeys[L"f12"] = VK_F12;

		specialKeys[L"ctrl"] = VK_CONTROL;
		specialKeys[L"alt"] = VK_MENU;
		specialKeys[L"shift"] = VK_SHIFT;
		specialKeys[L"win"] = VK_LWIN;
		specialKeys[L"enter"] = VK_RETURN;
		specialKeys[L"space"] = VK_SPACE;
		specialKeys[L"tab"] = VK_TAB;
		specialKeys[L"esc"] = VK_ESCAPE;
		specialKeys[L"backspace"] = VK_BACK;

		specialKeys[L"left"] = VK_LEFT;
		specialKeys[L"up"] = VK_UP;
		specialKeys[L"right"] = VK_RIGHT;
		specialKeys[L"down"] = VK_DOWN;
		specialKeys[L"insert"] = VK_INSERT;
		specialKeys[L"delete"] = VK_DELETE;
		specialKeys[L"home"] = VK_HOME;
		specialKeys[L"end"] = VK_END;
		specialKeys[L"pageup"] = VK_PRIOR;
		specialKeys[L"pagedown"] = VK_NEXT;

		specialKeys[L"capslock"] = VK_CAPITAL;
		specialKeys[L"numlock"] = VK_NUMLOCK;
		specialKeys[L"scrolllock"] = VK_SCROLL;

		specialKeys[L"num0"] = VK_NUMPAD0;  specialKeys[L"num1"] = VK_NUMPAD1;
		specialKeys[L"num2"] = VK_NUMPAD2;  specialKeys[L"num3"] = VK_NUMPAD3;
		specialKeys[L"num4"] = VK_NUMPAD4;  specialKeys[L"num5"] = VK_NUMPAD5;
		specialKeys[L"num6"] = VK_NUMPAD6;  specialKeys[L"num7"] = VK_NUMPAD7;
		specialKeys[L"num8"] = VK_NUMPAD8;  specialKeys[L"num9"] = VK_NUMPAD9;
		specialKeys[L"num*"] = VK_MULTIPLY; specialKeys[L"num+"] = VK_ADD;
		specialKeys[L"num-"] = VK_SUBTRACT; specialKeys[L"num."] = VK_DECIMAL;
		specialKeys[L"num/"] = VK_DIVIDE;
	}

	std::wstring key_lower = key;
	std::transform(key_lower.begin(), key_lower.end(), key_lower.begin(), ::towlower);

	auto it = specialKeys.find(key_lower);
	if (it != specialKeys.end()) return it->second;

	if (key_lower.length() == 1) {
		SHORT result = VkKeyScanW(key_lower[0]);
		if (result != -1) return LOBYTE(result);
	}

	return 0;
}

void SendKeyInput(const std::wstring& key, bool isKeyUp) {
	WORD vkCode = GetVirtualKeyCode(key);
	if (vkCode == 0) return;

	WORD scanCode = VirtualKeyToScanCode(vkCode);

	INPUT input = {};
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = scanCode;
	input.ki.dwFlags = KEYEVENTF_SCANCODE | (isKeyUp ? KEYEVENTF_KEYUP : 0);
	if (IsExtendedKey(vkCode)) {
		input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}
	SendInput(1, &input, sizeof(INPUT));
}

bool GetScaleFactorForWindow(HWND hwnd, double& outScale) {
	outScale = 1.0;
	HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	if (!hMonitor) return true;
	UINT dpiX = 96, dpiY = 96;
	if (GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY) == S_OK) {
		outScale = static_cast<double>(dpiX) / 96.0;
	}
	return true;
}

bool LogicalClientToPhysicalScreen(HWND hwnd, int logicalX, int logicalY, POINT& outPoint) {
	if (hwnd == nullptr || !IsWindow(hwnd)) return false;

	double scaleFactor = 1.0;
	GetScaleFactorForWindow(hwnd, scaleFactor);

	POINT clientTopLeft = { 0, 0 };
	if (!ClientToScreen(hwnd, &clientTopLeft)) return false;

	outPoint.x = clientTopLeft.x + static_cast<int>(std::lround(logicalX * scaleFactor));
	outPoint.y = clientTopLeft.y + static_cast<int>(std::lround(logicalY * scaleFactor));
	return true;
}

bool TryHkmKey(const std::wstring& key, bool isKeyUp) {
	WORD vkCode = GetVirtualKeyCode(key);
	if (vkCode == 0) return false;
	return SendHkmKey(vkCode, isKeyUp);
}

bool TryHkmMoveTo(HWND hwnd, int logicalX, int logicalY, int delay_after_ms) {
	POINT physicalPoint;
	if (!LogicalClientToPhysicalScreen(hwnd, logicalX, logicalY, physicalPoint)) return false;
	if (!SendHkmMouseMove(physicalPoint.x, physicalPoint.y)) return false;

	if (delay_after_ms > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_after_ms));
	}
	return true;
}

bool TryHkmLeftClickCurrent(int times, int clickDelayMs) {
	for (int i = 0; i < times; ++i) {
		if (!SendHkmLeftClick()) return false;
		if (i + 1 < times) {
			std::this_thread::sleep_for(std::chrono::milliseconds(std::max(1, clickDelayMs)));
		}
	}
	return true;
}

bool TryHkmLeftClick(HWND hwnd, int logicalX, int logicalY, int times, int clickDelayMs, int moveDelayMs) {
	if (!TryHkmMoveTo(hwnd, logicalX, logicalY, moveDelayMs)) return false;
	return TryHkmLeftClickCurrent(times, clickDelayMs);
}

bool TryHkmRightClick(int times, int clickDelayMs) {
	for (int i = 0; i < times; ++i) {
		if (!SendHkmRightClick()) return false;
		if (i + 1 < times) {
			std::this_thread::sleep_for(std::chrono::milliseconds(std::max(1, clickDelayMs)));
		}
	}
	return true;
}

} // namespace

namespace SPUtils {

long GetTimeMs() {
	auto now = std::chrono::steady_clock::now();
	return static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
}

bool CaptureAndResizeToLogic(HWND hwnd, int logic_x1, int logic_y1, int logic_x2, int logic_y2, cv::Mat& outMat) {
	outMat.release();
	if (hwnd == nullptr || !IsWindow(hwnd)) return false;

	DPI_AWARENESS_CONTEXT oldContext = nullptr;
	if (auto fn = GetSetThreadDpiAwarenessContextFn()) {
		oldContext = fn(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	}

	RECT clientPhysicalRect;
	if (!GetClientRect(hwnd, &clientPhysicalRect)) {
		if (auto fn = GetSetThreadDpiAwarenessContextFn()) {
			fn(oldContext);
		}
		return false;
	}

	double scaleFactor = 1.0;
	GetScaleFactorForWindow(hwnd, scaleFactor);

	POINT clientTopLeftPhysical = { clientPhysicalRect.left, clientPhysicalRect.top };
	if (!ClientToScreen(hwnd, &clientTopLeftPhysical)) {
		if (auto fn = GetSetThreadDpiAwarenessContextFn()) {
			fn(oldContext);
		}
		return false;
	}

	int clientLogicWidth = static_cast<int>(std::lround((clientPhysicalRect.right - clientPhysicalRect.left) / scaleFactor));
	int clientLogicHeight = static_cast<int>(std::lround((clientPhysicalRect.bottom - clientPhysicalRect.top) / scaleFactor));

	int requestedLogicWidth = (logic_x2 == 0) ? clientLogicWidth : (logic_x2 - logic_x1);
	int requestedLogicHeight = (logic_y2 == 0) ? clientLogicHeight : (logic_y2 - logic_y1);

	int finalLogicWidth = std::min(requestedLogicWidth, clientLogicWidth - logic_x1);
	int finalLogicHeight = std::min(requestedLogicHeight, clientLogicHeight - logic_y1);
	if (finalLogicWidth <= 0 || finalLogicHeight <= 0) {
		if (auto fn = GetSetThreadDpiAwarenessContextFn()) {
			fn(oldContext);
		}
		return false;
	}

	int physicalCaptureX = clientTopLeftPhysical.x + static_cast<int>(std::lround(logic_x1 * scaleFactor));
	int physicalCaptureY = clientTopLeftPhysical.y + static_cast<int>(std::lround(logic_y1 * scaleFactor));
	int physicalCaptureW = static_cast<int>(std::lround(finalLogicWidth * scaleFactor));
	int physicalCaptureH = static_cast<int>(std::lround(finalLogicHeight * scaleFactor));

	HDC hScreenDC = GetDC(nullptr);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, physicalCaptureW, physicalCaptureH);
	HGDIOBJ oldObj = SelectObject(hMemoryDC, hBitmap);
	BitBlt(hMemoryDC, 0, 0, physicalCaptureW, physicalCaptureH, hScreenDC, physicalCaptureX, physicalCaptureY, SRCCOPY);

	cv::Mat physicalMat(physicalCaptureH, physicalCaptureW, CV_8UC4);
	BITMAPINFOHEADER bi = {};
	bi.biSize = sizeof(bi);
	bi.biWidth = physicalCaptureW;
	bi.biHeight = -physicalCaptureH;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	GetDIBits(hMemoryDC, hBitmap, 0, physicalCaptureH, physicalMat.data, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

	SelectObject(hMemoryDC, oldObj);
	DeleteObject(hBitmap);
	DeleteDC(hMemoryDC);
	ReleaseDC(nullptr, hScreenDC);

	if (auto fn = GetSetThreadDpiAwarenessContextFn()) {
		fn(oldContext);
	}

	cv::resize(physicalMat, outMat, cv::Size(finalLogicWidth, finalLogicHeight), 0, 0, cv::INTER_AREA);
	return !outMat.empty();
}

bool FindPic(HWND hwnd, int logic_x1, int logic_y1, int logic_x2, int logic_y2,
	const std::wstring& pic_name, double sim, long& outx, long& outy)
{
	outx = -1;
	outy = -1;

	cv::Mat screenMatLogical;
	if (!CaptureAndResizeToLogic(hwnd, logic_x1, logic_y1, logic_x2, logic_y2, screenMatLogical)) {
		return false;
	}

	std::vector<std::wstring> imagePaths;
	std::wstringstream wss(pic_name);
	std::wstring singlePath;
	while (std::getline(wss, singlePath, L'|')) {
		if (!singlePath.empty()) imagePaths.push_back(singlePath);
	}

	cv::Mat preparedScreenMat = screenMatLogical;
	if (preparedScreenMat.channels() == 4) {
		cv::cvtColor(preparedScreenMat, preparedScreenMat, cv::COLOR_BGRA2BGR);
	}

	for (const auto& currentPath : imagePaths) {
		std::string path_s(currentPath.begin(), currentPath.end());
		cv::Mat templateMat = cv::imread(path_s, cv::IMREAD_COLOR);
		if (templateMat.empty()) continue;

		if (templateMat.channels() == 4) {
			cv::cvtColor(templateMat, templateMat, cv::COLOR_BGRA2BGR);
		}

		if (preparedScreenMat.channels() != templateMat.channels()) continue;

		cv::Mat result;
		cv::matchTemplate(preparedScreenMat, templateMat, result, cv::TM_CCOEFF_NORMED);

		double minVal = 0.0, maxVal = 0.0;
		cv::Point minLoc, maxLoc;
		cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

		if (maxVal >= sim) {
			outx = maxLoc.x + logic_x1;
			outy = maxLoc.y + logic_y1;
			return true;
		}
	}

	return false;
}

void CapturePng(HWND hwnd, long logic_x1, long logic_y1, long logic_x2, long logic_y2, const TCHAR* file_name) {
	if (file_name == nullptr || *file_name == _T('\0')) return;

	cv::Mat captured;
	if (!CaptureAndResizeToLogic(hwnd, static_cast<int>(logic_x1), static_cast<int>(logic_y1), static_cast<int>(logic_x2), static_cast<int>(logic_y2), captured)) {
		return;
	}

	std::string outPath = TCharPathToAnsi(file_name);
	if (outPath.empty()) return;

	try {
		cv::imwrite(outPath, captured);
	}
	catch (...) {
	}
}

void KeyDown(const std::wstring& key) {
	{
		std::lock_guard<std::mutex> lock(g_keyStateMutex);
		g_pressedKeys.insert(key);
	}
	int mode = GetWuyaInputModeSafe();
	if (mode != kWuyaModeNativeOnly) {
		if (TryHkmKey(key, false)) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
		if (mode == kWuyaModeExternalOnly) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
	}
	g_lastInputBackend.store(kWuyaBackendNative);
	SendKeyInput(key, false);
}

void KeyUp(const std::wstring& key) {
	{
		std::lock_guard<std::mutex> lock(g_keyStateMutex);
		g_pressedKeys.erase(key);
	}
	int mode = GetWuyaInputModeSafe();
	if (mode != kWuyaModeNativeOnly) {
		if (TryHkmKey(key, true)) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
		if (mode == kWuyaModeExternalOnly) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
	}
	g_lastInputBackend.store(kWuyaBackendNative);
	SendKeyInput(key, true);
}

void KeyPress(const std::wstring& key, int times, long delay_after_ms) {
	for (int i = 0; i < times; ++i) {
		KeyDown(key);
		Sleep(20 + rand() % 15);
		KeyUp(key);
	}
	if (delay_after_ms > 0) Sleep(delay_after_ms);
}

void ReleaseAllKeys() {
	std::vector<std::wstring> keys;
	{
		std::lock_guard<std::mutex> lock(g_keyStateMutex);
		keys.assign(g_pressedKeys.begin(), g_pressedKeys.end());
		g_pressedKeys.clear();
	}

	int mode = GetWuyaInputModeSafe();
	int lastBackend = g_lastInputBackend.load();
	if (mode != kWuyaModeNativeOnly || lastBackend == kWuyaBackendExternal) {
		ReleaseHkmKeyboard();
		ReleaseHkmMouse();
	}

	for (const auto& key : keys) {
		KeyUp(key);
	}

	ReleaseNativeMouseButtons();
	if (!keys.empty()) {
		return;
	}

	static const WORD kReleaseKeys[] = {
		VK_SHIFT, VK_CONTROL, VK_MENU,
		VK_LSHIFT, VK_RSHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU,
		VK_SPACE, VK_TAB, VK_ESCAPE, VK_RETURN, VK_BACK,
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_HOME, VK_END, VK_PRIOR, VK_NEXT,
		VK_INSERT, VK_DELETE, VK_CAPITAL
	};
	for (WORD vk : kReleaseKeys) {
		SendVirtualKeyUp(vk);
	}
	for (WORD vk = 'A'; vk <= 'Z'; ++vk) {
		SendVirtualKeyUp(vk);
	}
	for (WORD vk = '0'; vk <= '9'; ++vk) {
		SendVirtualKeyUp(vk);
	}
}

void MoveTo(HWND hwnd, int logicalX, int logicalY, int delay_after_ms) {
	int mode = GetWuyaInputModeSafe();
	if (mode != kWuyaModeNativeOnly) {
		if (TryHkmMoveTo(hwnd, logicalX, logicalY, delay_after_ms)) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
		if (mode == kWuyaModeExternalOnly) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
	}
	g_lastInputBackend.store(kWuyaBackendNative);

	POINT physicalPoint;
	if (!LogicalClientToPhysicalScreen(hwnd, logicalX, logicalY, physicalPoint)) return;

	const int xVirtual = GetSystemMetrics(SM_XVIRTUALSCREEN);
	const int yVirtual = GetSystemMetrics(SM_YVIRTUALSCREEN);
	const int cxVirtual = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	const int cyVirtual = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	if (cxVirtual <= 1 || cyVirtual <= 1) return;

	double normalizedX = (static_cast<double>(physicalPoint.x - xVirtual) * 65535.0) / (cxVirtual - 1);
	double normalizedY = (static_cast<double>(physicalPoint.y - yVirtual) * 65535.0) / (cyVirtual - 1);

	INPUT input = {};
	input.type = INPUT_MOUSE;
	input.mi.dx = static_cast<LONG>(normalizedX);
	input.mi.dy = static_cast<LONG>(normalizedY);
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(INPUT));

	if (delay_after_ms > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_after_ms));
	}
}

void LeftClick(HWND hwnd, int logicalX, int logicalY, int times, int clickDelayMs, int moveDelayMs) {
	int mode = GetWuyaInputModeSafe();
	if (mode != kWuyaModeNativeOnly) {
		if (TryHkmLeftClick(hwnd, logicalX, logicalY, times, clickDelayMs, moveDelayMs)) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
		if (mode == kWuyaModeExternalOnly) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
	}
	g_lastInputBackend.store(kWuyaBackendNative);

	MoveTo(hwnd, logicalX, logicalY, moveDelayMs);

	INPUT down = {};
	down.type = INPUT_MOUSE;
	down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	INPUT up = {};
	up.type = INPUT_MOUSE;
	up.mi.dwFlags = MOUSEEVENTF_LEFTUP;

	for (int i = 0; i < times; ++i) {
		SendInput(1, &down, sizeof(INPUT));
		std::this_thread::sleep_for(std::chrono::milliseconds(std::max(1, clickDelayMs / 2)));
		SendInput(1, &up, sizeof(INPUT));
		if (i + 1 < times) {
			std::this_thread::sleep_for(std::chrono::milliseconds(std::max(1, clickDelayMs / 2)));
		}
	}
}

void LeftClickCurrent(int times, int clickDelayMs) {
	int mode = GetWuyaInputModeSafe();
	if (mode != kWuyaModeNativeOnly) {
		if (TryHkmLeftClickCurrent(times, clickDelayMs)) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
		if (mode == kWuyaModeExternalOnly) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
	}
	g_lastInputBackend.store(kWuyaBackendNative);

	INPUT down = {};
	down.type = INPUT_MOUSE;
	down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	INPUT up = {};
	up.type = INPUT_MOUSE;
	up.mi.dwFlags = MOUSEEVENTF_LEFTUP;

	for (int i = 0; i < times; ++i) {
		SendInput(1, &down, sizeof(INPUT));
		std::this_thread::sleep_for(std::chrono::milliseconds(std::max(1, clickDelayMs / 2)));
		SendInput(1, &up, sizeof(INPUT));
		if (i + 1 < times) {
			std::this_thread::sleep_for(std::chrono::milliseconds(std::max(1, clickDelayMs / 2)));
		}
	}
}

void RightClick(int times, int clickDelayMs) {
	int mode = GetWuyaInputModeSafe();
	if (mode != kWuyaModeNativeOnly) {
		if (TryHkmRightClick(times, clickDelayMs)) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
		if (mode == kWuyaModeExternalOnly) {
			g_lastInputBackend.store(kWuyaBackendExternal);
			return;
		}
	}
	g_lastInputBackend.store(kWuyaBackendNative);

	INPUT down = {};
	down.type = INPUT_MOUSE;
	down.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	INPUT up = {};
	up.type = INPUT_MOUSE;
	up.mi.dwFlags = MOUSEEVENTF_RIGHTUP;

	for (int i = 0; i < times; ++i) {
		SendInput(1, &down, sizeof(INPUT));
		std::this_thread::sleep_for(std::chrono::milliseconds(std::max(1, clickDelayMs / 2)));
		SendInput(1, &up, sizeof(INPUT));
		if (i + 1 < times) {
			std::this_thread::sleep_for(std::chrono::milliseconds(std::max(1, clickDelayMs / 2)));
		}
	}
}

bool GetWindowRectLong(long hwnd_long, long* x1, long* y1, long* x2, long* y2) {
	HWND hwnd = reinterpret_cast<HWND>(static_cast<intptr_t>(hwnd_long));
	if (hwnd == nullptr || !IsWindow(hwnd) || x1 == nullptr || y1 == nullptr || x2 == nullptr || y2 == nullptr) return false;
	RECT r;
	if (!::GetWindowRect(hwnd, &r)) return false;
	*x1 = r.left;
	*y1 = r.top;
	*x2 = r.right;
	*y2 = r.bottom;
	return true;
}

long GetWindowProcessIdLong(long hwnd_long) {
	HWND hwnd = reinterpret_cast<HWND>(static_cast<intptr_t>(hwnd_long));
	if (hwnd == nullptr || !IsWindow(hwnd)) return 0;
	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	return static_cast<long>(pid);
}

bool ActivateWindowLong(long hwnd_long) {
	HWND hwnd = reinterpret_cast<HWND>(static_cast<intptr_t>(hwnd_long));
	if (hwnd == nullptr || !IsWindow(hwnd)) return false;

	ShowWindow(hwnd, SW_SHOWNORMAL);
	SetForegroundWindow(hwnd);
	SetActiveWindow(hwnd);
	return true;
}

} // namespace SPUtils

int GetWuyaInputBackend() {
	return g_lastInputBackend.load();
}

int GetWuyaInputDeviceReady() {
	auto& ctx = GetHkmContext();
	if (ctx.dev) return 1;
	DWORD devId = FindHkmDeviceId();
	return (devId == HKM_FAIL) ? 0 : 1;
}
