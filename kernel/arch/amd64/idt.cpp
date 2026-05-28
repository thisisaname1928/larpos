#include "amd64.hpp"
#include <cstdint>
#include <kutils.hpp>

namespace AMD64 {
__attribute__((aligned(16))) IDTEntry kernelIDT[256];

IDTR kernelIDTR = {.size = sizeof(IDTEntry), .offset = (uint64_t)&kernelIDT};

void setIDT(int th, uint64_t offset, uint8_t IST, uint8_t attribute) {
  asm("cli");

  if (th < 256) {
    kernelIDT[th].offset0 = offset & 0xffff;
    kernelIDT[th].offset1 = (offset >> 16) & 0xffff;
    kernelIDT[th].offset2 = (offset >> 32) & 0xffffffff;
    kernelIDT[th].attribute = attribute;
    kernelIDT[th].CS = CS;
    kernelIDT[th].reserved = 0;
    kernelIDT[th].IST = IST;
  }

  asm("sti");
}

extern "C" void exceptionHandler(std::uint64_t errCode, void *stackStart) {
  kout << "error!\n";

  halt();
}
} // namespace AMD64