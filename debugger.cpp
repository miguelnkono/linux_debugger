
#include "debugger.h"
#include "linenoise.h"
#include "register.h"
#include "utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/ptrace.h>

void debugger::run() {
  // wait until the child process has complete launching, then keep getting
  // inputs from the user until we get an EOF (Ctrl+d).

  int wait_status = 0;
  int options = 0;
  waitpid(m_pid, &wait_status,
          options); // we wait for the 'SIGTRAP' signal to be sent, which is a
                    // trace or or a breakpoint trap. It is sent when the child
                    // process starts.

  char *line = nullptr;
  while ((line = linenoise("db> ")) != nullptr) {
    handle_command(line);
    linenoiseHistoryAdd(line);
    linenoiseFree(line);
  }
}

void debugger::handle_command(const std::string &line) {
  std::vector<std::string> args = split(line, ' ');

  std::string command = args[0];

  if (is_prefix(command, "continue")) {
    continue_execution();
  } else if (is_prefix(command, "break")) {
    std::string addr{args[1], 2}; // assume that the user typed 0xADRRESS, so we
                                  // remove the first two characters.
    set_breakpoint_at_address(std::stol(addr, 0, 16));
  } else if (is_prefix(command, "register")) {
    if (is_prefix(args[1], "dump"))
      dump_registers();
    else if (is_prefix(args[1], "read"))
      std::cout << get_register_value(m_pid, get_register_from_name(args[2]))
                << "\n";
    else if (is_prefix(args[1], "write")) {
      std::string val{args[3], 2};
      set_register_value(m_pid, get_register_from_name(args[2]),
                        std::stol(val));
    }
  } else if (is_prefix(command, "memory")) {
    std::string address{args[2], 2};

    if (is_prefix(args[1], "read"))
      std::cout << std::hex << read_memory(std::stol(address, 0, 16))
                << std::endl;
    if (is_prefix(args[1], "write")) {
      std::string val{args[3], 2};
      write_memory(std::stol(address, 0, 16), std::stol(val, 0, 16));
    }
  } else {
    std::cerr << "Unknow command!!!\n";
  }
}

void debugger::continue_execution() {
  step_over_breakpoint();

  ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);

  wait_for_signal();
}

void debugger::set_breakpoint_at_address(std::intptr_t addr) {
  std::cout << "set breakpoint at address " << std::hex << addr << "\n";

  breakpoint bp{m_pid, addr};
  bp.enable();

  // m_breakpoints[addr] = bp;
  m_breakpoints.insert({m_pid, bp});
}

void debugger::dump_registers() {
  for (const reg_descriptor &rd : g_register_descriptors) {
    std::cout << rd.name << " 0x" << std::setfill('0') << std::setw(16)
              << std::hex << get_register_value(m_pid, rd.r) << std::endl;
  }
}

uint64_t debugger::get_pc() { return get_register_value(m_pid, reg::rip); }

void debugger::set_pc(uint64_t pc) { set_register_value(m_pid, reg::rip, pc); }

void debugger::step_over_breakpoint() {

  uint64_t possible_breakpoint_location = get_pc() - 1;

  if (m_breakpoints.count(possible_breakpoint_location)) {
    breakpoint &bp = m_breakpoints[possible_breakpoint_location];

    if (bp.is_enable()) {
      uint64_t previous_instruction_address = possible_breakpoint_location;
      set_pc(previous_instruction_address);

      bp.disable();
      ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
      wait_for_signal();
      bp.enable();
    }
  }
}

void debugger::wait_for_signal() {

  int wait_status;
  int options = 0;
  waitpid(m_pid, &wait_status, options);
}
