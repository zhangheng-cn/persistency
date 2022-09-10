#ifndef __ARA_CORE_H__
#define __ARA_CORE_H__

#include <vector>
#include <string>
#include <string_view>

// #include <span>

namespace ara {
namespace core {
template<class T>
using Vector = std::vector<T>;
using String = std::string;
using StringView = std::string;
// using Span = std::span;
using Byte = unsigned char;
}
}

#endif // __ARA_CORE_H__