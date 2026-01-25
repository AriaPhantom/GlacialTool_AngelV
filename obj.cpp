#include "stdafx.h"

#include "obj.h"

#include "SPUtils.h"

#include <algorithm>
#include <cwctype>
#include <string>
#include <vector>

namespace {

HWND LongToHwnd(long hwnd_long) {
	return reinterpret_cast<HWND>(static_cast<intptr_t>(hwnd_long));
}

std::wstring ToWString(const TCHAR* t) {
	if (t == nullptr) return L"";
#ifdef UNICODE
	return std::wstring(t);
#else
	int wideLen = MultiByteToWideChar(CP_ACP, 0, t, -1, nullptr, 0);
	if (wideLen <= 0) return L"";
	std::wstring out(static_cast<size_t>(wideLen), L'\0');
	MultiByteToWideChar(CP_ACP, 0, t, -1, &out[0], wideLen);
	out.erase(std::find(out.begin(), out.end(), L'\0'), out.end());
	return out;
#endif
}

std::wstring GetFileNameLower(const std::wstring& fullPath) {
	std::wstring name = fullPath;
	size_t pos = name.find_last_of(L"\\/");
	if (pos != std::wstring::npos) name = name.substr(pos + 1);
	std::transform(name.begin(), name.end(), name.begin(), ::towlower);
	return name;
}

bool GetProcessImageName(DWORD pid, std::wstring& outNameLower) {
	outNameLower.clear();

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (!hProcess) return false;

	wchar_t buffer[MAX_PATH] = {};
	DWORD size = static_cast<DWORD>(sizeof(buffer) / sizeof(buffer[0]));
	bool ok = false;
	if (QueryFullProcessImageNameW(hProcess, 0, buffer, &size)) {
		outNameLower = GetFileNameLower(std::wstring(buffer, size));
		ok = true;
	}
	CloseHandle(hProcess);
	return ok;
}

struct EnumWinParams {
	std::wstring processNameLower;
	std::wstring titleNeedle;
	std::wstring classNameNeedle;
	std::vector<long> hwnds;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	auto* p = reinterpret_cast<EnumWinParams*>(lParam);
	if (!IsWindow(hwnd) || !IsWindowVisible(hwnd)) return TRUE;

	if (!p->classNameNeedle.empty()) {
		wchar_t cls[256] = {};
		GetClassNameW(hwnd, cls, static_cast<int>(sizeof(cls) / sizeof(cls[0])));
		if (_wcsicmp(cls, p->classNameNeedle.c_str()) != 0) return TRUE;
	}

	if (!p->titleNeedle.empty()) {
		wchar_t title[512] = {};
		GetWindowTextW(hwnd, title, static_cast<int>(sizeof(title) / sizeof(title[0])));
		if (wcsstr(title, p->titleNeedle.c_str()) == nullptr) return TRUE;
	}

	if (!p->processNameLower.empty()) {
		DWORD pid = 0;
		GetWindowThreadProcessId(hwnd, &pid);
		std::wstring exeLower;
		if (!GetProcessImageName(pid, exeLower)) return TRUE;
		if (exeLower != p->processNameLower) return TRUE;
	}

	p->hwnds.push_back(static_cast<long>(reinterpret_cast<intptr_t>(hwnd)));
	return TRUE;
}

CString JoinHwnds(const std::vector<long>& hwnds) {
	CString out;
	for (size_t i = 0; i < hwnds.size(); ++i) {
		CString part;
		part.Format(_T("%ld"), hwnds[i]);
		out += part;
		if (i + 1 < hwnds.size()) out += _T(",");
	}
	return out;
}

} // namespace

sptool::sptool() : m_lastError(0), m_exitThread(0), m_boundHwnd(0) {}
sptool::~sptool() = default;

CString sptool::Ver() {
	return _T("GlacialTool_AngelV_NoDM");
}

long sptool::GetDmCount() {
	return 1;
}

long sptool::GetLastError() {
	return m_lastError;
}

void sptool::ReleaseRef() {
	// Compatibility no-op.
}

void sptool::SetExitThread(long en) {
	m_exitThread = en;
}

long sptool::Reg(const TCHAR* /*code*/, const TCHAR* /*ver*/) {
	return 1;
}

long sptool::RegEx(const TCHAR* /*code*/, const TCHAR* /*ver*/, const TCHAR* /*ip*/) {
	return 1;
}

long sptool::DmGuard(long /*enable*/, const TCHAR* /*param*/) {
	return 1;
}

long sptool::DmGuardExtract(const TCHAR* /*type*/, const TCHAR* /*out_file*/) {
	return 1;
}

long sptool::DmGuardLoadCustom(const TCHAR* /*type*/, const TCHAR* /*path*/) {
	return 1;
}

long sptool::EnableShareDict(long /*en*/) {
	return 1;
}

long sptool::SetPath(const TCHAR* /*path*/) {
	return 1;
}

long sptool::SetDict(long /*index*/, const TCHAR* /*file*/) {
	return 1;
}

long sptool::SetShowErrorMsg(long /*show*/) {
	return 1;
}

long sptool::BindWindowEx(long hwnd, const TCHAR* /*display*/, const TCHAR* /*mouse*/, const TCHAR* /*keypad*/, const TCHAR* /*public_desc*/, long /*mode*/) {
	HWND h = LongToHwnd(hwnd);
	if (h == nullptr || !IsWindow(h)) {
		m_lastError = GetLastError();
		return 0;
	}
	m_boundHwnd = hwnd;
	SPUtils::ActivateWindowLong(hwnd);
	return 1;
}

