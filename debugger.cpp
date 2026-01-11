
#include "debugger.h"
#include "linenoise.h"
#include "utils.h"

#include <iostream>
#include <sstream>
#include <sys/ptrace.h>

void debugger::run(){
    // wait until the child process has complete launching, then keep getting inputs from the user
    // until we get an EOF (Ctrl+d).

    int wait_status = 0;
    int options = 0;
    waitpid(m_pid, &wait_status, options);  // we wait for the 'SIGTRP' signal to be sent, which is a trace or
                                            // or a breakpoint trap. It is sent when the child process starts.

    char *line = nullptr;
    while ((line = linenoise("db> ")) != nullptr)
    {
        handle_command(line);
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }

}

void debugger::handle_command(const std::string &line) {
    std::vector<std::string> args = split(line, ' ');

    std::string command = args[0];

    if (is_prefix(command, "continue"))
    {
        continue_execution();
    } else
    {
        std::cerr << "Unknow command!!!\n";
    }

}

void debugger::continue_execution() {
    ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);

    int wait_status;
    int options = 0;
    waitpid(m_pid, &wait_status, options);
}