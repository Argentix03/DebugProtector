// DebugProtectorSDK.cpp
// This is the SDK for the DebugProtector - i was lazy to use a dll.
// 
// We dont want to burden the user, we want them to trust us to protect them so it includes only 2 functions for simplicity:
// StartDebugProtector - starts the DebugProtector and returns its PID.
// IsDebugProtectorRunning - checks if the DebugProtector is still running.
// 
// Remember: In reality there should be a good balance between:
// - educating the user and giving them more tools to integrate the DebugProtector deeper into their application.
// - keeping the SDK simple and easy to use, while letting them think we can take care of everything.
#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>

// Simply starts the DebugProtector and returns its PID, providing our own PID as an argument.
// In reality, the SDK should provide a way to pass a security baseline to the protector process.
DWORD StartDebugProtector(DWORD pid) {
    std::string command = "DebugProtector.exe " + std::to_string(pid);
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process
    if (!CreateProcessA(
        NULL,               // No module name (use command line)
        const_cast<char*>(command.c_str()), // Command line
        NULL,               // Process handle not inheritable
        NULL,               // Thread handle not inheritable
        FALSE,              // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE, // No creation flags
        NULL,               // Use parent's environment block
        NULL,               // Use parent's starting directory 
        &si,                // Pointer to STARTUPINFO structure
        &pi)                // Pointer to PROCESS_INFORMATION structure
        ) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")" << std::endl;
        return false;
    }

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::cout << "Started DebugProtector with PID " << pi.dwProcessId << std::endl;

    return pi.dwProcessId;
}

// Function to check if a process is still running
bool IsDebugProtectorRunning(DWORD pid) {
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (process == NULL) {
        return false; // Could not open the process, maybe it's not running
    }

    DWORD exitCode;
    if (!GetExitCodeProcess(process, &exitCode)) {
        CloseHandle(process);
        return false; // Failed to get exit code, assume process is not running
    }

    CloseHandle(process);
    return (exitCode == STILL_ACTIVE); // If STILL_ACTIVE, the process is running
}
