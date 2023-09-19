#pragma once

#include <memory>

namespace CeltEngine
{
    using byte = unsigned char;

    using uint = unsigned int;
    using uint32 = unsigned int;
    using uint16 = unsigned short;
    using uint8 = unsigned char;

    using string = std::string;
    using wstring = std::wstring;
    
    template<typename T>
    using Shared = std::shared_ptr<T>;

    template<typename T>
    using Unique = std::unique_ptr<T>;
}
