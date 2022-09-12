#include "file_storage.h"
#include <string.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

/** ****************************** 打开filestorage的权限控制 ******************************/
enum Permission : std::uint8_t {
  kRead = 1 << 0,
  kWrite = 1 << 1,
};
struct InstanceConfig {
  ara::core::String path;
  std::uint8_t permission;
};

std::map<ara::core::String, ara::core::Vector<InstanceConfig> > fs_global_config {
  {"/mid/p1", {{"/home/zh/code/persistency/test/p1/", Permission::kRead | Permission::kWrite},
               {"/home/zh/code/persistency/test/p1/", Permission::kRead}}},
  {"/mid/p1", {{"/home/zh/code/persistency/test/p2/", Permission::kRead | Permission::kWrite}}}
}; 
/** ****************************** *******************************/
namespace ara {
namespace per {
// kStorageNotFound 
ara::core::Result<SharedHandle<FileStorage> > OpenFileStorage (const ara::core::InstanceSpecifier &fs) noexcept {
  auto it = fs_global_config.find(fs.ToString());
  if(it == fs_global_config.end()) {
    // no permission
    // TODO return error ?
    return ara::core::Result<SharedHandle<FileStorage> >(ara::core::ErrorCode( static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kInitValueNotAvailable), GetPerDomain()));
  }
  // vector first value as config
  auto config = it->second.begin();
  return ara::core::Result<SharedHandle<FileStorage> >(FileStorage::GetInstance(config->path, config->permission));
}
// ara::core::Result<void> RecoverAllFiles (const ara::core::InstanceSpecifier &fs) noexcept {}
// ara::core::Result<void> ResetAllFiles (const ara::core::InstanceSpecifier &fs) noexcept {}
ara::core::Result<std::uint64_t> GetCurrentFileStorageSize (const ara::core::InstanceSpecifier &fs) noexcept {
  auto it = fs_global_config.find(fs.ToString());
  if(it == fs_global_config.end()) {
    // no config
    // TODO return error ?
    return ara::core::Result<std::uint64_t>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kInitValueNotAvailable), GetPerDomain()));
  }
  auto config = it->second.begin();
  struct stat dir_stat;
  int32_t ret = stat(config->path.c_str(), &dir_stat);
  return ara::core::Result<std::uint64_t>(dir_stat.st_size);
}
/** FileStorage */
FileStorage::~FileStorage () noexcept {}

// ara::core::Result<ara::core::Vector<ara::core::String> > FileStorage::GetAllFileNames () const noexcept {}

ara::core::Result<void> FileStorage::DeleteFile (ara::core::StringView fileName) noexcept {
  if(!(permission_ & Permission::kWrite)) {
    return ara::core::Result<void>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kIllegalWriteAccess), GetPerDomain()));
  }
  if(access((path_ + fileName).c_str(), 0)) {
    return ara::core::Result<void>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kFileNotFound), GetPerDomain()));
  }
  if(remove((path_ + fileName).c_str())) {
    // TODO delete failed
    if(errno == EACCES) {} // 无权限
    if(errno == EBUSY) {
      return ara::core::Result<void>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kResourceBusy), GetPerDomain()));
    }
    return ara::core::Result<void>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  return ara::core::Result<void>();
}

ara::core::Result<bool> FileStorage::FileExists (ara::core::StringView fileName) const noexcept {
  if(access((path_ + fileName).c_str(), 0)) {
    if(errno == ENOENT) { // file not exist
      return ara::core::Result<bool>(false);
    }
    return ara::core::Result<bool>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  return ara::core::Result<bool>(true);
}

// ara::core::Result<void> FileStorage::RecoverFile (ara::core::StringView fileName) noexcept {}

// ara::core::Result<void> FileStorage::ResetFile (ara::core::StringView fileName) noexcept {}

ara::core::Result<std::uint64_t> FileStorage::GetCurrentFileSize (ara::core::StringView fileName) const noexcept {
  if(access((path_ + fileName).c_str(), 0)) {
    if(errno == ENOENT) { // file not exist
      return ara::core::Result<std::uint64_t>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kFileNotFound), GetPerDomain()));
    }
    return ara::core::Result<std::uint64_t>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  struct stat fs;
  int32_t ret = stat((path_ + fileName).c_str(), &fs);
  return ara::core::Result<std::uint64_t>(fs.st_size);
}

ara::core::Result<FileInfo> FileStorage::GetFileInfo (ara::core::StringView fileName) const noexcept {
  if(access((path_ + fileName).c_str(), 0)) {
    if(errno == ENOENT) { // file not exist
      return ara::core::Result<FileInfo>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kFileNotFound), GetPerDomain()));
    }
    return ara::core::Result<FileInfo>(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  struct stat fs;
  int32_t ret = stat((path_ + fileName).c_str(), &fs);
  FileInfo fs_info;
  // TODO 时间精度 s/ns ?
  fs_info.creationTime = static_cast<uint64_t>(fs.st_ctim.tv_sec);
  fs_info.accessTime = static_cast<uint64_t>(fs.st_atim.tv_sec);
  fs_info.modificationTime = static_cast<uint64_t>(fs.st_mtim.tv_sec);
  return ara::core::Result<FileInfo>(std::move(fs_info));
}

