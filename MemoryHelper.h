#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <string>

class MemoryHelper {
public:
	DWORD GetModuleBaseAddress(TCHAR* modulename, DWORD pid) noexcept;

	HANDLE GetProcHandleFromName(const char* wndname, DWORD access) noexcept;
};