#include "stdafx.h"
#include "StartupVerify.h"

#include "ProjVer.h"
#include "log.h"

#include <windows.h>
#include <winreg.h>
#include <intrin.h>

#include <cpr/cpr.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace {

const char* kVerifyUrl = "http://121.36.42.119:8081/api/verify";
const char* kAuthToken = "InD36Pqa7z0D9mIy5g2Djgr1z3fIZmGxXJZ5yCfSi2c";
const bool kFailOpen = true;
const int kTimeoutMs = 5000;

std::string WideToUtf8(const std::wstring& value)
{
	if (value.empty()) {
		return std::string();
	}

	int size = WideCharToMultiByte(CP_UTF8, 0, value.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (size <= 1) {
		return std::string();
	}

	std::string result(size - 1, '\0');
	WideCharToMultiByte(CP_UTF8, 0, value.c_str(), -1, &result[0], size, nullptr, nullptr);
	return result;
}

std::wstring ReadRegistryString(HKEY root, const wchar_t* subkey, const wchar_t* value_name)
{
	DWORD size = 0;
	LSTATUS status = RegGetValueW(root, subkey, value_name, RRF_RT_REG_SZ, nullptr, nullptr, &size);
	if (status != ERROR_SUCCESS || size == 0) {
		return std::wstring();
	}

	std::wstring buffer(size / sizeof(wchar_t), L'\0');
	status = RegGetValueW(root, subkey, value_name, RRF_RT_REG_SZ, nullptr, &buffer[0], &size);
	if (status != ERROR_SUCCESS) {
		return std::wstring();
	}

	if (!buffer.empty() && buffer.back() == L'\0') {
		buffer.pop_back();
	}
	return buffer;
}

std::string GetMachineGuid()
{
	std::wstring guid = ReadRegistryString(
		HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Cryptography",
		L"MachineGuid");

	return WideToUtf8(guid);
}

std::string GetComputerNameUtf8()
{
	wchar_t name[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	if (!GetComputerNameW(name, &size)) {
		return std::string();
	}
	return WideToUtf8(name);
}

std::string GetUserNameUtf8()
{
	std::vector<wchar_t> buffer(256, L'\0');
	DWORD size = static_cast<DWORD>(buffer.size());
	if (!GetUserNameW(buffer.data(), &size)) {
		return std::string();
	}
	return WideToUtf8(buffer.data());
}

std::string GetOsVersionString()
{
	OSVERSIONINFOEXW info = {};
	info.dwOSVersionInfoSize = sizeof(info);

	using RtlGetVersionPtr = LONG(WINAPI*)(OSVERSIONINFOEXW*);
	auto rtl_get_version = reinterpret_cast<RtlGetVersionPtr>(
		GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion"));

	if (rtl_get_version && rtl_get_version(&info) == 0) {
		// ok
	}
	else {
		using GetVersionExWPtr = BOOL(WINAPI*)(LPOSVERSIONINFOW);
		auto get_version_ex = reinterpret_cast<GetVersionExWPtr>(
			GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetVersionExW"));
		if (get_version_ex) {
			get_version_ex(reinterpret_cast<OSVERSIONINFOW*>(&info));
		}
	}

	std::ostringstream ss;
	ss << info.dwMajorVersion << "." << info.dwMinorVersion << "." << info.dwBuildNumber;
	return ss.str();
}

std::string GetCpuBrand()
{
	int cpu_info[4] = { 0 };
	char brand[0x40] = { 0 };

	__cpuid(cpu_info, 0x80000000);
	unsigned int max_ext = static_cast<unsigned int>(cpu_info[0]);
	if (max_ext < 0x80000004) {
		return std::string();
	}

	__cpuid(cpu_info, 0x80000002);
	std::memcpy(brand, cpu_info, sizeof(cpu_info));
	__cpuid(cpu_info, 0x80000003);
	std::memcpy(brand + 16, cpu_info, sizeof(cpu_info));
	__cpuid(cpu_info, 0x80000004);
	std::memcpy(brand + 32, cpu_info, sizeof(cpu_info));

	return std::string(brand);
}

std::string GetBuildTag()
{
#if defined(NL)
	return "NL";
#elif defined(Angel)
	return "Angel";
#elif defined(Adele)
	return "Adele";
#else
	return "Unknown";
#endif
}

std::string NormalizeBody(const std::string& body)
{
	std::string normalized;
	normalized.reserve(body.size());
	for (unsigned char ch : body) {
		if (!std::isspace(ch)) {
			normalized.push_back(static_cast<char>(std::tolower(ch)));
		}
	}
	return normalized;
}

bool ParseAllowResponse(const std::string& body)
{
	std::string normalized = NormalizeBody(body);
	if (normalized.find("\"allow\":true") != std::string::npos) {
		return true;
	}
	if (normalized.find("\"allow\":1") != std::string::npos) {
		return true;
	}
	if (normalized.find("allow=true") != std::string::npos) {
		return true;
	}
	if (normalized == "allow" || normalized == "ok") {
		return true;
	}
	return false;
}

}

bool VerifyStartup(std::string* out_reason)
{
	const std::string machine_id = GetMachineGuid();
	const std::string hostname = GetComputerNameUtf8();
	const std::string username = GetUserNameUtf8();
	const std::string os_version = GetOsVersionString();
	const std::string cpu = GetCpuBrand();
	const std::string build_tag = GetBuildTag();

	cpr::Payload payload{
		{ "machine_id", machine_id },
		{ "hostname", hostname },
		{ "username", username },
		{ "os_version", os_version },
		{ "cpu", cpu },
		{ "build_tag", build_tag },
		{ "app", "GlacialTool_AngelV" },
	};

	cpr::Header headers;
	if (kAuthToken[0] != '\0') {
		headers["X-Auth-Token"] = kAuthToken;
	}

	cpr::Response response = cpr::Post(
		cpr::Url{ kVerifyUrl },
		headers,
		payload,
		cpr::VerifySsl(false),
		cpr::Timeout{ kTimeoutMs });

	if (response.error) {
		Log(_T("Startup verify: request error."));
		if (out_reason) {
			*out_reason = response.error.message;
		}
		return kFailOpen;
	}

	if (response.status_code != 200) {
		Log(_T("Startup verify: non-200 response."));
		if (out_reason) {
			*out_reason = response.text;
		}
		return kFailOpen;
	}

	if (!ParseAllowResponse(response.text)) {
		Log(_T("Startup verify: denied."));
		if (out_reason) {
			*out_reason = response.text;
		}
		return false;
	}

	Log(_T("Startup verify: allowed."));
	return true;
}
