
#include "breakpoint.h"

void breakpoint::enable() {

    long data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
    m_saved_data = static_cast<uint8_t>(data & 0xff); // saved bottom byte

    uint64_t int_3 = 0xcc;   // the int 3 instruction
    uint64_t data_with_int3 = ((data & ~0xff) | int_3); // set the bottom byte

    ptrace(PTRACE_POKEDATA, m_pid, m_addr, data_with_int3);
    m_enabled = true;
}

void breakpoint::disable() {

    long data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
    uint64_t restored_data = ((data & ~0xff) | m_saved_data);

    ptrace(PTRACE_POKEDATA, m_pid, m_addr, restored_data);
    m_enabled = false;
}
