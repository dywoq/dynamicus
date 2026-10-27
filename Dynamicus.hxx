// Copyright 2026 dywoq - Apache License 2.0
// https://github.com/dywoq/dynamicus

#ifndef DYNAMICUS_HXX
#define DYNAMICUS_HXX

#include <cstdint>
#include <optional>

namespace dynamicus {

// Represents a status code that identifies either operation failure or success.
// Use it instead of dynamicus::Status<T> if your function does not return any
// values.
struct StatusCode {
  using Type = uint32_t;
  inline constexpr static Type OK = 0;
  Type value = 0;
  constexpr explicit StatusCode() : value(OK) {}
  constexpr explicit StatusCode(Type value) : value(value) {}
  constexpr bool isOkay() const noexcept { return value == OK; }
  constexpr operator bool() const noexcept { return isOkay(); }
};

// Represents a result structure that contains a status code and value. Use it
// instead of dynamicus::StatusCode if your function returns any values.
template <typename T> class Status {
private:
  std::optional<T> m_value;
  StatusCode m_code;

public:
  Status(T value) noexcept : m_value(value), m_code() {}
  Status(StatusCode code) noexcept : m_value(std::nullopt), m_code(code) {}

  // Provides the underlying std::optional<T> instance.
  std::optional<T> getValue() noexcept { return m_value; }

  // Provides the status code. Use it to check whether dynamicus::Status<T>
  // represents a failure or success.
  StatusCode getCode() noexcept { return m_code; }

  // Provides a reference pointer to the underlying value. This operation is
  // considered unsafe because it returs nullptr if the value is not present,
  // and addressing `nullptr` leads to page fault -> program crash. Prefer
  // `getValue` over it if you want to handle "value is not present" situations
  // gracefully.
  T *getValueUnsafe() noexcept {
    if (!m_value.has_value())
      return nullptr;
    return &m_value.value();
  }
};

enum class OsType { WINDOWS, LINUX };
enum class ArchType { AMD64 };

// Provides the current operating system that the library was compiled for.
OsType getOs() noexcept;

// Provides the current architecture that the library was compiled for.
ArchType getArch() noexcept;

} // namespace dynamicus

#endif
