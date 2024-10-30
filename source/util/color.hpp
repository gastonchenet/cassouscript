#pragma once

#include <string>

namespace util
{
  struct color
  {
    static const std::string reset;
    static const std::string black;
    static const std::string red;
    static const std::string green;
    static const std::string yellow;
    static const std::string blue;
    static const std::string magenta;
    static const std::string cyan;
    static const std::string white;
    static const std::string bold;
    static const std::string underline;
    static const std::string background_black;
    static const std::string background_red;
    static const std::string background_green;
    static const std::string background_yellow;
    static const std::string background_blue;
    static const std::string background_magenta;
    static const std::string background_cyan;
    static const std::string background_white;

    static std::string colorize(const std::string& text, const std::string& color);
    static std::string colorize(const std::string& text, const std::string& color, const std::string& background);
  };
}