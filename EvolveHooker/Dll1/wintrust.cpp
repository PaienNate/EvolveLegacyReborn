#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <winhttp.h>
#include <locale.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <processthreadsapi.h>
#include <windows.h>
#include <direct.h>
#include <iphlpapi.h> // Include winsock2 before this, or winsock2 iphlpapi will be unavailable(goldberg)
#include <shlobj.h>

// C++ 
#include<string>
#include<memory>
#include<vector>
#include<tchar.h>
#include<atlstr.h>
#include <iostream>

// DETOURS(SlimDetours)
#include <KNSoft/SlimDetours/SlimDetours.h> 

// READ INI
#include "simpleini/SimpleIni.h"
#include <evolve/server_core.hpp>

// Using certain input methods can cause game crashes, which can be resolved by forcibly closing the program's input method context.
#include <imm.h>

// ===============
//    VARIABLES
// ===============
CSimpleIniA ini;

// INI SERVER
std::string server_ip;
// FOR WINHTTP HOOK
std::wstring wServerIP;
int server_port;

// INI EMU VARIABLE
bool emu_func = false;
std::string dll_path;
bool enable_console = false;
bool use_internal_server = false;
std::string internal_bind_address = "127.0.0.1";
std::string internal_asset_root = "EvolveCrack";
std::string internal_ca_cert_path = "certs\\mitmproxy-ca-cert.pem";
std::string internal_ca_key_path = "certs\\mitmproxy-ca.pem";
std::string internal_steam_id = "76561101839859666";
INIT_ONCE internal_server_init_once = INIT_ONCE_STATIC_INIT;
HANDLE internal_server_ready_event = nullptr;
HANDLE internal_server_stop_event = nullptr;
HANDLE internal_server_thread = nullptr;
std::unique_ptr<evolve::server::ServerCore> internal_server;
std::string internal_server_error;

LPCWSTR message =
L"补丁制作者 / Patch Creator: Pinenut\n"
L"B站账号 / Bilibili ID: Pinenutn\n\n"
L"本补丁用于实现游戏验证重定向功能。\n"
L"This patch enables game verification redirection.\n\n"
L"特别感谢 / Special Thanks To:\n"
L"- Nemirtingas (cs.rin.ru)\n"
L"- Schmogmog (Discord)\n"
L"- Nemerod (Discord)\n"
L"- Kiagam (Discord)\n"
L"- DeinAlbtraum (Discord)\n"
L"- bluem (Discord)\n"
L"- Archetype_4 (Discord)\n"
L"- Pikapika 和国内进化QQ群(366237012)管理、群友\n"
L"[Pikapika (group owner of Evolution QQ group) and the group admins and members]\n\n"
L"重要声明 / Important Notice:\n"
L"本补丁为免费提供，严禁倒卖或用于不正当获利！\n"
L"This patch is provided for free. Reselling or using it for improper profit is strictly prohibited!\n"
L"点击“确定”开始游戏。\n"
L"Click \"OK\" to start the game.\n\n"
L"官方网站 / Official Website: firehomework.top\n\n";

// ===============
// BETTER LOGGING
// ===============

#include "logging/easylogging++.h"
INITIALIZE_EASYLOGGINGPP
// TODO: LOGGING HOOK FUNCTION WITH PROPER WAY
el::Logger* defaultLogger = el::Loggers::getLogger("default");


// Crack Server
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "ws2_32.lib")
// SteamEmu
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "advapi32.lib")
// IMM Hook
#pragma comment(lib, "imm32.lib")

// ===============
//      UTILS
// ===============

void InitLogging() {
	el::Configurations conf;
	conf.setToDefault();
	conf.set(el::Level::Debug, el::ConfigurationType::Filename, "EVOLVE_LOG.txt");
	conf.set(el::Level::Debug, el::ConfigurationType::ToFile, "true");
	conf.set(el::Level::Debug, el::ConfigurationType::ToStandardOutput, "false");
	conf.set(el::Level::Debug, el::ConfigurationType::Format, "%thread %datetime %msg");
	el::Loggers::reconfigureLogger("default", conf);
}

