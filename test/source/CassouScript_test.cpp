#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "CassouScript" ? 0 : 1;
}
