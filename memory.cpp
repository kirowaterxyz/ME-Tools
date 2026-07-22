#include "memory.h"
#include "globals.h"
#include <cstring>

int ReadBuffer(HANDLE process, void* address, char* buffer, unsigned int size) {
    SIZE_T read = 0;
    ReadProcessMemory(process, address, buffer, size, &read);
    return (int)read;
}

int WriteBuffer(HANDLE process, void* address, const char* buffer, unsigned int size) {
    SIZE_T written = 0;
    WriteProcessMemory(process, address, buffer, size, &written);
    return (int)written;
}

char ReadChar(HANDLE process, void* address) {
    char v = 0;
    ReadBuffer(process, address, &v, sizeof(v));
    return v;
}

float ReadFloat(HANDLE process, void* address) {
    float v = 0.0f;
    ReadBuffer(process, address, (char*)&v, sizeof(v));
    return v;
}

DWORD ReadInt(HANDLE process, void* address) {
    DWORD v = 0;
    ReadBuffer(process, address, (char*)&v, sizeof(v));
    return v;
}

int WriteFloat(HANDLE process, void* address, float v) {
    return WriteBuffer(process, address, (char*)&v, sizeof(v));
}

DWORD ResolvePointerChain(HANDLE process, DWORD start_value, const DWORD* offsets, size_t count) {
    DWORD current = start_value;
    for (size_t i = 0; i < count; i++) {
        DWORD next = 0;
        if (!current || !ReadBuffer(process, (void*)(current + offsets[i]), (char*)&next, sizeof(next))) {
            return 0;
        }
        current = next;
    }
    return current;
}

int MaskCompare(const unsigned char* data, const unsigned char* pattern, const char* mask) {
    for (; *mask; mask++, data++, pattern++) {
        if (*mask == 'x' && *data != *pattern) return 0;
    }
    return 1;
}

long FindPatternInBuffer(const unsigned char* buffer, size_t buffer_size, const unsigned char* pattern, const char* mask) {
    size_t pattern_len = strlen(mask);
    if (buffer_size < pattern_len) return -1;
    for (size_t i = 0; i <= buffer_size - pattern_len; i++) {
        if (MaskCompare(buffer + i, pattern, mask)) return (long)i;
    }
    return -1;
}

DWORD FindPlayerBasePatternAddress(HANDLE process, DWORD module_base, DWORD module_size) {
    unsigned char* buffer = (unsigned char*)malloc(module_size);
    if (!buffer) return 0;

    DWORD total_read = 0;
    while (total_read < module_size) {
        int chunk = ReadBuffer(process, (void*)(module_base + total_read),
            (char*)(buffer + total_read), module_size - total_read);
        if (chunk <= 0) break;
        total_read += (DWORD)chunk;
    }

    long offset = FindPatternInBuffer(buffer, total_read, PLAYER_BASE_PATTERN, PLAYER_BASE_MASK);
    free(buffer);

    if (offset < 0) return 0;
    return module_base + (DWORD)offset;
}