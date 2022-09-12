#include "key_value_storage.h"
#include <map>

namespace ara {
namespace per {

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

ara::core::Result<SharedHandle<KeyValueStorage> > OpenKeyValueStorage(const ara::core::InstanceSpecifier &kvs) noexcept {
  // permission
  auto it = fs_global_config.find(kvs.ToString());
  if(it == fs_global_config.end()) {
    // no permission
    // TODO return error ?
    return ara::core::Result<SharedHandle<KeyValueStorage> >(ara::core::ErrorCode( static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kInitValueNotAvailable), GetPerDomain()));
  }
  auto config = it->second.begin();\

  // open leveldb
  leveldb::DB *db;
  leveldb::Options opt;
  opt.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(opt, config->path + "kvdb", &db);
  if(!status.ok()) {
    return ara::core::Result<SharedHandle<KeyValueStorage> >(ara::core::ErrorCode( static_cast<ara::core::ErrorDomain::CodeType>(PerErrc::kIntegrityCorrupted), GetPerDomain()));
  }

  // create kv storage
  return ara::core::Result<SharedHandle<KeyValueStorage> >(KeyValueStorage::GetInstance(db));
}

// ara::core::Result<void> RecoverKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;
// ara::core::Result<void> ResetKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;
// ara::core::Result<void> ResetKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;

/** KeyValueStorage */
KeyValueStorage::~KeyValueStorage () noexcept {
  delete level_db_;
}
ara::core::Result<ara::core::Vector<ara::core::String> > KeyValueStorage::GetAllKeys () const noexcept {

}

ara::core::Result<bool> KeyValueStorage::KeyExists (ara::core::StringView key) const noexcept {
  return ara::core::Result<bool>(true);
}

template <class T>
ara::core::Result<T> KeyValueStorage::GetValue (ara::core::StringView key) const noexcept {
  return ara::core::Result<void>();
}

template <class T>
ara::core::Result<void> KeyValueStorage::SetValue (ara::core::StringView key, const T &value) noexcept {
  return ara::core::Result<void>();
}

ara::core::Result<void> KeyValueStorage::RemoveKey (ara::core::StringView key) noexcept {
  level_db_->Delete(leveldb::WriteOptions(), key);
  return ara::core::Result<void>();
}

ara::core::Result<void> KeyValueStorage::RecoverKey (ara::core::StringView key) noexcept {
  return ara::core::Result<void>();
}

ara::core::Result<void> KeyValueStorage::ResetKey (ara::core::StringView key) noexcept {
  return ara::core::Result<void>();
}

ara::core::Result<void> KeyValueStorage::RemoveAllKeys () noexcept {
  return ara::core::Result<void>();
}

ara::core::Result<void> KeyValueStorage::SyncToStorage () noexcept {
  return ara::core::Result<void>();  
}

ara::core::Result<void> KeyValueStorage::DiscardPendingChanges () noexcept {
  return ara::core::Result<void>();
}

} // ns per
} // ns ara
