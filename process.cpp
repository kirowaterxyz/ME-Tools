#include "process.h"
#include "globals.h"
#include <cstring>

PROCESSENTRY32 GetProcessInfoByName(const wchar_t* exe_name) {
    PROCESSENTRY32 entry;
    memset(&entry, 0, sizeof(entry));
    entry.dwSize = sizeof(entry);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return { 0 };
    if (Process32First(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, exe_name) == 0) {
                CloseHandle(snapshot);
                return entry;
            }
        } while (Process32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return { 0 };
}

MODULEENTRY32 GetModuleInfoByName(DWORD process_id, const wchar_t* module_name) {
    MODULEENTRY32 entry;
    memset(&entry, 0, sizeof(entry));
    entry.dwSize = sizeof(entry);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
    if (snapshot == INVALID_HANDLE_VALUE) return { 0 };
    if (Module32First(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szModule, module_name) == 0) {
                CloseHandle(snapshot);
                return entry;
            }
        } while (Module32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return { 0 };
}

HWND GetGameWindow() {
    return FindWindowExA(0, 0, GAME_WINDOW_CLASS, 0);
}