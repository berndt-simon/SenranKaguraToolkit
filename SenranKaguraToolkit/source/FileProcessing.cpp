#include "FileProcessing.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

void reportAndThrow(const std::string& msg) {
	std::runtime_error error(msg);
	std::cerr << error.what() << std::endl;
	throw error;
}

void open(std::ifstream& file, const std::string& filename) {
	file.open(filename, std::ios::in || std::ios::binary);
	if (!file.is_open()) {
		reportAndThrow("Can't open '" + filename + "'!");
	}
}
