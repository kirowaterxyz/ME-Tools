#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>
#include <tlhelp32.h>

PROCESSENTRY32 GetProcessInfoByName(const wchar_t* exe_name);
MODULEENTRY32 GetModuleInfoByName(DWORD process_id, const wchar_t* module_name);
HWND GetGameWindow();

#endif