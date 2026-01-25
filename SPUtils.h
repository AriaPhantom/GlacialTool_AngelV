#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>

#include <opencv2/opencv.hpp>

namespace SPUtils {

// Returns monotonic milliseconds.
long GetTimeMs();

// Capture window client region (logic coords at 100% scale), handle per-monitor DPI, output in logic size.
bool CaptureAndResizeToLogic(HWND hwnd, int logic_x1, int logic_y1, int logic_x2, int logic_y2, cv::Mat& outMat);

// Template match within logic region; pic_name supports '|' separated paths.
bool FindPic(HWND hwnd, int logic_x1, int logic_y1, int logic_x2, int logic_y2,
	const std::wstring& pic_name, double sim, long& outx, long& outy);

// Save PNG of region (logic coords).
void CapturePng(HWND hwnd, long logic_x1, long logic_y1, long logic_x2, long logic_y2, const TCHAR* file_name);

// Keyboard (global SendInput, scan-code based).
void KeyDown(const std::wstring& key);
void KeyUp(const std::wstring& key);
void KeyPress(const std::wstring& key, int times = 1, long delay_after_ms = 0);

// Mouse (coords are logic coords relative to window client).
void MoveTo(HWND hwnd, int logicalX, int logicalY, int delay_after_ms = 0);
void LeftClick(HWND hwnd, int logicalX, int logicalY, int times = 1, int clickDelayMs = 30, int moveDelayMs = 50);
void RightClick(int times = 1, int clickDelayMs = 30);

// Window helpers.
bool GetWindowRectLong(long hwnd_long, long* x1, long* y1, long* x2, long* y2);
long GetWindowProcessIdLong(long hwnd_long);
bool ActivateWindowLong(long hwnd_long);

}
