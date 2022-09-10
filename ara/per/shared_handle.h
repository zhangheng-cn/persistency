#ifndef __SHARED_HANDLE_H__
#define __SHARED_HANDLE_H__

#include <memory>
namespace ara {
namespace per { 

template <class T>
using SharedHandle = std::shared_ptr<T>;

} // ns ara
} // ns per

#endif //__SHARED_HANDLE_H__