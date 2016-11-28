#pragma once

#include <inttypes.h>
#include <vector>
#include <string>
#include <iosfwd>

#include "FileProcessing.h"

namespace Filename {
	// Fixed Length Header of the filename.bin File
	struct Header_t {
		uint16_t unknown_0;
		uint16_t unknown_1;
		uint16_t unknown_2;
		uint16_t unknown_3;
	};

	// Read the Contents of the filename.bin into a Vector of Strings
	void load(std::istream& file, std::vector<std::string>& filenames);
}


// Template Specialization for the Filename_Header Type
template<>
void read(std::istream& file, Filename::Header_t* dst);
