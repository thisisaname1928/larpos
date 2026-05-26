#include "kutils.hpp"
#include <cstdint>
#include <limine/limine.h>

namespace {

__attribute__((used, section(".limine_requests"))) volatile std::uint64_t
    limine_base_revision[] = LIMINE_BASE_REVISION(6);

}

namespace {

__attribute__((used,
               section(".limine_requests"))) volatile limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
                           .revision = 0,
                           .response = nullptr};
}

namespace {

__attribute__((used, section(".limine_requests_start"))) volatile std::uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) volatile std::uint64_t
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

} // namespace

volatile std::uint32_t *fb_ptr;
limine_framebuffer *fb;

extern "C" {
void _start() {
  fb = framebuffer_request.response->framebuffers[0];
  fb_ptr = static_cast<volatile std::uint32_t *>(fb->address);
  PSF1::parseZapFont();
  konsole::init();

  int c = 0;

  while (true) {
    if (c == 0) {
      kout << "LLL\n";
      c++;
    } else {
      kout << "AAA\n";
      c = 0;
    }
  }
}
}