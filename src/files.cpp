#include "files.hpp"

#include <fstream>
#include <sstream>
#include <unistd.h>

bool files::exists(const char* filename) { // [1]
	return ( access( filename, F_OK ) != -1 );
}

std::string files::readText(const char* filename) {
	std::fstream f(filename);

	if (!f.good())
	{
		printf("Failed to open file %s\n", filename);
	}

	std::string line;
	std::stringstream ss;

	while (getline(f, line)) {
		ss << line << '\n';
	}

	return ss.str();
}

// [1] https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-14-17-c