
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
    waitpid(m_pid, &wait_status, options);  // we wait for the 'SIGTRAP' signal to be sent, which is a trace or
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
    } else if (is_prefix(command, "break"))
    {
        std::string addr { args[1], 2 }; // assume that the user typed 0xADRRESS, so we remove the first
                                        // the first two characters.
        set_breakpoint_at_address(std::stol(addr, 0, 16));
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

void debugger::set_breakpoint_at_address(std::intptr_t addr) {
    std::cout << "set breakpoint at address " << std::hex << addr << "\n";

    breakpoint bp { m_pid, addr };
    bp.enable();

    // m_breakpoints[addr] = bp;
    m_breakpoints.insert({ m_pid, bp });
}