std::string get_full_program_path() {
	DWORD bufferSize = MAX_PATH;
	std::vector<wchar_t> buffer(bufferSize);

	DWORD length = GetModuleFileNameW(nullptr, buffer.data(), bufferSize);

	if (length == bufferSize && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		bufferSize = 32768;
		buffer.resize(bufferSize);
		length = GetModuleFileNameW(nullptr, buffer.data(), bufferSize);
	}

	if (length == 0) {
		throw std::runtime_error("Failed to get module filename. Error code: " +
			std::to_string(GetLastError()));
	}

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, buffer.data(), -1,
		nullptr, 0, nullptr, nullptr);
	if (size_needed == 0) {
		throw std::runtime_error("Failed to convert wide string to UTF-8. Error code: " +
			std::to_string(GetLastError()));
	}

	std::string result(size_needed, 0);
	if (!WideCharToMultiByte(CP_UTF8, 0, buffer.data(), -1,
		&result[0], size_needed, nullptr, nullptr)) {
		throw std::runtime_error("Failed to convert wide string to UTF-8. Error code: " +
			std::to_string(GetLastError()));
	}

	result.pop_back();

	size_t last_slash = result.find_last_of("\\");
	if (last_slash == std::string::npos) {
		return result;
	}

	return result.substr(0, last_slash + 1);
}

std::wstring utf8_to_wide(const std::string& value) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, NULL, 0);
	if (size_needed <= 0) {
		return L"127.0.0.1";
	}

	std::wstring wide(size_needed - 1, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, &wide[0], size_needed);
	return wide;
}

void update_redirect_server(const std::string& host, int port) {
	server_ip = host;
	server_port = port;
	wServerIP = utf8_to_wide(host);
}

bool is_absolute_path(const std::string& path) {
	if (path.size() > 2 && path[1] == ':') {
		return true;
	}
	if (path.rfind("\\\\", 0) == 0 || path.rfind("/", 0) == 0) {
		return true;
	}
	return false;
}

std::string resolve_runtime_path(const std::string& path) {
	if (path.empty() || is_absolute_path(path)) {
		return path;
	}
	return get_full_program_path() + path;
}

DWORD WINAPI InternalServerThreadProc(LPVOID) {
	evolve::server::ServerConfig config;
	config.bind_address = internal_bind_address;
	config.preferred_port = static_cast<uint16_t>(server_port);
	config.asset_root = resolve_runtime_path(internal_asset_root);
	config.ca_certificate_path = resolve_runtime_path(internal_ca_cert_path);
	config.ca_private_key_path = resolve_runtime_path(internal_ca_key_path);
	config.steam_id = internal_steam_id;

	internal_server = std::make_unique<evolve::server::ServerCore>(std::move(config));
	if (!internal_server->Start()) {
		internal_server_error = internal_server->GetLastError();
		SetEvent(internal_server_ready_event);
		return 1;
	}

	update_redirect_server(internal_server->GetRedirectHost(),
		static_cast<int>(internal_server->GetBoundPort()));
	internal_server_error.clear();
	SetEvent(internal_server_ready_event);

	if (internal_server_stop_event) {
		WaitForSingleObject(internal_server_stop_event, INFINITE);
	}

	if (internal_server) {
		internal_server->Stop();
	}
	return 0;
}

BOOL CALLBACK InitInternalServerOnce(PINIT_ONCE, PVOID, PVOID*) {
	internal_server_ready_event = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	internal_server_stop_event = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (!internal_server_ready_event || !internal_server_stop_event) {
		internal_server_error = "failed to create internal server synchronization objects";
		if (internal_server_ready_event) {
			SetEvent(internal_server_ready_event);
		}
		return TRUE;
	}

	internal_server_thread = CreateThread(nullptr, 0, InternalServerThreadProc, nullptr, 0, nullptr);
	if (!internal_server_thread) {
		internal_server_error = "failed to create internal server thread";
		SetEvent(internal_server_ready_event);
	}
	return TRUE;
}

bool EnsureInternalServerStarted() {
	if (!use_internal_server) {
		return true;
	}

	InitOnceExecuteOnce(&internal_server_init_once, InitInternalServerOnce, nullptr, nullptr);
	if (!internal_server_ready_event) {
		LOG(ERROR) << "[SERVER_EMU] internal server ready event was not created\n";
		return false;
	}

	const DWORD wait_result = WaitForSingleObject(internal_server_ready_event, 10000);
	if (wait_result != WAIT_OBJECT_0) {
		LOG(ERROR) << "[SERVER_EMU] waiting for internal server timed out\n";
		return false;
	}

	if (!internal_server || !internal_server->IsReady()) {
		LOG(ERROR) << "[SERVER_EMU] internal server failed to start: " << internal_server_error << "\n";
		return false;
	}

	return true;
}

