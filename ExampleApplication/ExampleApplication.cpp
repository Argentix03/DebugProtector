// ExampleApplication
// This is an example application that uses the DebugProtector

#include "DebugProtector.h"
#include <iostream>
#include <windows.h>

int main()
{
    // Get our own pid to provide to DebugProtector
    DWORD protected_pid = GetCurrentProcessId();

	// Start the debug protector and get its PID
	DWORD protector_pid = StartDebugProtector(protected_pid);
	if (protector_pid == 0) {
		std::cerr << "Failed to start DebugProtector" << std::endl;
		return 1;
	}
	std::cout << "DebugProtector started with PID " << protector_pid << std::endl;

    // Main loop, this is our main application loop.
	// One thing we add to our loop is a check that debug protector is running.
	// If it's not running, we terminate our application.
	// Since we are using the SDK, we can use the IsDebugProtectorRunning function.
	// Since we know nothing about the internals of it, we can only assume that as long as the DebugProtector is running, our application is safe.
	// The SDK will not provide much for us to use in order to protect DebugProtector itself, since its THEIR JOB to protect us! we trust them to protect themselves.
	// If its running but not doing whats its suppose to do, for example running infinite loop of junk code, or is suspended, we are pretty much hopeless.
    while (true) {

		// health check. make sure we our protected!
		std::cout << "Checking code integrity...\n";
		if (!IsDebugProtectorRunning(protector_pid)) {
			std::cout << "DebugProtector is not running. Terminating..." << std::endl;
			exit(1);
		}

		// now that we know we safe our application code can run...
		std::cout << "Hello World!\n";
		Sleep(1000);

		// more application code...
	}

	return 0;
}

