#include "arch/amd64/arch.hpp"
#include "kutils.hpp"
#include <cstdint>

namespace arch {

#define IA32_APIC_BASE_MSR 0x1B

std::uintptr_t LAPICBase = 0;

bool checkAPICSupport() {
  std::uint32_t eax, ebx, ecx, edx;

  asm volatile("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(1));

  return (edx & (1 << 9)) != 0;
}

bool initAPIC() {
  disablePIC();

  if (!checkAPICSupport()) {
    return false;
  }

  // turn on lapic
  std::uint64_t msrval = readmsr(IA32_APIC_BASE_MSR);
  msrval |= (1 << 11);
  writermsr(IA32_APIC_BASE_MSR, msrval);

  // find lapic base
  LAPICBase = (msrval & 0xfffff000) + kernelMappingOffset;

  kout << HEX(LAPICBase);

  std::uint32_t *helperPtr = (std::uint32_t *)(LAPICBase + 0x20);
  kout << "APIC ID: " << (std::uint64_t)(*helperPtr >> 24) << "\n";

  // helperPtr = (std::uint32_t *)((char *)LAPICBase + 0x30);
  // kout << "APIC version: " << *helperPtr << '\n';

  return true;
}

} // namespace arch