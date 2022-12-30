#include "MemoryHelper.h"

DWORD MemoryHelper::GetModuleBaseAddress(TCHAR* modulename, DWORD pid) noexcept {
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32)) {
		do {
			if (_tcscmp(ModuleEntry32.szModule, modulename) == 0) {
				dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} 
		while (Module32Next(hSnapshot, &ModuleEntry32));
	}
	CloseHandle(hSnapshot);

	return dwModuleBaseAddress;
}

HANDLE MemoryHelper::GetProcHandleFromName(const char* wndname, DWORD access) noexcept {
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes) {
		if (strcmp(pEntry.szExeFile, wndname) == 0) {
			HANDLE hProcess = OpenProcess(access, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
				return hProcess;

			else
				return NULL;
		}
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}