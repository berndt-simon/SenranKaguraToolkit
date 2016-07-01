#include "FileProcessing.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "FileCommons.h"


void openToRead(std::ifstream& file, const std::string& filename) {
	file.open(filename, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		reportAndThrow("Can't open '" + filename + "'!");
	}
}
