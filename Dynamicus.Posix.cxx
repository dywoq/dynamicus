// Copyright 2026 dywoq - Apache License 2.0
// https://github.com/dywoq/dynamicus

#include "Dynamicus.hxx"
#include <dlfcn.h>

namespace dynamicus {

OsType getOs() noexcept { return OsType::LINUX; }

ArchType getArch() noexcept {
#if ARCH_AMD64
  return ArchType::AMD64;
#endif
}

struct Library::hiddenInfo {
  void *handle;
};

Library::Library() noexcept {}
Library::Library(const std::filesystem::path &libraryPath) noexcept
    : m_hiddenInfoPtr(std::make_unique<hiddenInfo>()) {
  load(libraryPath);
}
Library::Library(const char *libraryPath) noexcept
    : m_hiddenInfoPtr(std::make_unique<hiddenInfo>()) {
  load(libraryPath);
}

Library::~Library() noexcept { destroy(); }

Library::Condition Library::getCondition() noexcept { return m_condition; }

void Library::load(const std::filesystem::path &libraryPath) noexcept {
  if (m_condition == Condition::INITIALIZED)
    return;
  m_hiddenInfoPtr->handle = dlopen(libraryPath.c_str(), RTLD_LAZY | RTLD_NOW);
  m_condition =
      dlerror() == nullptr ? Condition::INITIALIZED : Condition::FAILED;
}

void Library::load(const char *libraryPath) noexcept {
  load(std::filesystem::path(libraryPath));
}

void Library::destroy() noexcept {
  if (m_condition == Condition::DESTROYED)
    return;
  dlclose(m_hiddenInfoPtr->handle);
  m_condition = Condition::DESTROYED;
}

Status<void *> Library::getSymbolAddress(std::string_view symbolName) noexcept {
  if (m_condition == Condition::DESTROYED)
    return StatusCode::fromEnum(Codes::DESTROYED);
  if (m_condition == Condition::FAILED)
    return StatusCode::fromEnum(Codes::FAILED_TO_INITIALIZE);

  void *symbolAddress = dlsym(m_hiddenInfoPtr->handle, symbolName.data());
  return symbolAddress
             ? Status(symbolAddress)
             : StatusCode::fromEnum(Codes::GETTING_SYMBOL_ADDR_FAILED);
}

} // namespace dynamicus