void StopInternalServer() {
	if (internal_server_stop_event) {
		SetEvent(internal_server_stop_event);
	}
	if (internal_server_thread) {
		WaitForSingleObject(internal_server_thread, 5000);
		CloseHandle(internal_server_thread);
		internal_server_thread = nullptr;
	}
	if (internal_server_ready_event) {
		CloseHandle(internal_server_ready_event);
		internal_server_ready_event = nullptr;
	}
	if (internal_server_stop_event) {
		CloseHandle(internal_server_stop_event);
		internal_server_stop_event = nullptr;
	}
	internal_server.reset();
}

// =====================
//    CRACK SERVER
// =====================
int (WSAAPI* Real_getaddrinfo)(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA* pHints, PADDRINFOA* ppResult) = getaddrinfo;

// Game seems will cache this ip in somewhere but I can't find where.
int WSAAPI Evolve_getaddrinfo(PCSTR pNodeName, PCSTR pServiceName,
	const ADDRINFOA* pHints, PADDRINFOA* ppResult) {
	if (pNodeName && strstr(pNodeName, "2k.com") != nullptr) {
		if (use_internal_server && !EnsureInternalServerStarted()) {
			return Real_getaddrinfo(pNodeName, pServiceName, pHints, ppResult);
		}
		sockaddr_in* addr = (sockaddr_in*)malloc(sizeof(sockaddr_in));
		if (addr == NULL) {
			return EAI_MEMORY;
		}
		addr->sin_family = AF_INET;
		addr->sin_port = htons(static_cast<uint16_t>(server_port));
		addr->sin_addr.s_addr = inet_addr(server_ip.c_str());
		*ppResult = (PADDRINFOA)malloc(sizeof(ADDRINFOA));
		if (*ppResult == NULL) {
			free(addr);
			return EAI_MEMORY;
		}
		memset(*ppResult, 0, sizeof(ADDRINFOA));
		(*ppResult)->ai_family = AF_INET;
		(*ppResult)->ai_socktype = SOCK_STREAM;
		(*ppResult)->ai_protocol = IPPROTO_TCP;
		(*ppResult)->ai_addrlen = sizeof(sockaddr_in);
		(*ppResult)->ai_addr = (struct sockaddr*)addr;
		return 0;
	}
	return Real_getaddrinfo(pNodeName, pServiceName, pHints, ppResult);
}

HINTERNET(WINAPI* Real_WinHttpConnect)(HINTERNET hSession, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD dwReserved) = WinHttpConnect;

HINTERNET WINAPI Evolve_WinHttpConnect(HINTERNET hSession, LPCWSTR pswzServerName,
	INTERNET_PORT nServerPort, DWORD dwReserved) {
	if (use_internal_server && !EnsureInternalServerStarted()) {
		return Real_WinHttpConnect(hSession, pswzServerName, nServerPort, dwReserved);
	}
	pswzServerName = wServerIP.c_str();
	nServerPort = static_cast<INTERNET_PORT>(server_port);
	return Real_WinHttpConnect(hSession, pswzServerName, nServerPort, dwReserved);
}

HINTERNET(WINAPI* Real_WinHttpOpenRequest)(
	IN HINTERNET hConnect,
	IN LPCWSTR   pwszVerb,
	IN LPCWSTR   pwszObjectName,
	IN LPCWSTR   pwszVersion,
	IN LPCWSTR   pwszReferrer,
	IN LPCWSTR* ppwszAcceptTypes,
	IN DWORD     dwFlags
	) = WinHttpOpenRequest;

HINTERNET WINAPI Evolve_WinHttpOpenRequest(
	IN HINTERNET hConnect,
	IN LPCWSTR   pwszVerb,
	IN LPCWSTR   pwszObjectName,
	IN LPCWSTR   pwszVersion,
	IN LPCWSTR   pwszReferrer,
	IN LPCWSTR* ppwszAcceptTypes,
	IN DWORD     dwFlags
) {
	dwFlags |= WINHTTP_FLAG_SECURE;
	HINTERNET hRequest = Real_WinHttpOpenRequest(hConnect, pwszVerb, pwszObjectName, pwszVersion, pwszReferrer, ppwszAcceptTypes, dwFlags);
	DWORD okdwFlags = SECURITY_FLAG_IGNORE_ALL_CERT_ERRORS;
	if (hRequest) {
		DWORD okdwFlags = SECURITY_FLAG_IGNORE_ALL_CERT_ERRORS;
		WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &okdwFlags, sizeof(okdwFlags));
	}
	else {
		defaultLogger->error("Real_WinHttpOpenRequest failed with error %v", GetLastError());
	}
	return hRequest;
}
// CRACK SERVER REDIRECT CODE END

