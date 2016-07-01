#include "ResourceDumper.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <iterator>

#include "FileCommons.h"

void openToWrite(std::ofstream& file, const std::string& filename) {
	file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.is_open()) {
		reportAndThrow("Can't create/open '" + filename + "'!");
	}
}

void ResourceDumper::dump(std::initializer_list<std::string> resource_name, const char* data, uint32_t count) {
	path fullPath = _outputPrefix;
	for (auto res_name_itt(resource_name.begin()); res_name_itt != resource_name.end(); res_name_itt++) {
		fullPath.concat(*res_name_itt);
	}	
	fullPath += _outputPostfix;

	boost::filesystem::create_directories(fullPath.parent_path());

	std::ofstream destination;
	openToWrite(destination, fullPath.string());

	destination.write(data, count);

	destination.flush();
	destination.close();
}

void ResourceDumper::dump(std::initializer_list<std::string> resource_name, blob_t data) {
	dump(resource_name, reinterpret_cast<char*>(&data[0]), data.size() * sizeof(byte_t));
}




