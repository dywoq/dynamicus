// Copyright 2026 dywoq - Apache License 2.0
// https://github.com/dywoq/dynamicus

#include "Dynamicus.hxx"

namespace dynamicus {

OsType getOs() noexcept { return OsType::WINDOWS; }

ArchType getArch() noexcept {
#if ARCH_AMD64
  return ArchType::AMD64;
#endif
}

} // namespace dynamicus
