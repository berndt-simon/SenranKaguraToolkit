#pragma once

#include <string>
#include <iosfwd>
#include <initializer_list>

#include <boost\filesystem.hpp>

#include "FileProcessing.h"
#include "Filetype_CAT.h"


class ResourceDumper {
public:
	ResourceDumper();
	~ResourceDumper() = default;

	void dump(std::initializer_list<std::string> resource_name, blob_t data);
	void dump(std::initializer_list<std::string> resource_name, const char* data, uint32_t count);
	
	boost::filesystem::path& output_prefix();
	boost::filesystem::path& output_suffix();

private:
	boost::filesystem::path _output_prefix;
	boost::filesystem::path _output_suffix;
};
