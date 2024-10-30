#include "terminal.hpp"

int util::getTerminalWidth()
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int columns;
  
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
  {
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
  }
#else
  struct winsize w;
  
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1)
  {
    return w.ws_col;
  }
#endif
  return -1;
}