// =====================
//    Steam EMU HOOK
// =====================
DWORD(WINAPI* Real_GetEnvironmentVariableA)(
	IN LPCSTR lpName,        // 环境变量的名称 / Name of the environment variable
	OUT LPSTR lpBuffer,      // 存放环境变量值的缓冲区 / Buffer to store the environment variable value
	IN DWORD nSize           // 缓冲区大小 / Size of the buffer
	) = GetEnvironmentVariableA;
LSTATUS(WINAPI* Real_RegQueryValueExA)(
	IN HKEY hKey,                // 注册表的句柄 / Handle to the registry key
	IN LPCSTR lpValueName,       // 注册表值的名称 / Name of the registry value
	IN LPDWORD lpReserved,       // 保留参数 / Reserved parameter
	OUT LPDWORD lpType,          // 值的数据类型 / Type of data stored in the value
	OUT LPBYTE lpData,           // 存放注册表值数据的缓冲区 / Buffer to store the registry value data
	IN LPDWORD lpcbData          // 缓冲区的大小 / Size of the buffer
	) = RegQueryValueExA;

DWORD WINAPI Evolve_GetEnvironmentVariableA(
	IN  LPCSTR lpName,
	OUT LPSTR  lpBuffer,
	IN  DWORD  nSize
) {
	// 预定义的 AppId 字符串
	const char* predefinedAppId = "273350";
	const DWORD predefinedAppIdLength = 6;

	// 判断是否是 "SteamAppId" 或 "SteamGameId"
	if (strcmp(lpName, "SteamAppId") == 0 || strcmp(lpName, "SteamGameId") == 0) {
		if (lpBuffer != NULL) {
			if (nSize >= predefinedAppIdLength + 1) {  // +1 是为了容纳 '\0'
				strcpy_s(lpBuffer, nSize, predefinedAppId);
				return predefinedAppIdLength;
			}
			else {
				return predefinedAppIdLength + 1;  // 返回所需的缓冲区大小
			}
		}
	}
	return Real_GetEnvironmentVariableA(lpName, lpBuffer, nSize);
}

