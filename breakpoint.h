
#ifndef __BREAKPOINT_H__
#define __BREAKPOINT_H__

#include <sys/wait.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <cstdint>

class breakpoint
{
private:
    pid_t m_pid;
    std::intptr_t m_addr;
    bool m_enabled;
    uint8_t m_saved_data;   // data which used to be at the breakpoint addres.

public:
    breakpoint(pid_t pid, std::intptr_t addr) {
        this->m_pid = pid;
        this->m_addr = addr;
        this->m_enabled = false;
        this->m_saved_data = {};
    }

    void enable();
    void disable();

    auto is_enable() const -> bool{ return m_enabled; }
    auto get_address() const -> std::intptr_t { return m_addr; }
};

#endif // __BREAKPOINT_H__
