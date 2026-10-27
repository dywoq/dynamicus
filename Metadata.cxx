// Copyright 2026 dywoq - Apache License 2.0
// https://github.com/dywoq/dynamicus

#include "Dynamicus.hxx"

namespace dynamicus {

std::uint16_t getMajorVersion() noexcept { return MAJOR_VERSION; }

std::uint16_t getMinorVersion() noexcept { return MINOR_VERSION; }

BuildMode getBuildMode() noexcept {
#if RELEASE
  return BuildMode::RELEASE_MODE;
#elif DEBUG
  return BuildMode::DEBUG_MODE;
#endif
}

} // namespace dynamicus
