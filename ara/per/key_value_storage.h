#ifndef __KEY_VALUE_STORAGE_H__
#define __KEY_VALUE_STORAGE_H__

#include "ara/core/result.h"
#include "ara/core/core.h"
#include "ara/core/instance_specifier.h"
#include "ara/per/shared_handle.h"
#include "ara/per/unique_handle.h"
#include "ara/per/per_error_domain.h"

#include "leveldb/db.h"

namespace ara {
namespace per {

class KeyValueStorage;
ara::core::Result<SharedHandle<KeyValueStorage> > OpenKeyValueStorage(const ara::core::InstanceSpecifier &kvs) noexcept;
ara::core::Result<void> RecoverKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;
ara::core::Result<void> ResetKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;
ara::core::Result<void> ResetKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;

class KeyValueStorage final {
public:
  KeyValueStorage ()=delete;
  KeyValueStorage (KeyValueStorage &&kvs)=delete;
  KeyValueStorage (const KeyValueStorage &)=delete;
  KeyValueStorage& operator= (KeyValueStorage &&kvs)=delete;
  KeyValueStorage& operator= (const KeyValueStorage &)=delete;
  ~KeyValueStorage () noexcept;

  ara::core::Result<ara::core::Vector<ara::core::String> > GetAllKeys () const noexcept;
  ara::core::Result<bool> KeyExists (ara::core::StringView key) const noexcept;
  template <class T>
  ara::core::Result<T> GetValue (ara::core::StringView key) const noexcept;
  template <class T>
  ara::core::Result<void> SetValue (ara::core::StringView key, const T &value) noexcept;
  ara::core::Result<void> RemoveKey (ara::core::StringView key) noexcept;
  ara::core::Result<void> RecoverKey (ara::core::StringView key) noexcept;
  ara::core::Result<void> ResetKey (ara::core::StringView key) noexcept;
  ara::core::Result<void> RemoveAllKeys () noexcept;
  ara::core::Result<void> SyncToStorage () noexcept;
  ara::core::Result<void> DiscardPendingChanges () noexcept;
private:
  friend ara::core::Result<SharedHandle<KeyValueStorage> > OpenKeyValueStorage(const ara::core::InstanceSpecifier &kvs) noexcept;
  friend ara::core::Result<void> RecoverKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;
  friend ara::core::Result<void> ResetKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;
  friend ara::core::Result<void> ResetKeyValueStorage (const ara::core::InstanceSpecifier &kvs) noexcept;
private:
  static SharedHandle<KeyValueStorage> GetInstance(leveldb::DB *db) {
    static SharedHandle<KeyValueStorage> instance(new KeyValueStorage(db));
    return instance;
  }
  KeyValueStorage(leveldb::DB *db) : level_db_(db) {
  }
  leveldb::DB *level_db_;
};

} // ns per
} // ns ara

#endif //__KEY_VALUE_STORAGE_H__