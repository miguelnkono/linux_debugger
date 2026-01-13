
#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include <cinttypes>
#include <linux/types.h>
#include <string>
#include <sys/wait.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include "breakpoint.h"

class debugger {
private:
  std::string m_program_name;
  pid_t m_pid;

  // data structure to hold our breakpoints
  std::unordered_map<std::intptr_t, breakpoint> m_breakpoints;

  void handle_command(const std::string &line);
  void continue_execution();

  uint64_t read_memory(uint64_t addr) {
    return ptrace(PTRACE_PEEKDATA, m_pid, addr, nullptr);
  }

  void write_memory(uint64_t addr, uint64_t value) {
    ptrace(PTRACE_POKEDATA, m_pid, addr, value);
  }

  uint64_t get_pc();
  void set_pc(uint64_t pc);
  void wait_for_signal();

public:
  // debugger(std::string program_name, pid_t pid) :
  // m_program_name{std::move(m_program_name)}, m_pid{pid} {}
  debugger(std::string &program_name, pid_t pid) {
    this->m_program_name = program_name;
    this->m_pid = pid;
  };

  void run();

  void set_breakpoint_at_address(std::intptr_t addr);

  void dump_registers();

  void step_over_breakpoint();
};

#endif // __DEBUGGER_H__
