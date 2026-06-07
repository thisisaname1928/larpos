#include "arch.hpp"
#include <cstdint>

namespace arch {
std::uint64_t kernelGDT[] = {0x0, 0x00af9a000000ffff, 0x00cf92000000ffff};

GDTR kernelGDTR = {
    .size = sizeof(kernelGDT) - 1,
    .offset = (std::uint64_t)&kernelGDT,
};

} // namespace arch