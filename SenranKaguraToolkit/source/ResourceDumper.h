#pragma once

#include <string>
#include <iosfwd>
#include <initializer_list>

#include <boost\filesystem.hpp>

#include "FileProcessing.h"

class ResourceDumper {
public:
	ResourceDumper();
	~ResourceDumper() = default;
	
	boost::filesystem::path& output_prefix();
	const boost::filesystem::path& output_prefix() const;

	boost::filesystem::path& output_suffix();
	const boost::filesystem::path& output_suffix() const;

	void dump(std::initializer_list<std::string> resource_name, void* data, size_t count);
	void dump(std::initializer_list<std::string> resource_name, blob_t data);

private:
	boost::filesystem::path _output_prefix;
	boost::filesystem::path _output_suffix;
};
