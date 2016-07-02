#include "ResourceDumper.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <iterator>

#include "FileProcessing.h"


void ResourceDumper::dump(std::initializer_list<std::string> resource_name, const char* data, uint32_t count) {
	boost::filesystem::path fullPath = _output_prefix;
	for (auto res_name_itt(resource_name.begin()); res_name_itt != resource_name.end(); res_name_itt++) {
		fullPath.concat(*res_name_itt);
	}	
	fullPath += _output_postfix;

	boost::filesystem::create_directories(fullPath.parent_path());

	std::ofstream destination;
	openToWrite(destination, fullPath.string());

	destination.write(data, count);

	destination.flush();
	destination.close();
}

void ResourceDumper::dump(std::initializer_list<std::string> resource_name, blob_t data) {
	dump(resource_name, reinterpret_cast<char*>(data.data()), data.size() * sizeof(byte_t));
}




