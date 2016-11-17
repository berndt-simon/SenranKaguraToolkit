#include "ResourceDumper.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <iterator>

#include "FileProcessing.h"

ResourceDumper::ResourceDumper()
	: _output_prefix()
	, _output_suffix() {
}

boost::filesystem::path& ResourceDumper::output_prefix() {
	return _output_prefix;
}
const boost::filesystem::path& ResourceDumper::output_prefix() const {
	return _output_prefix;
}
boost::filesystem::path& ResourceDumper::output_suffix() {
	return _output_suffix;
}
const boost::filesystem::path& ResourceDumper::output_suffix() const {
	return _output_suffix;
}

void ResourceDumper::dump(std::initializer_list<std::string> resource_name, void* data, size_t count) {
	boost::filesystem::path fullPath = _output_prefix;
	for (const auto& res_sub_name : resource_name) {
		fullPath /= res_sub_name;
	}	
	fullPath += _output_suffix;

	boost::filesystem::path parent(fullPath.parent_path());
	if (!boost::filesystem::exists(parent)) {
		boost::filesystem::create_directories(parent);
	}

	std::ofstream destination;
	open_to_write(destination, fullPath.string());

	destination.write(static_cast<const char*>(data), count);

	destination.flush();
	destination.close();
}

void ResourceDumper::dump(std::initializer_list<std::string> resource_name, blob_t data) {
	dump(resource_name, reinterpret_cast<char*>(data.data()), data.size() * sizeof(byte_t));
}




