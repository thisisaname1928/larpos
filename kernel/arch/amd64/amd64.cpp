#include "amd64.hpp"
#include "kutils.hpp"
#include <cstdint>

namespace AMD64 {
std::uint16_t CS, DS;

void doNothing() {}

void init() {

  // load new gdt
  CS = 0x8;
  DS = 0x10;
  loadGDTR64(&kernelGDTR);
  reloadSegment64(CS, DS);

  // init all 256 idt entries
  for (int i = 0; i < 256; i++) {
    setIDT(i, (uint64_t)&doNothing, 0, 0 | IDT_DPL(0) | IDT_INTERRUPT_GATE);
  }

  // setup first 32 idt entries
  for (int i = 0; i < 32; i++)
    setIDT(i, isrFuncTab[i], 0, IDT_PRESENT | IDT_DPL(0) | IDT_TRAP_GATE);

  loadIDT64(&kernelIDTR);
}

void halt() {
  asm("hlt");
  while (true) {
  }
}

} // namespace AMD64