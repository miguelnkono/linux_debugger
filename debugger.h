
#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include <sys/wait.h>
#include <string>
#include <vector>
#include <utility>
#include <linux/types.h>
#include <unordered_map>
#include <cinttypes>

#include "breakpoint.h"

class debugger
{
private:
    std::string m_program_name;
    pid_t m_pid;

    // data structure to hold our breakpoints
    std::unordered_map<std::intptr_t, breakpoint> m_breakpoints;

    void handle_command(const std::string &line);
    void continue_execution();
public:
    // debugger(std::string program_name, pid_t pid) : m_program_name{std::move(m_program_name)}, m_pid{pid} {}
    debugger(std::string &program_name, pid_t pid) {
        this->m_program_name = program_name;
        this->m_pid = pid;
    };

    void run();

    void set_breakpoint_at_address(std::intptr_t addr);
};


#endif // __DEBUGGER_H__