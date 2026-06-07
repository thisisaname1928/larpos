#pragma once

#include <cstdint>
namespace arch {

typedef struct __attribute__((__packed__)) {
  std::uint16_t size;
  std::uint64_t offset;
} GDTR;

#define IDT_INTERRUPT_GATE 0xe
#define IDT_TRAP_GATE 0xf
#define IDT_DPL(x) (((x) & 3) << 5)
#define IDT_PRESENT (1 << 7)

typedef struct __attribute__((__packed__)) {
  std::uint16_t offset0;
  std::uint16_t CS;
  std::uint8_t IST;
  std::uint8_t attribute;
  std::uint16_t offset1;
  std::uint32_t offset2;
  std::uint32_t reserved;
} IDTEntry;

typedef struct __attribute__((__packed__)) {
  std::uint16_t size;
  std::uint64_t offset;
} IDTR;

extern IDTEntry kernelIDT[256];
extern std::uint64_t kernelGDT[];
extern GDTR kernelGDTR;
extern IDTR kernelIDTR;
extern std::uint16_t CS, DS;

void halt();
void init();
void setIDT(int th, uint64_t offset, uint8_t IST, uint8_t attribute);
extern "C" void exceptionHandler(std::uint64_t errCode, void *stackStart);

inline void outb(std::uint16_t port, std::uint8_t val) {
  asm volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline void outw(std::uint16_t port, std::uint16_t val) {
  asm volatile("outw %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline void outl(std::uint16_t port, std::uint32_t val) {
  asm volatile("outl %0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline std::uint8_t inb(std::uint16_t port) {
  uint16_t retc;
  asm volatile("inb %w1, %b0" : "=a"(retc) : "Nd"(port));

  return retc;
}

inline std::uint16_t inw(std::uint16_t port) {
  uint16_t retc;
  asm volatile("inw %w1, %w0" : "=a"(retc) : "Nd"(port));

  return retc;
}

inline std::uint32_t inl(std::uint16_t port) {
  uint16_t retc;
  asm volatile("inl %w1, %0" : "=a"(retc) : "Nd"(port));

  return retc;
}

inline void ioWait() { asm volatile("outb %%al, $0x80" : : "a"(0)); }

inline uint64_t readmsr(uint32_t msr) {
  uint32_t low, high;
  asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
  return ((uint64_t)high << 32) | low;
}

inline void writermsr(uint64_t msr, uint64_t value) {
  uint32_t low = value & 0xFFFFFFFF;
  uint32_t high = value >> 32;
  asm volatile("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

bool checkAPICSupport();
void disablePIC();
bool initAPIC();

} // namespace arch

extern "C" {
extern void loadGDTR64(arch::GDTR *gdtr);
extern void reloadSegment64(std::uint16_t CS, std::uint16_t DS);
extern void loadIDT64(arch::IDTR *IDTR);
extern std::uint64_t isrFuncTab[32];
}