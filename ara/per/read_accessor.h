#ifndef __READ_ACCESSOR_H__
#define __READ_ACCESSOR_H__

#include <iostream>
#include "ara/core/core.h"
#include "ara/core/result.h"

namespace ara {
namespace per {

enum class Origin : std::uint32_t {
  kBeginning= 0,
  kCurrent= 1,
  kEnd= 2
};


class ReadAccessor {
public: 
  ReadAccessor ()=delete;
  ReadAccessor (ReadAccessor &&ra)=delete;
  ReadAccessor (const ReadAccessor &)=delete;
  ReadAccessor& operator= (ReadAccessor &&ra)=delete;
  ReadAccessor& operator= (const ReadAccessor &)=delete;
  ~ReadAccessor();
  ara::core::Result<char> PeekChar () const noexcept;
  ara::core::Result<ara::core::Byte> PeekByte () const noexcept;
  ara::core::Result<char> GetChar () noexcept;
  ara::core::Result<ara::core::Byte> GetByte () noexcept;
  ara::core::Result<ara::core::String> ReadText () noexcept;
  ara::core::Result<ara::core::String> ReadText (std::uint64_t n) noexcept;
  ara::core::Result<ara::core::Vector<ara::core::Byte> > ReadBinary () noexcept;
  ara::core::Result<ara::core::Vector<ara::core::Byte> > ReadBinary (std::uint64_t n) noexcept;
  ara::core::Result<ara::core::String> ReadLine (char delimiter='\n') noexcept;
  std::uint64_t GetSize () const noexcept;
  std::uint64_t GetPosition () const noexcept;
  ara::core::Result<void> SetPosition (std::uint64_t position) noexcept;
  ara::core::Result<std::uint64_t> MovePosition (Origin origin, std::int64_t offset) noexcept;
  bool IsEof () const noexcept;
protected:
  friend class FileStorage;
  FILE *fp_;
  bool isReadable_;
  ReadAccessor (FILE *fp, bool readbale=true):fp_(fp), isReadable_(readbale) {}
}; 

} // ns per
} // ns ara


#endif // __READ_ACCESSOR_H__