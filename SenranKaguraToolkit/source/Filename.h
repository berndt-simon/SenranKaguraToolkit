#pragma once

#include <inttypes.h>
#include <memory>
#include <vector>
#include <string>
#include <iosfwd>
	
	struct Filename_Header {
		uint16_t unknown_0;
		uint16_t unknown_1;
		uint16_t unknown_2;
		uint16_t unknown_3;
	};

	std::unique_ptr<std::vector<std::string>> readFilenames(std::ifstream& file);