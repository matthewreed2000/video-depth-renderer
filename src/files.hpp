#pragma once

#include <string>

namespace files {
	bool exists(const char* filename);
	std::string readText(const char* filename);
};