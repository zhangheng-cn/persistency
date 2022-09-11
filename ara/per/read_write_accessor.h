#ifndef __READ_WRITE_ACCESSOR_H__
#define __READ_WRITE_ACCESSOR_H__

#include "read_accessor.h"

namespace ara {
namespace per {

class ReadWriteAccessor : public ReadAccessor {
public:
  ReadWriteAccessor ()=delete;
  ara::core::Result<void> SyncToFile () noexcept;
  ara::core::Result<void> SetFileSize (std::uint64_t size) noexcept;
  ara::core::Result<void> WriteText (ara::core::StringView s) noexcept;
  // ara::core::Result<void> WriteBinary (ara::core::Span< const ara::core::Byte > b) noexcept;
  ReadWriteAccessor& operator<< (ara::core::StringView s) noexcept;
private:
  friend class FileStorage;
  ReadWriteAccessor(FILE *fp, bool readbale=true): ReadAccessor(fp, readbale) {}
};

} // ns per
} // ns ara

#endif // __READ_WRITE_ACCESSOR_H__