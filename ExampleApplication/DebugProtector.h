#pragma once
#include <windows.h>
DWORD StartDebugProtector(DWORD pid);
bool IsDebugProtectorRunning(DWORD pid);