#pragma once

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

namespace util
{
  int getTerminalWidth();
}