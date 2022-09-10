#ifndef __FILE_STORAGE_H__
#define __FILE_STORAGE_H__

#include "read_accessor.h"
#include "ara/core/result.h"
#include "ara/core/core.h"
#include "ara/core/instance_specifier.h"
#include "ara/per/shared_handle.h"
#include "ara/per/unique_handle.h"
#include "ara/per/read_accessor.h"
#include "ara/per/read_write_accessor.h"

namespace ara {
namespace per {
class FileStorage;
ara::core::Result<SharedHandle<FileStorage> > OpenFileStorage (const ara::core::InstanceSpecifier &fs) noexcept;
ara::core::Result<void> RecoverAllFiles (const ara::core::InstanceSpecifier &fs) noexcept;
ara::core::Result<void> ResetAllFiles (const ara::core::InstanceSpecifier &fs) noexcept;
ara::core::Result<std::uint64_t> GetCurrentFileStorageSize (const ara::core::InstanceSpecifier &fs) noexcept;

enum class OpenMode : std::uint32_t {
  kAtTheBeginning= 1 << 0,
  kAtTheEnd= 1 << 1,
  kTruncate= 1 << 2,
  kAppend= 1 << 3
};

constexpr OpenMode operator| (OpenMode left, OpenMode right);
OpenMode& operator|= (OpenMode &left, const OpenMode &right);

enum class FileCreationState : std::uint32_t {
  kCreatedDuringInstallion= 1,
  kCreatedDuringUpdate= 2,
  kCreatedDuringReset= 3,
  kCreatedDuringRecovery= 4,
  kCreatedByApplication= 5
};

enum class FileModificationState : std::uint32_t {
  kModifiedDuringUpdate= 2,
  kModifiedDuringReset= 3,
  kModifiedDuringRecovery= 4,
  kModifiedByApplication= 5
};

struct FileInfo {
  std::uint64_t creationTime;
  std::uint64_t modificationTime;
  std::uint64_t accessTime;
  FileCreationState fileCreationState;
  FileModificationState fileModificationState;
};

class FileStorage {
public:
  FileStorage ()=delete;
  FileStorage (FileStorage &&fs)=delete;
  FileStorage (const FileStorage &)=delete;
  FileStorage& operator= (FileStorage &&fs)=delete;
  FileStorage& operator= (const FileStorage &)=delete;
  ~FileStorage () noexcept;
  ara::core::Result<ara::core::Vector<ara::core::String> > GetAllFileNames () const noexcept;
  ara::core::Result<void> DeleteFile (ara::core::StringView fileName) noexcept;
  ara::core::Result<bool> FileExists (ara::core::StringView fileName) const noexcept;
  ara::core::Result<void> RecoverFile (ara::core::StringView fileName) noexcept;
  ara::core::Result<void> ResetFile (ara::core::StringView fileName) noexcept;
  ara::core::Result<std::uint64_t> GetCurrentFileSize (ara::core::StringView fileName) const noexcept;
  ara::core::Result<FileInfo> GetFileInfo (ara::core::StringView fileName) const noexcept;
  ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileReadWrite (ara::core::StringView fileName) noexcept;
  ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileReadWrite (ara::core::StringView fileName, OpenMode mode) noexcept;
  // ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileReadWrite (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;
  ara::core::Result<UniqueHandle<ReadAccessor> > OpenFileReadOnly (ara::core::StringView fileName) noexcept;
  ara::core::Result<UniqueHandle<ReadAccessor> > OpenFileReadOnly (ara::core::StringView fileName, OpenMode mode) noexcept;
  // ara::core::Result<UniqueHandle<ReadAccessor> > OpenFileReadOnly (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;
  ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileWriteOnly (ara::core::StringView fileName) noexcept;
  ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileWriteOnly (ara::core::StringView fileName, OpenMode mode) noexcept;
  // ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileWriteOnly (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;
private:
  friend ara::core::Result<SharedHandle<FileStorage> > OpenFileStorage (const ara::core::InstanceSpecifier &fs) noexcept;
  friend ara::core::Result<void> RecoverAllFiles (const ara::core::InstanceSpecifier &fs) noexcept;
  friend ara::core::Result<void> ResetAllFiles (const ara::core::InstanceSpecifier &fs) noexcept;
  friend ara::core::Result<std::uint64_t> GetCurrentFileStorageSize (const ara::core::InstanceSpecifier &fs) noexcept;
private:
  FileStorage(ara::core::StringView path) {}
  ara::core::StringView path;
};


} // ns per
} // ns ara

#endif // __FILE_STORAGE_H__