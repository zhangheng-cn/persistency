#ifndef __PER_ERROR_DOMAIN_H__
#define __PER_ERROR_DOMAIN_H__

#include "ara/core/error_domain.h"
#include "ara/core/error_code.h"




namespace ara {
namespace per {
class PerErrorDomain;
extern PerErrorDomain global_per_errordomain;

class PerErrorDomain final : public ara::core::ErrorDomain {
public:
  const char* Name () const noexcept override {
    return "PER_ERROR";
  }
  const char* Message (CodeType errorCode) const noexcept override {
    switch((uint32_t)errorCode) {
      case 1:
        return "kStorageNotFoun";
        break;
      case 2:
        return "kKeyNotFoun";
        break;
      case 3:
        return "kIllegalWriteAcces";
        break;
      case 4:
        return "kPhysicalStorageFailur";
        break;
      case 5:
        return "kIntegrityCorrupte";
        break;
      case 6:
        return "kValidationFaile";
        break;
      case 7:
        return "kEncryptionFailed";
        break;
      case 8:
        return "kDataTypeMismatc";
        break;
      case 9:
        return "kInitValueNotAvailable";
        break;
      case 10:
        return "kResourceBusy";
        break;
      case 12:
        return "kOutOfStorageSpace";
        break;
      case 13:
        return "kFileNotFound";
        break;
      case 15:
        return "kInvalidPosition";
        break;
      case 16:
        return "kIsEof";
        break;
      case 17:
        return "kInvalidOpenMode";
        break;
      case 18:
        return "kInvalidSiz";
        break;
      default:
        return "kUnkonwn";
        break;
    }
    return "kUnkonwn";
  }
  PerErrorDomain () : ErrorDomain((IdType)1) {}
};



enum class PerErrc : ara::core::ErrorDomain::CodeType {
  kStorageNotFound= 1,        // KV Storage or File Storage not confiugred
  kKeyNotFound= 2,            // key not found in KV Storage
  kIllegalWriteAccess= 3,     // read only
  kPhysicalStorageFailure= 4, // e.g. corrupted file system or corrupted hardware
  kIntegrityCorrupted= 5,     // KV Storage structure or File Storage meta data corrupted.
  kValidationFailed= 6,       // the validation of redundancy measures failed 
  kEncryptionFailed= 7,       // the encryption or decryption failed
  kDataTypeMismatch= 8,       // data type does not match
  kInitValueNotAvailable= 9,  // no initial value is available
  kResourceBusy= 10,          // resource is currently busy
  kOutOfStorageSpace= 12,     // no space
  kFileNotFound= 13,          // filename not found
  kInvalidPosition= 15,       // SetPosition param is not reachable
  kIsEof= 16,                 // file end
  kInvalidOpenMode= 17,       // combination of OpenModes is invalid
  kInvalidSize= 18            // size bigger than the current size
};

constexpr const ara::core::ErrorDomain& GetPerDomain () noexcept {
  return global_per_errordomain;
}
// constexpr ara::core::ErrorCode MakeErrorCode (PerErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept;



} // ns per
} // ns ara





#endif // __PER_ERROR_DOMAIN_H__


