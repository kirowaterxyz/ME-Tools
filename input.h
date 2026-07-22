#ifndef INPUT_H
#define INPUT_H

#include <windows.h>

void KeyDown(WORD vk);
void KeyUp(WORD vk);
void MouseDown(WORD vk);
void MouseUp(WORD vk);
void MouseScrollDown();
void HandleKeyup(int key);

#endif