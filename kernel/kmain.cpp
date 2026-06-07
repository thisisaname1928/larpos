#include "arch/amd64/arch.hpp"
#include "arch/arch.hpp"
#include "kutils.hpp"
#include <cstddef>
#include <cstdint>
#include <limine/limine.h>

namespace {

__attribute__((used, section(".limine_requests"))) volatile std::uint64_t
    limine_base_revision[] = LIMINE_BASE_REVISION(6);

}

namespace {

__attribute__((used,
               section(".limine_requests"))) volatile limine_framebuffer_request
    framebufferRequest = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
                          .revision = 0,
                          .response = nullptr};
}

namespace {
__attribute__((
    used,
    section(".limine_requests"))) volatile limine_hhdm_request hddmRequest = {
    .id = LIMINE_HHDM_REQUEST_ID, .revision = 0, .response = nullptr};
} // namespace

namespace {

__attribute__((used, section(".limine_requests_start"))) volatile std::uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) volatile std::uint64_t
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

} // namespace

volatile std::uint32_t *fb_ptr;
limine_framebuffer *fb;
std::uint64_t kernelMappingOffset;

extern "C" {
void _start() {
  if (hddmRequest.response != NULL) {
    kernelMappingOffset = hddmRequest.response->offset;
  } else
    arch::halt();

  fb = framebufferRequest.response->framebuffers[0];
  fb_ptr = static_cast<volatile std::uint32_t *>(fb->address);
  PSF1::parseZapFont();
  konsole::init();

  arch::init();

  kout << "hi!\n" << (std::uint64_t)123 << "\n";

  arch::halt();
}
}