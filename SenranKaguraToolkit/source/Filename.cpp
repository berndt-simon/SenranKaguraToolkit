#include "Filename.h"

#include "FileProcessing.h"
#include <fstream>

std::unique_ptr<std::vector<std::string>> readFilenames(std::ifstream& file) {
	std::unique_ptr<std::vector<std::string>> filenames = std::make_unique<std::vector<std::string>>();
	Filename_Header header;
	read(file, &header);

	return filenames;
}