long sptool::UnBindWindow() {
	m_boundHwnd = 0;
	return 1;
}

long sptool::EnableBind(long /*en*/) {
	// No background bind; keep for compatibility.
	return 1;
}

long sptool::SetWindowState(long hwnd, long /*flag*/) {
	return SPUtils::ActivateWindowLong(hwnd) ? 1 : 0;
}

long sptool::GetWindowRect(long hwnd, long* x1, long* y1, long* x2, long* y2) {
	return SPUtils::GetWindowRectLong(hwnd, x1, y1, x2, y2) ? 1 : 0;
}

long sptool::GetWindowProcessId(long hwnd) {
	return SPUtils::GetWindowProcessIdLong(hwnd);
}

long sptool::GetWindow(long hwnd, long /*flag*/) {
	return hwnd;
}

long sptool::TerminateProcess(long pid) {
	if (pid <= 0) return 0;
	HANDLE h = OpenProcess(PROCESS_TERMINATE, FALSE, static_cast<DWORD>(pid));
	if (!h) return 0;
	BOOL ok = ::TerminateProcess(h, 0);
	CloseHandle(h);
	return ok ? 1 : 0;
}

long sptool::ForceUnBindWindow(long hwnd) {
	if (m_boundHwnd != 0 && m_boundHwnd == hwnd) {
		m_boundHwnd = 0;
	}
	return 1;
}

long sptool::IsBind(long hwnd) {
	return (m_boundHwnd != 0 && m_boundHwnd == hwnd) ? 1 : 0;
}

long sptool::GetTime() {
	return SPUtils::GetTimeMs();
}

long sptool::CapturePng(long x1, long y1, long x2, long y2, const TCHAR* file_name) {
	HWND hwnd = LongToHwnd(m_boundHwnd);
	if (hwnd == nullptr || !IsWindow(hwnd)) return 0;
	SPUtils::CapturePng(hwnd, x1, y1, x2, y2, file_name);
	return 1;
}

long sptool::FindPic(long x1, long y1, long x2, long y2, const TCHAR* pic_name, const TCHAR* /*delta_color*/, double sim, long /*dir*/, long* x, long* y) {
	if (x) *x = -1;
	if (y) *y = -1;

	HWND hwnd = LongToHwnd(m_boundHwnd);
	if (hwnd == nullptr || !IsWindow(hwnd)) return 0;

	long outx = -1, outy = -1;
	bool found = SPUtils::FindPic(hwnd, static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2), ToWString(pic_name), sim, outx, outy);
	if (found) {
		if (x) *x = outx;
		if (y) *y = outy;
		return 1;
	}
	return 0;
}

CString sptool::GetColor(long x, long y) {
	HWND hwnd = LongToHwnd(m_boundHwnd);
	if (hwnd == nullptr || !IsWindow(hwnd)) return _T("");

	cv::Mat mat;
	if (!SPUtils::CaptureAndResizeToLogic(hwnd, static_cast<int>(x), static_cast<int>(y), static_cast<int>(x + 1), static_cast<int>(y + 1), mat)) {
		return _T("");
	}
	if (mat.empty()) return _T("");

	int b = 0, g = 0, r = 0;
	if (mat.channels() >= 3) {
		cv::Vec4b px = mat.at<cv::Vec4b>(0, 0);
		b = px[0]; g = px[1]; r = px[2];
	}
	CString out;
	out.Format(_T("%02X%02X%02X"), r, g, b);
	return out;
}

long sptool::EnableRealMouse(long /*type*/, long /*delay*/, long /*step*/) {
	return 1;
}

long sptool::LockInput(long /*flags*/) {
	return 1;
}

long sptool::KeyDownChar(const TCHAR* key) {
	SPUtils::KeyDown(ToWString(key));
	return 1;
}

long sptool::KeyUpChar(const TCHAR* key) {
	SPUtils::KeyUp(ToWString(key));
	return 1;
}

long sptool::KeyPressChar(const TCHAR* key) {
	SPUtils::KeyPress(ToWString(key), 1, 0);
	return 1;
}

long sptool::MoveTo(long x, long y) {
	HWND hwnd = LongToHwnd(m_boundHwnd);
	if (hwnd == nullptr || !IsWindow(hwnd)) return 0;
	SPUtils::MoveTo(hwnd, static_cast<int>(x), static_cast<int>(y), 0);
	return 1;
}

long sptool::LeftClick() {
	// DM's default LeftClick() clicks at current cursor position.
	INPUT down = {};
	down.type = INPUT_MOUSE;
	down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	INPUT up = {};
	up.type = INPUT_MOUSE;
	up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &down, sizeof(INPUT));
	Sleep(15);
	SendInput(1, &up, sizeof(INPUT));
	return 1;
}

long sptool::RightClick() {
	SPUtils::RightClick(1, 30);
	return 1;
}

CString sptool::EnumWindowByProcess(const TCHAR* process_name, const TCHAR* title, const TCHAR* class_name, long /*filter*/) {
	EnumWinParams p;
	p.processNameLower = ToWString(process_name);
	std::transform(p.processNameLower.begin(), p.processNameLower.end(), p.processNameLower.begin(), ::towlower);
	p.titleNeedle = ToWString(title);
	p.classNameNeedle = ToWString(class_name);

	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&p));
	return JoinHwnds(p.hwnds);
}

CString sptool::EnumWindow(long /*parent*/, const TCHAR* title, const TCHAR* class_name, long /*filter*/) {
	EnumWinParams p;
	p.titleNeedle = ToWString(title);
	p.classNameNeedle = ToWString(class_name);
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&p));
	return JoinHwnds(p.hwnds);
}
