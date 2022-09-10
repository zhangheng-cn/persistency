#include "read_write_accessor.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

namespace ara {
namespace per {
ara::core::Result<void> ReadWriteAccessor::SyncToFile () noexcept {
  fflush(fp_);
  return ara::core::Result<void>();
}

ara::core::Result<void> ReadWriteAccessor::SetFileSize (std::uint64_t size) noexcept {
  if(size > GetSize()) {
    return ara::core::Result<void>();
  }
  ftruncate(fileno(fp_), size);
  return ara::core::Result<void>();
}

ara::core::Result<void> ReadWriteAccessor::WriteText (ara::core::StringView s) noexcept {
  fwrite(s.data(), sizeof(char), s.size(), fp_);
  return ara::core::Result<void>();
}
// ara::core::Result<void> WriteBinary (ara::core::Span< const ara::core::Byte > b) noexcept;
ReadWriteAccessor& ReadWriteAccessor::operator<< (ara::core::StringView s) noexcept {
  WriteText(s);
  return *this;
}

} // ns per
} // ns rar