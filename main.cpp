
#include <sys/personality.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#include "debugger.h"

int main(int argc, char **argv) {
    if (argc < 2)
    {
        std::cerr << "Error. Usage: linux_debugger <program_to_debug>\n";
        return -1;
    }

    std::string program = argv[1];

    pid_t pid = fork();
    if (pid == 0)
    {
        personality(ADDR_NO_RANDOMIZE);

        // we're in the child process
        if (ptrace(PTRACE_TRACEME, 0, 0, 0))
        {
            std::cerr << "Error in ptrace\n";
        }

        execl(program.c_str(), program.c_str(), nullptr);
    } else if (pid >= 1)
    {
        // we're in the parent process
        std::cout << "Started debugging the child process with the id of : " << pid << "\n";
        debugger debugger {program, pid};
        debugger.run();
    }

    return 0;
}
