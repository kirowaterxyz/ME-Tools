#include "globals.h"

const unsigned char PLAYER_BASE_PATTERN[] = {
    0x89, 0x0D, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0x00, 0x00, 0x00, 0x00,
    0xFF
};
const char PLAYER_BASE_MASK[] = "xx????x????x";
const DWORD PLAYER_CHAIN_OFFSETS[] = { 0xCC, 0x4A4, 0x214 };

std::atomic<bool> running(true);
std::atomic<bool> kg(false);
std::atomic<bool> strang(false);
std::atomic<bool> beamer(false);
std::atomic<bool> sidestepbeamer(false);

int kickglitch_attack_key = VK_SPACE;
int strang_jump_key = VK_LBUTTON;
int beamer_quick_turn_key = 'Q';
int sidestepbeamer_jump_key = VK_LBUTTON;