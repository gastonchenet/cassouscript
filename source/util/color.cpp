#include "Color.hpp"

const std::string util::color::reset = "\033[0m";
const std::string util::color::black = "\033[30m";
const std::string util::color::red = "\033[31m";
const std::string util::color::green = "\033[32m";
const std::string util::color::yellow = "\033[33m";
const std::string util::color::blue = "\033[34m";
const std::string util::color::magenta = "\033[35m";
const std::string util::color::cyan = "\033[36m";
const std::string util::color::white = "\033[37m";
const std::string util::color::bold = "\033[1m";
const std::string util::color::underline = "\033[4m";
const std::string util::color::background_black = "\033[40m";
const std::string util::color::background_red = "\033[41m";
const std::string util::color::background_green = "\033[42m";
const std::string util::color::background_yellow = "\033[43m";
const std::string util::color::background_blue = "\033[44m";
const std::string util::color::background_magenta = "\033[45m";
const std::string util::color::background_cyan = "\033[46m";
const std::string util::color::background_white = "\033[47m";

std::string util::color::colorize(const std::string& text, const std::string& color)
{
  return color + text + reset;
}

std::string util::color::colorize(const std::string& text, const std::string& color, const std::string& background)
{
  return color + background + text + reset;
}