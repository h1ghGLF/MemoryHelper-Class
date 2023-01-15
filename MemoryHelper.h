#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

class MemoryHelper {
public:
	DWORD GetModuleBaseAddress(TCHAR* moduleName, DWORD pId) noexcept {
		DWORD dwModuleBaseAddress = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pId);
		MODULEENTRY32 ModuleEntry32 = { 0 };
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnapshot, &ModuleEntry32)) {
			do {
				if (_tcscmp(ModuleEntry32.szModule, moduleName) == 0) {
					dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &ModuleEntry32));
		}
		CloseHandle(hSnapshot);

		return dwModuleBaseAddress;
	}

	HANDLE GetProcHandleFromName(const char* wndName, DWORD desiredAccess) noexcept {
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof(pEntry);
		BOOL hRes = Process32First(hSnapShot, &pEntry);
		while (hRes) {
			if (strcmp(pEntry.szExeFile, wndName) == 0) {
				HANDLE hProcess = OpenProcess(desiredAccess, 0, (DWORD)pEntry.th32ProcessID);
				if (hProcess != NULL)
					return hProcess;

				else
					return NULL;
			}
			hRes = Process32Next(hSnapShot, &pEntry);
		}
		CloseHandle(hSnapShot);
	}

	template <typename T>
	T ReadMemory(uintptr_t uptrAddress, HANDLE hProcess) noexcept {
		T Value;
		ReadProcessMemory(hProcess, (LPCVOID)uptrAddress, &Value, sizeof(T), NULL);
		return Value;
	}

	template <typename T>
	T WriteMemory(uintptr_t uptrAddress, HANDLE hProcess, T Value) noexcept {
		return WriteProcessMemory(hProcess, (LPVOID)uptrAddress, &Value, sizeof(T), NULL);
	}
};