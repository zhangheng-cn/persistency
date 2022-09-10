#include "file_storage.h"
#include <string.h>

namespace ara {
namespace per {
ara::core::Result<SharedHandle<FileStorage> > OpenFileStorage (const ara::core::InstanceSpecifier &fs) noexcept {
  return ara::core::Result<SharedHandle<FileStorage> >(SharedHandle<FileStorage>(new FileStorage("test_file_storage")));
}
ara::core::Result<void> RecoverAllFiles (const ara::core::InstanceSpecifier &fs) noexcept {}
ara::core::Result<void> ResetAllFiles (const ara::core::InstanceSpecifier &fs) noexcept {}
ara::core::Result<std::uint64_t> GetCurrentFileStorageSize (const ara::core::InstanceSpecifier &fs) noexcept {}
/** FileStorage */
FileStorage::~FileStorage () noexcept {}
ara::core::Result<ara::core::Vector<ara::core::String> > FileStorage::GetAllFileNames () const noexcept {}
ara::core::Result<void> FileStorage::DeleteFile (ara::core::StringView fileName) noexcept {}
ara::core::Result<bool> FileStorage::FileExists (ara::core::StringView fileName) const noexcept {}
ara::core::Result<void> FileStorage::RecoverFile (ara::core::StringView fileName) noexcept {}
ara::core::Result<void> FileStorage::ResetFile (ara::core::StringView fileName) noexcept {}
ara::core::Result<std::uint64_t> FileStorage::GetCurrentFileSize (ara::core::StringView fileName) const noexcept {}
ara::core::Result<FileInfo> FileStorage::GetFileInfo (ara::core::StringView fileName) const noexcept {}
ara::core::Result<UniqueHandle<ReadWriteAccessor> > FileStorage::OpenFileReadWrite (ara::core::StringView fileName) noexcept {}
ara::core::Result<UniqueHandle<ReadWriteAccessor> > FileStorage::OpenFileReadWrite (ara::core::StringView fileName, OpenMode mode) noexcept {}
// ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileReadWrite (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;
ara::core::Result<UniqueHandle<ReadAccessor> > FileStorage::OpenFileReadOnly (ara::core::StringView fileName) noexcept {
  FILE *fp = fopen(fileName.c_str(), "a+b");
  if(fp == nullptr) {
    std::cout << "file open failed !" << fileName << " " << strerror(errno) << "\n";
  }
  return ara::core::Result<UniqueHandle<ReadAccessor> >(UniqueHandle<ReadAccessor>(new ReadAccessor(fp)));
}
ara::core::Result<UniqueHandle<ReadAccessor> > FileStorage::OpenFileReadOnly (ara::core::StringView fileName, OpenMode mode) noexcept {}
// ara::core::Result<UniqueHandle<ReadAccessor> > OpenFileReadOnly (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;
ara::core::Result<UniqueHandle<ReadWriteAccessor> > FileStorage::OpenFileWriteOnly (ara::core::StringView fileName) noexcept {}
ara::core::Result<UniqueHandle<ReadWriteAccessor> > FileStorage::OpenFileWriteOnly (ara::core::StringView fileName, OpenMode mode) noexcept {}
// ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileWriteOnly (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;

} // ns per
} // ns ara