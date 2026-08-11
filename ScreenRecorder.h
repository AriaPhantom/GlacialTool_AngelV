#pragma once

#include <string>

namespace LieScreenRecorder {

// Starts one asynchronous game-client recording. Repeated calls while active are ignored.
bool Start(HWND gameWindow, std::wstring& outputPath);

}
