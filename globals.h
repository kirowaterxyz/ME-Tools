#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <atomic>

#define TARGET_PROCESS_NAME   L"mirrorsedge.exe"
#define GAME_WINDOW_CLASS     "LaunchUnrealUWindowsClient"

#define PLAYER_STATE          0x68
#define PLAYER_HEALTH         0x2B8
#define PLAYER_VX             0x100
#define PLAYER_VY             0x104
#define PLAYER_MAX_GROUND_SPEED 0x264

extern const unsigned char PLAYER_BASE_PATTERN[];
extern const char PLAYER_BASE_MASK[];
extern const DWORD PLAYER_CHAIN_OFFSETS[];

extern std::atomic<bool> running;
extern std::atomic<bool> kg;
extern std::atomic<bool> strang;
extern std::atomic<bool> beamer;
extern std::atomic<bool> sidestepbeamer;
extern std::atomic<bool> strangArmed;

extern int kickglitch_attack_key;
extern int strang_jump_key;
extern int beamer_quick_turn_key;
extern int sidestepbeamer_jump_key;

#endif