ara::core::Result<UniqueHandle<ReadWriteAccessor> > FileStorage::OpenFileReadWrite (ara::core::StringView fileName) noexcept {
  if(!(permission_ & Permission::kRead 
      && permission_ & Permission::kWrite)) {
    return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kIllegalWriteAccess), GetPerDomain()));
  }
  FILE *fp = fopen((path_ + fileName).c_str(), "w+b");
  if(fp == nullptr) {
    // TODO 根据errorno 判断返回的error
    // 文件不存在
    if(errno == EACCES) {}
    if(errno == EBUSY) {
      return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kResourceBusy), GetPerDomain()));
    }
    return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(UniqueHandle<ReadWriteAccessor>(new ReadWriteAccessor(fp)));
}

ara::core::Result<UniqueHandle<ReadWriteAccessor> > FileStorage::OpenFileReadWrite (ara::core::StringView fileName, OpenMode mode) noexcept {
  // MODE
  if(!(permission_ & Permission::kRead && permission_ & Permission::kWrite)) {
    return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kValidationFailed), GetPerDomain()));
  }
  FILE *fp = fopen((path_ + fileName).c_str(), "w+b");
  if(fp == nullptr) {
    // TODO 根据errorno 判断返回的error
    // 文件不存在
    if(errno == EACCES) {}
    if(errno == EBUSY) {
      return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kResourceBusy), GetPerDomain()));
    }
    return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(UniqueHandle<ReadWriteAccessor>(new ReadWriteAccessor(fp)));
}

// ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileReadWrite (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;

// 打开直接读
ara::core::Result<UniqueHandle<ReadAccessor> > FileStorage::OpenFileReadOnly (ara::core::StringView fileName) noexcept {
  if(!(permission_ & Permission::kRead)) {
    return ara::core::Result<UniqueHandle<ReadAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kValidationFailed), GetPerDomain()));
  }
  FILE *fp = fopen((path_ + fileName).c_str(), "rb");
  if(fp == nullptr) {
    // TODO 根据errorno 判断返回的error
    // 文件不存在
    if(errno == EACCES) {}
    if(errno == EBUSY) {
      return ara::core::Result<UniqueHandle<ReadAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kResourceBusy), GetPerDomain()));
    }
    return ara::core::Result<UniqueHandle<ReadAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  return ara::core::Result<UniqueHandle<ReadAccessor> >(UniqueHandle<ReadAccessor>(new ReadAccessor(fp)));
}

// mode
ara::core::Result<UniqueHandle<ReadAccessor> > FileStorage::OpenFileReadOnly (ara::core::StringView fileName, OpenMode mode) noexcept {
  if(!(permission_ & Permission::kRead)) {
    return ara::core::Result<UniqueHandle<ReadAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kValidationFailed), GetPerDomain()));
  }
  FILE *fp = fopen((path_ + fileName).c_str(), "rb");
  if(fp == nullptr) {
    // TODO 根据errorno 判断返回的error
    // 文件不存在
    if(errno == EACCES) {}
    if(errno == EBUSY) {
      return ara::core::Result<UniqueHandle<ReadAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kResourceBusy), GetPerDomain()));
    }
    return ara::core::Result<UniqueHandle<ReadAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  return ara::core::Result<UniqueHandle<ReadAccessor> >(UniqueHandle<ReadAccessor>(new ReadAccessor(fp)));
}

// ara::core::Result<UniqueHandle<ReadAccessor> > OpenFileReadOnly (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;

ara::core::Result<UniqueHandle<ReadWriteAccessor> > FileStorage::OpenFileWriteOnly (ara::core::StringView fileName) noexcept {
  if(!(permission_ & Permission::kWrite)) {
    return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kValidationFailed), GetPerDomain()));
  }
  FILE *fp = fopen((path_ + fileName).c_str(), "wb");
  if(fp == nullptr) {
    // TODO 
    if(errno == EACCES) {}
    if(errno == EBUSY) {
      return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kResourceBusy), GetPerDomain()));
    }
    return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(UniqueHandle<ReadWriteAccessor>(new ReadWriteAccessor(fp, false)));
}

ara::core::Result<UniqueHandle<ReadWriteAccessor> > FileStorage::OpenFileWriteOnly (ara::core::StringView fileName, OpenMode mode) noexcept {
  if(!(permission_ & Permission::kWrite)) {
    return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kValidationFailed), GetPerDomain()));
  }
  FILE *fp = fopen((path_ + fileName).c_str(), "wb");
  if(fp == nullptr) {
    // TODO 
    if(errno == EACCES) {}
    if(errno == EBUSY) {
      return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kResourceBusy), GetPerDomain()));
    }
    return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kPhysicalStorageFailure), GetPerDomain()));
  }
  return ara::core::Result<UniqueHandle<ReadWriteAccessor> >(UniqueHandle<ReadWriteAccessor>(new ReadWriteAccessor(fp, false)));
}

// ara::core::Result<UniqueHandle<ReadWriteAccessor> > OpenFileWriteOnly (ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte > buffer) noexcept;

} // ns per
} // ns ara