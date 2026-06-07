#include <arch/amd64/arch.hpp>

namespace arch {
void disablePIC() {
  outb(0x20, 0x11);
  ioWait();
  outb(0xA0, 0x11);
  ioWait();
  outb(0x21, 0x20);
  ioWait();
  outb(0xA1, 0x28);
  ioWait();
  outb(0x21, 0x04);
  ioWait();
  outb(0xA1, 0x02);
  ioWait();
  outb(0x21, 0x01);
  ioWait();
  outb(0xA1, 0x01);
  ioWait();
  outb(0x21, 0xFF);
  ioWait();
  outb(0xA1, 0xFF);
  ioWait();
}
} // namespace arch