#pragma once

#ifdef CE_WINDOWS
#define CE_BREAK __debugbreak()
#else
#include <signal.h>
#define CE_BREAK raise(SIGTRAP)
#endif

