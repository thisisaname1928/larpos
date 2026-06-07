#include "kutils.hpp"
#include <cstddef>
#include <cstdint>

// GCC and Clang reserve the right to generate calls to the following
// 4 C-linkage functions even if they are not directly called.
// They must be implemented as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" {

int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
void __cxa_pure_virtual() {
  while (true) {
  }
}
void *__dso_handle;

void *memcpy(void *__restrict dest, const void *__restrict src, std::size_t n) {
  std::uint8_t *__restrict pdest = static_cast<std::uint8_t *__restrict>(dest);
  const std::uint8_t *__restrict psrc =
      static_cast<const std::uint8_t *__restrict>(src);

  for (std::size_t i = 0; i < n; i++) {
    pdest[i] = psrc[i];
  }

  return dest;
}

void *memset(void *s, int c, std::size_t n) {
  std::uint8_t *p = static_cast<std::uint8_t *>(s);

  for (std::size_t i = 0; i < n; i++) {
    p[i] = static_cast<uint8_t>(c);
  }

  return s;
}

void *memmove(void *dest, const void *src, std::size_t n) {
  std::uint8_t *pdest = static_cast<std::uint8_t *>(dest);
  const std::uint8_t *psrc = static_cast<const std::uint8_t *>(src);

  if (reinterpret_cast<std::uintptr_t>(src) >
      reinterpret_cast<std::uintptr_t>(dest)) {
    for (std::size_t i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else if (reinterpret_cast<std::uintptr_t>(src) <
             reinterpret_cast<std::uintptr_t>(dest)) {
    for (std::size_t i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *s1, const void *s2, std::size_t n) {
  const std::uint8_t *p1 = static_cast<const std::uint8_t *>(s1);
  const std::uint8_t *p2 = static_cast<const std::uint8_t *>(s2);

  for (std::size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}
}

PSF1::Header *ZapFontHeader;

void PSF1::parseZapFont() {
  ZapFontHeader = (Header *)(&_binary_assets_zap_psf_start);
}

char *PSF1::getZapGlyph(int code) {
  return (char *)((char *)ZapFontHeader + sizeof(Header) +
                  code * ZapFontHeader->characterSize);
}

void put_pixel(int x, int y, std::uint32_t color) {
  fb_ptr[y * (fb->pitch / 4) + x] = color;
}

void kputchar(char ch, int x, int y, std::uint32_t color) {
  char *glyph = PSF1::getZapGlyph(ch);

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 8; j++) {
      if ((glyph[i] >> (7 - j)) & 0x1) {
        put_pixel(x * 8 + j, y * 16 + i, color);
      }
    }
  }
}

int konsole::width = 0, konsole::height = 0, konsole::x = 0, konsole::y = 0;

void konsole::init() {
  width = fb->width / 8;
  height = fb->height / 16;
}

void konsole::send(std::uint8_t ch) {
  if (x >= width) {
    y++;
    x = 0;
  }

  if (y >= height) {
    // scroll up
    memmove((void *)fb_ptr, (const char *)fb_ptr + fb->pitch * 16,
            (fb->pitch * (fb->height - 16)));
    memset((void *)((char *)fb_ptr + fb->pitch * (fb->height - 16)), 0,
           fb->pitch * 16);
    x = 0;
    y = height - 1;
  }

  if (ch == '\n') {
    y++;
    x = 0;
  } else if (ch == '\r') {
    x = 0;
  } else {
    kputchar(ch, x, y, 0xffffff);
    x++;
  }
}

konsole::kout &konsole::kout::operator<<(const char *rhs) {
  while (*rhs != 0) {
    send(*rhs);
    rhs++;
  }

  return *this;
}

konsole::kout &konsole::kout::operator<<(std::uint64_t n) {
  if (n == 0) {
    send('0');
    return *this;
  }

  char buffer[20];
  int c = 0;

  while (n != 0) {
    buffer[c] = (n % 10);
    n /= 10;
    c++;
  }

  for (int i = c - 1; i >= 0; i--) {
    send(buffer[i] + 48);
  }

  return *this;
}

const char hexTransTab[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

konsole::kout &konsole::kout::operator<<(hex n) {
  if (n == 0) {
    send('0');
    return *this;
  }

  char buffer[20];
  int c = 0;

  while (n != 0) {
    buffer[c] = (n % 16);
    n /= 16;
    c++;
  }

  for (int i = c - 1; i >= 0; i--) {
    send(hexTransTab[(int)buffer[i]]);
  }

  return *this;
}

konsole::kout kout;