#pragma once

#ifdef CE_WINDOWS
#define CE_BREAK __debugbreak()
#else
#include <signal.h>
#define CE_BREAK raise(SIGTRAP)
#endif

#if defined(CE_DEBUG) || defined(CE_RELEASE)
#define CE_ASSERT(condition) if(condition) {} else { CE_ERROR("Assetion failed: %s IN %s:%d", #condition, __FILE__, __LINE__); CE_BREAK; }
#else
#define CE_ASSERT(condition, message, ...)
#endif