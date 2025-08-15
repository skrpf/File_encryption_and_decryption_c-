#include <iostream>
#include <string>
#include <vector>
#include "ProcessManagement.hpp"

// Use preprocessor directives to include the correct headers for the OS
#ifdef _WIN32
    #include <windows.h> // For Windows API (CreateProcess, etc.)
#else
    #include <unistd.h>      // For fork, execv
    #include <sys/wait.h>    // For waitpid
    #include <cstring>       // For strdup
#endif

// A helper function to contain the platform-specific process creation logic
void executeCryption(const std::string& taskString) {
#ifdef _WIN32
    // --- WINDOWS IMPLEMENTATION ---

    // On Windows, executables usually have a .exe extension.
    // The command line is the program name followed by its arguments.
    std::string command = "cryption.exe " + taskString;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Zero out the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create a mutable C-style string for the CreateProcess function
    std::vector<char> cmd(command.begin(), command.end());
    cmd.push_back('\0');

    // Start the child process.
    if (!CreateProcess(NULL,    // Use command line
        cmd.data(),             // Command to execute
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        0,                      // No creation flags
        NULL,                   // Use parent's environment block
        NULL,                   // Use parent's starting directory
        &si,                    // Pointer to STARTUPINFO structure
        &pi)                    // Pointer to PROCESS_INFORMATION structure
        )
    {
        std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
        return;
    }

    // Wait until the child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Clean up handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

#else
    // --- LINUX / macOS IMPLEMENTATION (your original code) ---
    
    pid_t childProcessToRun = fork();
    if (childProcessToRun == 0) {
        // This is the child process
        char* args[3];
        // Note: strdup allocates memory which should be freed, but execv replaces
        // the process memory, so it's okay in this specific case.
        args[0] = strdup("./cryption");
        args[1] = strdup(taskString.c_str());
        args[2] = nullptr;
        
        execv("./cryption", args);
        
        // If execv returns, it means an error occurred
        std::cerr << "Error executing cryption: " << strerror(errno) << std::endl;
        exit(1);
    }
    else if (childProcessToRun > 0) {
        // This is the parent process
        int status;
        waitpid(childProcessToRun, &status, 0); // Wait for the child to complete
    }
    else {
        // Fork failed
        std::cerr << "Fork failed" << std::endl;
        exit(1);
    }
#endif
}

// --- Your existing class methods (no changes needed here) ---

ProcessManagement::ProcessManagement() {}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    taskQueue.push(std::move(task));
    return true;
}

void ProcessManagement::executeTasks() {
    while (!taskQueue.empty()) {
        std::unique_ptr<Task> taskToExecute = std::move(taskQueue.front());
        taskQueue.pop();
        std::cout << "Executing task: " << taskToExecute->toString() << std::endl;
        
        // This single function call now works on any OS
        executeCryption(taskToExecute->toString());
    }
}