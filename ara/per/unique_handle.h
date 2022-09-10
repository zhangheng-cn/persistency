#ifndef __UNIQUE_HANDLE_H__
#define __UNIQUE_HANDLE_H__

#include <memory>
namespace ara {
namespace per { 

template <class T>
using UniqueHandle = std::unique_ptr<T>;

} // ns ara
} // ns per

#endif // __UNIQUE_HANDLE_H__