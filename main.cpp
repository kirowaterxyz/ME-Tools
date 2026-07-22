#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdio>
#include <thread>
#include <mmsystem.h>

#include "globals.h"
#include "memory.h"
#include "process.h"
#include "input.h"

#pragma comment(lib, "winmm.lib")

static void Tools(HANDLE process, DWORD static_address) {
    while (running) {
        DWORD object_this = static_address ? ReadInt(process, (void*)static_address) : 0;
        DWORD player_base = 0;
        if (object_this) {
            player_base = ResolvePointerChain(process, object_this, PLAYER_CHAIN_OFFSETS, 3);
        }

        bool focused = (GetForegroundWindow() == GetGameWindow());

        static bool kg = false;
        static bool kgWasDown = false;
        bool kgDown = (GetAsyncKeyState('C') & 0x8000) != 0;
        if (kgDown && !kgWasDown && focused) {
            kg = !kg;
        }
        kgWasDown = kgDown;

        if (kg && player_base) {
            char state = ReadChar(process, (void*)(player_base + PLAYER_STATE));
            if (state == 1) {
                std::thread(MouseScrollDown).detach();
                kg = false;
            }
            else if (state == 12) {
                MouseDown((WORD)kickglitch_attack_key);
                KeyDown((WORD)kickglitch_attack_key);
                std::thread(HandleKeyup, kickglitch_attack_key).detach();
            }
        }

        if (focused && (GetAsyncKeyState('V') & 0x8000) && player_base) {
            WriteFloat(process, (void*)(player_base + PLAYER_MAX_GROUND_SPEED), 2147483647.0f);
            char state = ReadChar(process, (void*)(player_base + PLAYER_STATE));
            if (state == 1) {
                KeyDown((WORD)strang_jump_key);
                MouseDown((WORD)strang_jump_key);
                std::thread(HandleKeyup, strang_jump_key).detach();
                WriteFloat(process, (void*)(player_base + PLAYER_MAX_GROUND_SPEED), 720.0f);
            }
        }

        static bool beamer = false;
        if (focused && (GetAsyncKeyState('B') & 0x8000) && player_base) {
            if (!beamer) {
                float vx = ReadFloat(process, (void*)(player_base + PLAYER_VX));
                float vy = ReadFloat(process, (void*)(player_base + PLAYER_VY));
                if (sqrtf(vx * vx + vy * vy) > 1000.0f) {
                    SetForegroundWindow(GetGameWindow());
                    KeyDown((WORD)beamer_quick_turn_key);
                    MouseDown((WORD)beamer_quick_turn_key);
                    std::thread(HandleKeyup, beamer_quick_turn_key).detach();
                    beamer = true;
                }
            }
        }
        else {
            beamer = false;
        }

        static bool sidestepbeamer = false;
        if (focused && (GetAsyncKeyState('N') & 0x8000) && player_base) {
            if (!sidestepbeamer) {
                float vx = ReadFloat(process, (void*)(player_base + PLAYER_VX));
                float vy = ReadFloat(process, (void*)(player_base + PLAYER_VY));
                if (sqrtf(vx * vx + vy * vy) > 1000.0f) {
                    SetForegroundWindow(GetGameWindow());
                    KeyDown((WORD)sidestepbeamer_jump_key);
                    MouseDown((WORD)sidestepbeamer_jump_key);
                    std::thread(HandleKeyup, sidestepbeamer_jump_key).detach();
                    sidestepbeamer = true;
                }
            }
        }
        else {
            sidestepbeamer = false;
        }

        if (player_base) {
            char health = ReadChar(process, (void*)(player_base + PLAYER_HEALTH));
            if (health == 0) {
                beamer  = false;
                sidestepbeamer = false;
                kg = false;
                strang = false;
            }
        }
        Sleep(1);
    }
}

int main() {
    timeBeginPeriod(1);

    Beep(523, 500);

    srand(GetTickCount());
    char title[33] = { 0 };
    const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < 32; i++) {
        title[i] = chars[rand() % (sizeof(chars) - 1)];
    }
    SetConsoleTitleA(title);

    system("color b");

	printf("this is a free tool for educational purposes only.\n");
    printf("made by github.com/kirowaterxyz/me-tools \n\n");

    printf("waiting for %ws...", TARGET_PROCESS_NAME);

    PROCESSENTRY32 proc = {};
    while (running) {
        proc = GetProcessInfoByName(TARGET_PROCESS_NAME);
        if (proc.th32ProcessID) break;
        Sleep(500);
    }

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc.th32ProcessID);
    if (!process) {
        printf("failed to open process (try running as admin).\n");
        return 1;
    }

    MODULEENTRY32 mod = GetModuleInfoByName(proc.th32ProcessID, TARGET_PROCESS_NAME);
    if (!mod.hModule) {
        printf("failed to resolve module base.\n");
        return 1;
    }

    DWORD module_base = (DWORD)mod.modBaseAddr;
    DWORD module_size = mod.modBaseSize;
	printf(" success\n");
    printf("module base: 0x%08X (size 0x%X)\n", module_base, module_size);

    printf("looking for player base signature...");
    DWORD pattern_addr = FindPlayerBasePatternAddress(process, module_base, module_size);
    if (!pattern_addr) {
        printf("signature not found.\n");
        CloseHandle(process);
        return 1;
    }

    DWORD static_address = ReadInt(process, (void*)(pattern_addr + 2));
    printf(" success\n");
    printf("pattern found at 0x%08X, static address = 0x%08X\n", pattern_addr, static_address);

    std::thread poll(Tools, process, static_address);

    printf("\npress enter to quit.\n");
    getchar();
    running = false;

    poll.join();
    CloseHandle(process);
    timeEndPeriod(1);
    return 0;
}