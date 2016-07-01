#pragma once

#include <inttypes.h>
#include <memory>
#include <vector>
#include <string>
#include <iosfwd>

#include "FileProcessing.h"
	
	struct Filename_Header {
		uint16_t unknown_0;
		uint16_t unknown_1;
		uint16_t unknown_2;
		uint16_t unknown_3;
	};

	template<>
	void read(std::ifstream& file, Filename_Header* dst);

	void readFilenames(std::ifstream& file, std::vector<std::string>& filenames_out);