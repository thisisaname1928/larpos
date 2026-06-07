#include <cstdint>

inline std::uint64_t readcr3(void) {
  std::uint64_t cr3Val;
  asm volatile("mov %%cr3, %0" : "=r"(cr3Val)::"memory");
  return cr3Val;
}

// bool isMemoryMapped(std::uint64_t address) { uint64_t cr3 = readcr3(); }