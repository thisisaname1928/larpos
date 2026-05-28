#pragma once

#include <cstdint>
namespace AMD64 {

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

} // namespace AMD64

extern "C" {
extern void loadGDTR64(AMD64::GDTR *gdtr);
extern void reloadSegment64(std::uint16_t CS, std::uint16_t DS);
extern void loadIDT64(AMD64::IDTR *IDTR);
extern std::uint64_t isrFuncTab[32];
}