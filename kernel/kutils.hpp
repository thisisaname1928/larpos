#pragma once
#include <cstdint>
#include <limine/limine.h>

extern "C" {
void *memcpy(void *__restrict dest, const void *__restrict src, std::size_t n);
void *memset(void *s, int c, std::size_t n);
void *memmove(void *dest, const void *src, std::size_t n);
int memcmp(const void *s1, const void *s2, std::size_t n);
}

extern "C" {
extern int _binary_assets_zap_psf_start;
extern int _binary_assets_zap_psf_end;
}
namespace PSF1 {
#define MAGIC 0x0436

typedef struct {
  uint16_t magic;
  uint8_t fontMode;
  uint8_t characterSize;
} Header;

void parseZapFont();
char *getZapGlyph(int code);

} // namespace PSF1

extern PSF1::Header *ZapFontHeader;
extern volatile std::uint32_t *fb_ptr;
extern limine_framebuffer *fb;
void kputchar(char ch, int x, int y, std::uint32_t color);

namespace konsole {
void init();
void send(std::uint8_t ch);
extern int height, width;
extern int x, y;

class kout {
public:
  kout &operator<<(const char *rhs);

private:
};
} // namespace konsole

extern konsole::kout kout;
