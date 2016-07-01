#include "FileProcessing.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

void reportAndThrow(const std::string& msg) {
	std::runtime_error error(msg);
	std::cerr << error.what() << std::endl;
	throw error;
}

void openToRead(std::ifstream& file, const std::string& filename) {
	file.open(filename, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		reportAndThrow("Can't read/open '" + filename + "'!");
	}
}

void openToWrite(std::ofstream& file, const std::string& filename) {
	file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.is_open()) {
		reportAndThrow("Can't create/open '" + filename + "'!");
	}
}
