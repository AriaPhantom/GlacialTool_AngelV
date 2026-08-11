#include "stdafx.h"

#include "ScreenRecorder.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wrl/client.h>

#include <atomic>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

namespace {

using Microsoft::WRL::ComPtr;

constexpr UINT32 kFramesPerSecond = 10;
constexpr UINT32 kRecordingSeconds = 60;
constexpr UINT32 kAverageBitrate = 8'000'000;
constexpr int kCaptureX = 310;
constexpr int kCaptureY = 116;
constexpr UINT32 kCaptureWidth = 746;
constexpr UINT32 kCaptureHeight = 496;
constexpr LONGLONG kFrameDuration = 10'000'000LL / kFramesPerSecond;

std::atomic<bool> g_recording{ false };

struct GameWindowCapture {
	HWND gameWindow = nullptr;
	int x = 0;
	int y = 0;
	UINT32 width = 0;
	UINT32 height = 0;
	HDC screenDc = nullptr;
	HDC memoryDc = nullptr;
	HBITMAP bitmap = nullptr;
	HGDIOBJ previousObject = nullptr;
	BYTE* pixels = nullptr;

	~GameWindowCapture() {
		if (memoryDc && previousObject) SelectObject(memoryDc, previousObject);
		if (bitmap) DeleteObject(bitmap);
		if (memoryDc) DeleteDC(memoryDc);
		if (screenDc) ReleaseDC(nullptr, screenDc);
	}

	bool Initialize(HWND window) {
		if (!IsWindow(window) || IsIconic(window)) return false;
		gameWindow = window;
		RECT clientRect = {};
		if (!GetClientRect(gameWindow, &clientRect)) return false;
		int clientWidth = clientRect.right - clientRect.left;
		int clientHeight = clientRect.bottom - clientRect.top;
		if (clientWidth < kCaptureX + static_cast<int>(kCaptureWidth) ||
			clientHeight < kCaptureY + static_cast<int>(kCaptureHeight)) {
			return false;
		}
		POINT clientOrigin = { 0, 0 };
		if (!ClientToScreen(gameWindow, &clientOrigin)) return false;
		x = clientOrigin.x + kCaptureX;
		y = clientOrigin.y + kCaptureY;

		width = kCaptureWidth;
		height = kCaptureHeight;
		screenDc = GetDC(nullptr);
		if (!screenDc) return false;
		memoryDc = CreateCompatibleDC(screenDc);
		if (!memoryDc) return false;

		BITMAPINFO bitmapInfo = {};
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biWidth = static_cast<LONG>(width);
		bitmapInfo.bmiHeader.biHeight = -static_cast<LONG>(height);
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmap = CreateDIBSection(screenDc, &bitmapInfo, DIB_RGB_COLORS,
			reinterpret_cast<void**>(&pixels), nullptr, 0);
		if (!bitmap || !pixels) return false;
		HGDIOBJ selectedObject = SelectObject(memoryDc, bitmap);
		if (!selectedObject || selectedObject == HGDI_ERROR) return false;
		previousObject = selectedObject;
		return true;
	}

