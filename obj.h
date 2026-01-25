#pragma once

// GlacialTool_AngelV: no-DM replacement backend.
// Keep the original gameplay logic unchanged by providing a DM-compatible API surface.

#include <windows.h>
#include <afxstr.h>
#include <string>
#include <unordered_set>

class sptool {
public:
	sptool();
	~sptool();

	// Basic / compatibility
	CString Ver();
	long GetDmCount();
	long GetLastError();
	void ReleaseRef();
	void SetExitThread(long en);

	// Registration / guard / dict (no-op for compatibility)
	long Reg(const TCHAR* code, const TCHAR* ver);
	long RegEx(const TCHAR* code, const TCHAR* ver, const TCHAR* ip);
	long DmGuard(long enable, const TCHAR* param);
	long DmGuardExtract(const TCHAR* type, const TCHAR* out_file);
	long DmGuardLoadCustom(const TCHAR* type, const TCHAR* path);
	long EnableShareDict(long en);
	long SetPath(const TCHAR* path);
	long SetDict(long index, const TCHAR* file);
	long SetShowErrorMsg(long show);

	// Window binding (no background binding; we track a target HWND)
	long BindWindowEx(long hwnd, const TCHAR* display, const TCHAR* mouse, const TCHAR* keypad, const TCHAR* public_desc, long mode);
	long UnBindWindow();
	long EnableBind(long en);
	long SetWindowState(long hwnd, long flag);
	long GetWindowRect(long hwnd, long* x1, long* y1, long* x2, long* y2);
	long GetWindowProcessId(long hwnd);
	long GetWindow(long hwnd, long flag);
	long TerminateProcess(long pid);
	long ForceUnBindWindow(long hwnd);
	long IsBind(long hwnd);

	// Time / capture / recognition
	long GetTime();
	long CapturePng(long x1, long y1, long x2, long y2, const TCHAR* file_name);
	long FindPic(long x1, long y1, long x2, long y2, const TCHAR* pic_name, const TCHAR* delta_color, double sim, long dir, long* x, long* y);
	CString GetColor(long x, long y);

	// Input
	long EnableRealMouse(long type, long delay, long step);
	long LockInput(long flags);
	long KeyDownChar(const TCHAR* key);
	long KeyUpChar(const TCHAR* key);
	long KeyPressChar(const TCHAR* key);
	long MoveTo(long x, long y);
	long LeftClick();
	long RightClick();

	// Enumeration helpers used by UI
	CString EnumWindowByProcess(const TCHAR* process_name, const TCHAR* title, const TCHAR* class_name, long filter);
	CString EnumWindow(long parent, const TCHAR* title, const TCHAR* class_name, long filter);

private:
	bool EnsureCpContext();
	void ResetCpContext();
	bool EnsureCpPicLoaded(const std::wstring& path);

	long m_lastError;
	long m_exitThread;
	long m_boundHwnd;
	LPVOID m_cpData;
	LPVOID m_cpPicTable;
	long m_cpHwnd;
	std::unordered_set<std::wstring> m_cpLoadedPics;
};