LSTATUS WINAPI Evolve_RegQueryValueExA(
	IN HKEY hKey,
	IN LPCSTR lpValueName,
	IN LPDWORD lpReserved,
	OUT LPDWORD lpType,
	OUT LPBYTE lpData,
	IN LPDWORD lpcbData
) {
	std::string keyname = lpValueName ? lpValueName : "";

	if (keyname == "SteamClientDll64") {
		if (lpData != NULL && lpcbData != NULL) {
			// 拼接dll_path
			std::string steamClientPath = get_full_program_path() + dll_path;
			size_t dataSize = steamClientPath.size();

			if (*lpcbData >= dataSize + 1) {
				memcpy(lpData, steamClientPath.c_str(), dataSize + 1); // 复制路径及末尾的 '\0'
				*lpcbData = static_cast<DWORD>(dataSize + 1);
				return ERROR_SUCCESS;
			}
			else {
				*lpcbData = static_cast<DWORD>(dataSize + 1);
				return ERROR_MORE_DATA;
			}
		}
	}
	else if (keyname == "pid") {
		if (lpData != NULL && lpcbData != NULL) {
			DWORD processId = GetCurrentProcessId(); // 获取当前进程的 PID

			if (*lpcbData >= sizeof(DWORD)) {
				memcpy(lpData, &processId, sizeof(processId)); // 将 PID 复制到 lpData
				*lpcbData = sizeof(DWORD); // 设置返回数据的大小
				return ERROR_SUCCESS;
			}
			else {
				*lpcbData = sizeof(DWORD);
				return ERROR_MORE_DATA;
			}
		}
	}
	else if (keyname == "ActiveUser") {
		if (lpData != NULL && lpcbData != NULL) {
			DWORD activeUser = 0x03100004771F810D & 0xFFFFFFFF;

			if (*lpcbData >= sizeof(DWORD)) {
				memcpy(lpData, &activeUser, sizeof(activeUser));
				*lpcbData = sizeof(DWORD);
				return ERROR_SUCCESS;
			}
			else {
				*lpcbData = sizeof(DWORD);
				return ERROR_MORE_DATA;
			}
		}
	}
	// 其他情况调用原始函数
	return Real_RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
// =====================
//       IMM HOOK
// =====================
// TODO: FIND HOW THIS GAME USE IMM


// =====================
//       WINTRUST
// =====================
extern "C" __declspec(dllexport) LONG WINAPI WinVerifyTrust(
	HWND hwnd,
	GUID * pgActionID,
	LPVOID pWVTData
) {
	return LONG(0);
}

// =====================
//       INITIALIZE
// =====================
void InitConfiguration() {
	ini.SetUnicode();
	SI_Error rc = ini.LoadFile((get_full_program_path() + "EvolveLogging.ini").c_str());
	if (rc < 0) {
		MessageBox(
			NULL,
			L"配置文件不存在。\n请检查Bin64_SteamRetail目录是否存在EvolveLogging.ini。\n\n"
			L"The configuration file does not exist.\nPlease check if the EvolveLogging.ini file exists in the Bin64_SteamRetail directory.",
			L"错误 / Error",
			MB_ICONERROR | MB_OK
		);
		exit(0);
	}
	use_internal_server = ini.GetBoolValue("server", "use_internal_server", false);
	server_port = ini.GetLongValue("server", "server_port", 2000);
	internal_bind_address = ini.GetValue("server", "internal_bind_address", "127.0.0.1");
	internal_asset_root = ini.GetValue("server", "internal_asset_root", "EvolveCrack");
	internal_ca_cert_path = ini.GetValue("server", "internal_ca_cert_path", "certs\\mitmproxy-ca-cert.pem");
	internal_ca_key_path = ini.GetValue("server", "internal_ca_key_path", "certs\\mitmproxy-ca.pem");
	internal_steam_id = ini.GetValue("server", "steam_id", "76561101839859666");
	if (use_internal_server) {
		update_redirect_server(internal_bind_address == "0.0.0.0" ? "127.0.0.1" : internal_bind_address,
			server_port);
	}
	else {
		update_redirect_server(ini.GetValue("server", "server_domain", "117.72.125.6"), server_port);
	}
}

void InitHooker() {
	TCHAR szAppName[MAX_PATH] = TEXT("Evolve.exe");
	TCHAR szCurName[MAX_PATH];
	GetModuleFileName(NULL, szCurName, MAX_PATH);
	PathStripPath(szCurName);

	if (StrCmpI(szCurName, szAppName) == 0)
	{
		int x;
		x = MessageBox(GetForegroundWindow(), message, L"【注意事项/NOTICE】", 1);
		if (x != 1)
		{
			exit(0);
		}
		HRESULT hr;
		hr = SlimDetoursInlineHook(TRUE, &(PVOID&)Real_getaddrinfo, Evolve_getaddrinfo);
		if (FAILED(hr)) {
			LOG(ERROR) << "[SERVER_EMU] Evolve_getaddrinfo hook failed\n";
		}
		LOG(DEBUG) << "[SERVER_EMU] Evolve_getaddrinfo hook success\n";
		hr = SlimDetoursInlineHook(TRUE, &(PVOID&)Real_WinHttpConnect, Evolve_WinHttpConnect);
		if (FAILED(hr)) {
			LOG(ERROR) << "[SERVER_EMU] Evolve_WinHttpConnect hook failed\n";
		}
		LOG(DEBUG) << "[SERVER_EMU] Evolve_WinHttpConnect hook success\n";
		hr = SlimDetoursInlineHook(TRUE, &(PVOID&)Real_WinHttpOpenRequest, Evolve_WinHttpOpenRequest);
		if (FAILED(hr)) {
			LOG(ERROR) << "[SERVER_EMU] Evolve_WinHttpOpenRequest hook failed\n";
		}
		LOG(DEBUG) << "[SERVER_EMU] Evolve_WinHttpOpenRequest hook success\n";


		// EMU MODE PATCH
		if (ini.GetBoolValue("steam", "emu_steam", "false")) {
			// emu hook start
			dll_path = ini.GetValue("steam", "dll_path", "");
			if (dll_path != "") {
				hr = SlimDetoursInlineHook(TRUE, &(PVOID&)Real_GetEnvironmentVariableA, Evolve_GetEnvironmentVariableA);
				if (FAILED(hr)) {
					LOG(ERROR) << "[STEAM_EMU] Evolve_GetEnvironmentVariableA hook failed\n";
				}
				LOG(DEBUG) << "[STEAM_EMU] Evolve_GetEnvironmentVariableA hook success\n";
				hr = SlimDetoursInlineHook(TRUE, &(PVOID&)Real_RegQueryValueExA, Evolve_RegQueryValueExA);
				if (FAILED(hr)) {
					LOG(ERROR) << "[STEAM_EMU] Evolve_RegQueryValueExA hook failed\n";
				}
				LOG(DEBUG) << "[STEAM_EMU] Evolve_RegQueryValueExA hook success\n";
				emu_func = true;
			}
			else {
				MessageBox(
					NULL,
					L"不存在EMU DLL PATH，请检查配置文件。\n\n"
					L"EMU DLL PATH does not exist. Please check the configuration file.",
					L"错误 / Error",
					MB_ICONERROR | MB_OK
				);
				exit(0);
			}
		}
	}
}

void UninstallHooker() {
	HRESULT hr;
	hr = SlimDetoursInlineHook(FALSE, &(PVOID&)Real_getaddrinfo, Evolve_getaddrinfo);
	if (FAILED(hr)) {
		LOG(ERROR) << "[SERVER_EMU] Evolve_getaddrinfo hook uninstall failed\n";
	}
	hr = SlimDetoursInlineHook(FALSE, &(PVOID&)Real_WinHttpConnect, Evolve_WinHttpConnect);
	if (FAILED(hr)) {
		LOG(ERROR) << "[SERVER_EMU] Evolve_WinHttpConnect hook uninstall failed\n";
	}
	hr = SlimDetoursInlineHook(FALSE, &(PVOID&)Real_WinHttpOpenRequest, Evolve_WinHttpOpenRequest);
	if (FAILED(hr)) {
		LOG(ERROR) << "[SERVER_EMU] Evolve_WinHttpOpenRequest hook uninstall failed\n";
	}
	if (emu_func) {
		hr = SlimDetoursInlineHook(FALSE, &(PVOID&)Real_GetEnvironmentVariableA, Evolve_GetEnvironmentVariableA);
		if (FAILED(hr)) {
			LOG(ERROR) << "[STEAM_EMU] Evolve_GetEnvironmentVariableA hook uninstall failed\n";
		}
		hr = SlimDetoursInlineHook(FALSE, &(PVOID&)Real_RegQueryValueExA, Evolve_RegQueryValueExA);
		if (FAILED(hr)) {
			LOG(ERROR) << "[STEAM_EMU] Evolve_RegQueryValueExA hook uninstall failed\n";
		}
	}
	StopInternalServer();
}

// =====================
//         MAIN
// =====================
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		// Intercept the game launch and redirect to launcher if --no-launcher is not present in argv
		std::wstring cmdLine = GetCommandLineW();

		if (cmdLine.find(L"--no-launcher") == std::string::npos) {
			if (cmdLine[0] == L'"') {
				cmdLine.erase(0, 1);
				size_t pos = cmdLine.find(L'"');
				cmdLine.erase(0, pos + 1);
			}
			else {
				size_t pos = cmdLine.find(L' ');
				cmdLine.erase(0, pos);
			}

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			LPCWSTR launcherFilename = L"EvolveLauncher.exe";
			WCHAR launcherPath[MAX_PATH];

			GetFullPathName(launcherFilename, MAX_PATH, launcherPath, nullptr);

			std::wstring launcherCmd = launcherPath + cmdLine;

			size_t length = wcslen(launcherCmd.c_str()) + 1;
			LPWSTR launcherCmdLine = new WCHAR[length];
			wcscpy_s(launcherCmdLine, length, launcherCmd.c_str());

			// Check if launcher is present, else we just run normally
			if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(L"EvolveLauncher.exe")) {
				CreateProcess(
					nullptr,
					launcherCmdLine,
					nullptr,
					nullptr,
					FALSE,
					DETACHED_PROCESS,
					nullptr,
					nullptr,
					&si,
					&pi
				);


				//WaitForSingleObject(pi.hProcess, INFINITE);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				delete[] launcherCmdLine;

				exit(0);
			}
		}

		// Turn off IME
		ImmDisableIME(0);
		InitLogging();
		InitConfiguration();
		InitHooker();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		UninstallHooker();
	}
	return TRUE;
}
