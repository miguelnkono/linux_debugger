
#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include <sys/wait.h>
#include <string>
#include <vector>
#include <utility>
#include <linux/types.h>

class debugger
{
private:
    std::string m_program_name;
    pid_t m_pid;

    void handle_command(const std::string &line);
    void continue_execution();
public:
    // debugger(std::string program_name, pid_t pid) : m_program_name{std::move(m_program_name)}, m_pid{pid} {}
    debugger(std::string &program_name, pid_t pid) {
        this->m_program_name = program_name;
        this->m_pid = pid;
    };

    void run();
};


#endif // __DEBUGGER_H__