	bool Capture() const {
		if (!IsWindow(gameWindow) || IsIconic(gameWindow)) return false;
		return BitBlt(memoryDc, 0, 0, static_cast<int>(width), static_cast<int>(height),
			screenDc, x, y, SRCCOPY | CAPTUREBLT) != FALSE;
	}
};

std::wstring BuildOutputPath() {
	const wchar_t* root = L"C:\\sptool";
	const wchar_t* recordings = L"C:\\sptool\\recordings";
	CreateDirectoryW(root, nullptr);
	if (!CreateDirectoryW(recordings, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS) {
		return {};
	}

	SYSTEMTIME now = {};
	GetLocalTime(&now);
	std::wostringstream name;
	name << recordings << L"\\lie_roi_"
		<< std::setfill(L'0')
		<< std::setw(4) << now.wYear
		<< std::setw(2) << now.wMonth
		<< std::setw(2) << now.wDay << L'_'
		<< std::setw(2) << now.wHour
		<< std::setw(2) << now.wMinute
		<< std::setw(2) << now.wSecond
		<< std::setw(3) << now.wMilliseconds << L'_'
		<< GetCurrentProcessId() << L".mp4";
	return name.str();
}

HRESULT ConfigureWriter(const std::wstring& outputPath, UINT32 width, UINT32 height,
	ComPtr<IMFSinkWriter>& writer, DWORD& streamIndex) {
	HRESULT hr = MFCreateSinkWriterFromURL(outputPath.c_str(), nullptr, nullptr, &writer);
	if (FAILED(hr)) return hr;

	ComPtr<IMFMediaType> outputType;
	hr = MFCreateMediaType(&outputType);
	if (SUCCEEDED(hr)) hr = outputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	if (SUCCEEDED(hr)) hr = outputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
	if (SUCCEEDED(hr)) hr = outputType->SetUINT32(MF_MT_AVG_BITRATE, kAverageBitrate);
	if (SUCCEEDED(hr)) hr = outputType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	if (SUCCEEDED(hr)) hr = MFSetAttributeSize(outputType.Get(), MF_MT_FRAME_SIZE, width, height);
	if (SUCCEEDED(hr)) hr = MFSetAttributeRatio(outputType.Get(), MF_MT_FRAME_RATE, kFramesPerSecond, 1);
	if (SUCCEEDED(hr)) hr = MFSetAttributeRatio(outputType.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	if (SUCCEEDED(hr)) hr = writer->AddStream(outputType.Get(), &streamIndex);
	if (FAILED(hr)) return hr;

	ComPtr<IMFMediaType> inputType;
	hr = MFCreateMediaType(&inputType);
	if (SUCCEEDED(hr)) hr = inputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	if (SUCCEEDED(hr)) hr = inputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
	if (SUCCEEDED(hr)) hr = inputType->SetUINT32(MF_MT_DEFAULT_STRIDE, width * 4);
	if (SUCCEEDED(hr)) hr = inputType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	if (SUCCEEDED(hr)) hr = MFSetAttributeSize(inputType.Get(), MF_MT_FRAME_SIZE, width, height);
	if (SUCCEEDED(hr)) hr = MFSetAttributeRatio(inputType.Get(), MF_MT_FRAME_RATE, kFramesPerSecond, 1);
	if (SUCCEEDED(hr)) hr = MFSetAttributeRatio(inputType.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	if (SUCCEEDED(hr)) hr = writer->SetInputMediaType(streamIndex, inputType.Get(), nullptr);
	if (SUCCEEDED(hr)) hr = writer->BeginWriting();
	return hr;
}

HRESULT WriteFrame(IMFSinkWriter* writer, DWORD streamIndex, const GameWindowCapture& capture,
	LONGLONG sampleTime) {
	const DWORD frameBytes = capture.width * capture.height * 4;
	ComPtr<IMFMediaBuffer> buffer;
	HRESULT hr = MFCreateMemoryBuffer(frameBytes, &buffer);
	if (FAILED(hr)) return hr;

	BYTE* destination = nullptr;
	hr = buffer->Lock(&destination, nullptr, nullptr);
	if (SUCCEEDED(hr)) {
		memcpy(destination, capture.pixels, frameBytes);
		buffer->Unlock();
	}
	if (SUCCEEDED(hr)) hr = buffer->SetCurrentLength(frameBytes);
	if (FAILED(hr)) return hr;

	ComPtr<IMFSample> sample;
	hr = MFCreateSample(&sample);
	if (SUCCEEDED(hr)) hr = sample->AddBuffer(buffer.Get());
	if (SUCCEEDED(hr)) hr = sample->SetSampleTime(sampleTime);
	if (SUCCEEDED(hr)) hr = sample->SetSampleDuration(kFrameDuration);
	if (SUCCEEDED(hr)) hr = writer->WriteSample(streamIndex, sample.Get());
	return hr;
}

void Record(HWND gameWindow, std::wstring outputPath) {
	struct RecordingFlagReset {
		~RecordingFlagReset() { g_recording.store(false); }
	} resetFlag;

	HRESULT comResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	bool uninitializeCom = SUCCEEDED(comResult);
	HRESULT hr = MFStartup(MF_VERSION);
	if (FAILED(hr)) {
		if (uninitializeCom) CoUninitialize();
		return;
	}

	GameWindowCapture capture;
	ComPtr<IMFSinkWriter> writer;
	DWORD streamIndex = 0;
	if (capture.Initialize(gameWindow)) {
		hr = ConfigureWriter(outputPath, capture.width, capture.height, writer, streamIndex);
	}
	else {
		hr = E_FAIL;
	}

	if (SUCCEEDED(hr)) {
		auto nextFrame = std::chrono::steady_clock::now();
		const UINT32 frameCount = kFramesPerSecond * kRecordingSeconds;
		UINT32 framesWritten = 0;
		for (UINT32 frame = 0; frame < frameCount; ++frame) {
			if (!capture.Capture()) {
				if (framesWritten == 0) hr = E_FAIL;
				break;
			}
			hr = WriteFrame(writer.Get(), streamIndex, capture, frame * kFrameDuration);
			if (FAILED(hr)) break;
			++framesWritten;
			nextFrame += std::chrono::milliseconds(1000 / kFramesPerSecond);
			std::this_thread::sleep_until(nextFrame);
		}
		HRESULT finalizeResult = writer->Finalize();
		if (SUCCEEDED(hr)) hr = finalizeResult;
	}

	writer.Reset();
	MFShutdown();
	if (uninitializeCom) CoUninitialize();
	if (FAILED(hr)) DeleteFileW(outputPath.c_str());
}

} // namespace

bool LieScreenRecorder::Start(HWND gameWindow, std::wstring& outputPath) {
	if (!IsWindow(gameWindow)) return false;
	bool expected = false;
	if (!g_recording.compare_exchange_strong(expected, true)) return false;

	outputPath = BuildOutputPath();
	if (outputPath.empty()) {
		g_recording.store(false);
		return false;
	}

	try {
		std::thread(Record, gameWindow, outputPath).detach();
	}
	catch (...) {
		g_recording.store(false);
		outputPath.clear();
		return false;
	}
	return true;
}
