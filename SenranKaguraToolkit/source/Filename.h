#pragma once

#include <inttypes.h>
#include <vector>
#include <string>
#include <iosfwd>

#include "FileProcessing.h"

// Fixed Length Header of the filename.bin File
struct Filename_Header {
	uint16_t unknown_0;
	uint16_t unknown_1;
	uint16_t unknown_2;
	uint16_t unknown_3;
};

// Template Specialization for the Filename_Header Type
template<>
void read(std::istream& file, Filename_Header* dst);

// Read the Contents of the filename.bin into a Vector of Strings
void readFilenames(std::istream& file, std::vector<std::string>& filenames_out);