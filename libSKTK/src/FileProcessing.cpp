#include "FileProcessing.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

void report_and_throw(const std::string& msg) {
	std::runtime_error error(msg);
	std::cerr << error.what() << std::endl;
	throw error;
}

std::string archive_name(boost::filesystem::path file, bool full) {
	std::string filename(file.stem().string());
	if (!full) {
		// Assume there is only one valid underscore within an archive name
		// If more than one underscore is present make that one the end of the sanatized part
		if (std::count(filename.begin(), filename.end(), '_') > 1) {
			const auto firstUnderscore(filename.find_first_of('_'));
			const auto secondUnderscore(filename.find_first_of('_', firstUnderscore + 1));
			filename.erase(secondUnderscore);
		}
	}
	return filename;
}

void open_to_read(std::ifstream& file, const boost::filesystem::path& filename) {
	file.open(filename.string(), std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		report_and_throw("Can't read/open '" + filename.string() + "'!");
	}
}

void open_to_write(std::ofstream& file, const boost::filesystem::path& filename) {
	const boost::filesystem::path parent(filename.parent_path());
	if (!boost::filesystem::exists(parent)) {
		boost::filesystem::create_directories(parent);
	}
	file.open(filename.string(), std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.is_open()) {
		report_and_throw("Can't create/open '" + filename.string() + "'!");
	}
}
