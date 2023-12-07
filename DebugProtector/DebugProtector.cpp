// DebugProtector - A most minimal Integrity/Anti-Tamper POC.
// 
// An example of a basic code integrity checker for Windows applications.
// The protector is a separate process that monitors the health of the protected process. 
// If the protected process is compromised, the protector will terminate it.
// 
// In this minimal example there is only 1 naive check: if a remote debugger is attached to the protected process, or if the protector fails to attach itself as a debugger the protector will terminate it.
// One better way to do it in reality would be for the SDK to set up a security baseline of the protected process on initiation, set up IPC and transfer the data to the protector process. 
// The protector process would then monitor the protected process for any changes to the security baseline.

#include <windows.h>
#include <iostream>
#include <string>

bool CheckProcessHealth(HANDLE hProcess);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: DebugProtector <PID>" << std::endl;
        return 1;
    }

    DWORD pid = std::stoul(argv[1]);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    std::cout << "Attached to process " << pid << ". Monitoring..." << std::endl;

    // Basic loop to keep the application running
    // In a real application, you would handle debug events here
    while (true) {
        // Health check every 1 second
        Sleep(1000);
        if (!CheckProcessHealth(hProcess)) {
			std::cout << "Process " << pid << " is not healthy. Terminating..." << std::endl;
            TerminateProcess(hProcess, 0);
			break;
        }
        else {
            std::cout << "Process " << pid << " is healthy." << std::endl;
        }
        if (!DebugActiveProcess(pid)) {
            std::cerr << "Failed to attach to process " << pid << std::endl;
            TerminateProcess(hProcess, 0);
            return 1;
        }
        DebugActiveProcessStop(pid);
    }

    return 0;
}

bool CheckProcessHealth(HANDLE hProcess) {
    // pretend health checks here
    // in reality you would check for code integrity, memory integrity, etc. 

    // check if remote debugger is attached to the target process
    if (CheckRemoteDebuggerPresent(hProcess, nullptr)) {
		std::cout << "Remote debugger detected!" << std::endl;
        Sleep(5); // give 5 second to show output
		return false;
	}

	return true;
}
