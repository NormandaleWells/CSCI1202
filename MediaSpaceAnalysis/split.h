
#pragma once

#include <string>
#include <vector>

// Split a line of data by whitespace.
std::vector<std::string> split(const std::string & s) noexcept;

// Split a line of data by the given separator character.
std::vector<std::string> split(const std::string & s, char c) noexcept;
