#ifndef MEMORY_H
#define MEMORY_H

#include <windows.h>
#include <cstddef>

int ReadBuffer(HANDLE process, void* address, char* buffer, unsigned int size);
int WriteBuffer(HANDLE process, void* address, const char* buffer, unsigned int size);
char ReadChar(HANDLE process, void* address);
float ReadFloat(HANDLE process, void* address);
DWORD ReadInt(HANDLE process, void* address);
int WriteFloat(HANDLE process, void* address, float v);
DWORD ResolvePointerChain(HANDLE process, DWORD start_value, const DWORD* offsets, size_t count);

int MaskCompare(const unsigned char* data, const unsigned char* pattern, const char* mask);
long FindPatternInBuffer(const unsigned char* buffer, size_t buffer_size, const unsigned char* pattern, const char* mask);
DWORD FindPlayerBasePatternAddress(HANDLE process, DWORD module_base, DWORD module_size);

#endif