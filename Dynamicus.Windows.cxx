// Copyright 2026 dywoq - Apache License 2.0
// https://github.com/dywoq/dynamicus

#include "Dynamicus.hxx"
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace dynamicus {

OsType getOs() noexcept { return OsType::WINDOWS; }

ArchType getArch() noexcept {
#if ARCH_AMD64
  return ArchType::AMD64;
#endif
}

struct Library::hiddenInfo {
  HMODULE module;
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
  this->m_hiddenInfoPtr->module = LoadLibraryW(libraryPath.c_str());
  this->m_condition = this->m_hiddenInfoPtr->module ? Condition::INITIALIZED
                                                    : Condition::FAILED;
}

void Library::load(const char *libraryPath) noexcept {
  load(std::filesystem::path(libraryPath));
}

void Library::destroy() noexcept {
  if (this->m_condition == Condition::DESTROYED)
    return;
  FreeLibrary(m_hiddenInfoPtr->module);
  this->m_condition = Condition::DESTROYED;
}

Status<void *> Library::getSymbolAddress(std::string_view symbolName) noexcept {
  if (m_condition == Condition::DESTROYED)
    return StatusCode::fromEnum(Codes::DESTROYED);
  if (m_condition == Condition::FAILED)
    return StatusCode::fromEnum(Codes::FAILED_TO_INITIALIZE);

  auto address = GetProcAddress(m_hiddenInfoPtr->module, symbolName.data());
  return address ? Status(reinterpret_cast<void *>(address))
                 : StatusCode::fromEnum(Codes::GETTING_SYMBOL_ADDR_FAILED);
}

} // namespace dynamicus
