#include "input.h"
#include <cstdlib>
#include <ctime>

void KeyDown(WORD vk) {
    INPUT in = {};
    in.type = INPUT_KEYBOARD;
    in.ki.wVk = vk;
    SendInput(1, &in, sizeof(INPUT));
}

void KeyUp(WORD vk) {
    INPUT in = {};
    in.type = INPUT_KEYBOARD;
    in.ki.wVk = vk;
    in.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &in, sizeof(INPUT));
}

void MouseDown(WORD vk) {
    if (vk != VK_LBUTTON && vk != VK_RBUTTON) return;
    INPUT in = {};
    in.type = INPUT_MOUSE;
    in.mi.dwFlags = (vk == VK_LBUTTON) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &in, sizeof(INPUT));
}

void MouseUp(WORD vk) {
    if (vk != VK_LBUTTON && vk != VK_RBUTTON) return;
    INPUT in = {};
    in.type = INPUT_MOUSE;
    in.mi.dwFlags = (vk == VK_LBUTTON) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
    SendInput(1, &in, sizeof(INPUT));
}

void MouseScrollDown() {
    INPUT in = {};
    in.type = INPUT_MOUSE;
    in.mi.dwFlags = MOUSEEVENTF_WHEEL;
    in.mi.mouseData = (DWORD)-WHEEL_DELTA;
    SendInput(1, &in, sizeof(INPUT));
}

void HandleKeyup(int key) {
    std::srand((unsigned)std::time(0));
    int sleepTime = 50 + std::rand() % 100;
    Sleep(sleepTime);
    MouseUp((WORD)key);
    KeyUp((WORD)key);
}