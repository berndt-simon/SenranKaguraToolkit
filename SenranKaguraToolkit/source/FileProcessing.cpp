#include "FileProcessing.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

void reportAndThrow(const std::string& msg) {
	std::runtime_error error(msg);
	std::cerr << error.what() << std::endl;
	throw error;
}

void openToRead(std::ifstream& file, const boost::filesystem::path& filename) {
	file.open(filename.string(), std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		reportAndThrow("Can't read/open '" + filename.string() + "'!");
	}
}

void openToWrite(std::ofstream& file, const boost::filesystem::path& filename) {
	const boost::filesystem::path parent(filename.parent_path());
	if (!boost::filesystem::exists(parent)) {
		boost::filesystem::create_directories(parent);
	}
	file.open(filename.string(), std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.is_open()) {
		reportAndThrow("Can't create/open '" + filename.string() + "'!");
	}
}
