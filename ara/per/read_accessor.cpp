#include "read_accessor.h"

#include <sys/stat.h>

namespace ara {
namespace per {

ReadAccessor::~ReadAccessor() {
  fclose(fp_);
}
// kPhysicalStorageFailure kIsEof
ara::core::Result<char> ReadAccessor::PeekChar () const noexcept {
  return std::move(ara::core::Result<char>('a'));
  return std::move(ara::core::Result<char>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain())));
}
// kPhysicalStorageFailure kIsEof
ara::core::Result<ara::core::Byte> ReadAccessor::PeekByte () const noexcept {
  return std::move(ara::core::Result<ara::core::Byte>('a'));

}
// kPhysicalStorageFailure kIsEof
ara::core::Result<char> ReadAccessor::GetChar () noexcept {
  return std::move(ara::core::Result<char>('a'));
}
// kPhysicalStorageFailure kIsEof
ara::core::Result<ara::core::Byte> ReadAccessor::GetByte () noexcept {
  return std::move(ara::core::Result<ara::core::Byte>('a'));
}
// kPhysicalStorageFailure kIsEof
ara::core::Result<ara::core::String> ReadAccessor::ReadText () noexcept {
  ara::core::String str{};
  uint64_t need_size = GetSize() - GetPosition();
  str.resize(need_size);
  uint64_t read_len = fread((void*)str.c_str(), sizeof(char), need_size, fp_);
  return std::move(ara::core::Result<ara::core::String>(std::move(str)));
}
// kPhysicalStorageFailure kIsEof
ara::core::Result<ara::core::String> ReadAccessor::ReadText (std::uint64_t n) noexcept {
  if(n > GetSize() - GetPosition()) {
    return ara::core::Result<ara::core::String>("error");
  }
  ara::core::String str{};
  str.resize(n);
  uint64_t read_len = fread((void*)str.c_str(), sizeof(char), n, fp_);
  return std::move(ara::core::Result<ara::core::String>(std::move(str)));
}

ara::core::Result<ara::core::Vector<ara::core::Byte> > ReadAccessor::ReadBinary () noexcept {
  ara::core::Vector<ara::core::Byte> vec{};
  uint64_t need_size = GetSize() - GetPosition();
  vec.resize(need_size);
  uint64_t read_len = fread(vec.data(), sizeof(ara::core::Byte), need_size, fp_);
  return std::move(ara::core::Result<ara::core::Vector<ara::core::Byte> >(std::move(vec)));
}

ara::core::Result<ara::core::Vector<ara::core::Byte> > ReadAccessor::ReadBinary (std::uint64_t n) noexcept {
  ara::core::Vector<ara::core::Byte> vec{};
  if(n > GetSize() - GetPosition() ) {
    return ara::core::Result<ara::core::Vector<ara::core::Byte> >(vec);
  }
  vec.resize(n);
  uint64_t read_len = fread(vec.data(), sizeof(ara::core::Byte), n, fp_);
  return std::move(ara::core::Result<ara::core::Vector<ara::core::Byte> >(std::move(vec)));
}

ara::core::Result<ara::core::String> ReadAccessor::ReadLine (char delimiter) noexcept {
  return ara::core::Result<ara::core::String>("readline");
}
std::uint64_t ReadAccessor::GetSize () const noexcept {
  struct stat fs;
  fstat(fileno(fp_), &fs);
  return fs.st_size;
}
std::uint64_t ReadAccessor::GetPosition () const noexcept {
  return ftell(fp_);
}
// kInvalidPosition
ara::core::Result<void> ReadAccessor::SetPosition (std::uint64_t position) noexcept {
  fseek(fp_, position, SEEK_SET);
  return ara::core::Result<void>();
}
// kInvalidPosition
ara::core::Result<std::uint64_t> ReadAccessor::MovePosition (Origin origin, std::int64_t offset) noexcept {
  if(origin == Origin::kBeginning) {
    fseek(fp_, offset, SEEK_SET);
  } else if (origin == Origin::kCurrent){
    fseek(fp_, offset, SEEK_CUR);
  } else {
    fseek(fp_, offset, SEEK_END);
  }
  return ara::core::Result<std::uint64_t>(GetPosition());
}
bool ReadAccessor::IsEof () const noexcept {
  return feof(fp_) ? true : false;
}


} // ns per
} // ns ara