
#include "register.h"

#include <algorithm>
#include <sys/ptrace.h>
#include <sys/user.h>

uint64_t get_register_value(pid_t pid, reg r) {
  user_regs_struct regs;
  ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

  // we search for the index of the register requested in the regs structure.
  const reg_descriptor *it =
      std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                   [r](reg_descriptor &&rd) { return rd.r == r; });

  return *(reinterpret_cast<uint64_t *>(&regs) +
           (it - begin(g_register_descriptors)));
}

void set_register_value(pid_t pid, reg r, uint64_t value) {
  user_regs_struct regs;
  ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

  // we search for the index of the register requested in the regs structure.
  const reg_descriptor *it =
      std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                   [r](reg_descriptor &&rd) { return rd.r == r; });

  *(reinterpret_cast<uint64_t *>(&regs) +
    (it - begin(g_register_descriptors))) = value;
  ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
}

uint64_t get_register_value_from_dwart_register(pid_t pid, unsigned regnum) {

  const reg_descriptor *it = std::find_if(
      begin(g_register_descriptors), end(g_register_descriptors),
      [regnum](reg_descriptor &&rd) -> bool { return rd.dwarf_r == regnum; });

  // if we didn't find the register by its dwarf (find_if returned the last the
  // element of g_register_descriptors)
  if (it == end(g_register_descriptors)) {
    throw std::out_of_range{"Unkown dwarf register!"};
  }

  return get_register_value(pid, it->r);
}

std::string get_register_name(reg r) {
  const reg_descriptor *it =
      std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                   [r](reg_descriptor &&rd) -> bool { return rd.r == r; });
  return it->name;
}

reg get_register_from_name(const std::string &name) {
  const reg_descriptor *it = std::find_if(
      begin(g_register_descriptors), end(g_register_descriptors),
      [name](reg_descriptor &&rd) -> bool { return rd.name == name; });

  return it->r;
}
