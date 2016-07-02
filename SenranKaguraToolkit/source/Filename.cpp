#include "Filename.h"

#include "FileProcessing.h"
#include <fstream>
#include <string>

namespace Filename {
	void load(std::istream& file, std::vector<std::string>& filenames_out) {
		Header_t header;
		read(file, &header);
		const std::streamoff offset_base = file.tellg();
		std::vector<uint32_t> offsets;
		do {
			offsets.push_back(read<uint32_t>(file));
		} while (file.tellg() < offset_base + offsets.front());

		for (auto oItt(offsets.begin()); oItt != offsets.end(); oItt++) {
			file.seekg(offset_base + (*oItt), std::ios::beg);
			std::string filename;
			std::getline(file, filename, static_cast<char>(0));
			filenames_out.push_back(filename);
		}

	}
}


template<>
void read(std::istream& file, Filename::Header_t* dst) {
	read(file, &(dst->unknown_0));
	read(file, &(dst->unknown_1));
	read(file, &(dst->unknown_2));
	read(file, &(dst->unknown_3));
}