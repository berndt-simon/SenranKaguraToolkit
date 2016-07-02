#pragma once

#include <string>
#include <iosfwd>
#include <initializer_list>

#include <boost\filesystem.hpp>

#include "FileProcessing.h"
#include "Filetype_CAT.h"


class ResourceDumper {
public:
	ResourceDumper()
		:_output_prefix(), _output_postfix() {
	};
	~ResourceDumper() = default;

	void dump(std::initializer_list<std::string> resource_name, blob_t data);
	void dump(std::initializer_list<std::string> resource_name, const char* data, uint32_t count);
	
	boost::filesystem::path& outputPrefix() {
		return _output_prefix;
	}

	const boost::filesystem::path& outputPrefix() const {
		return _output_prefix;
	}

	boost::filesystem::path& outputPostfix() {
		return _output_postfix;
	}

	const boost::filesystem::path& outputPostfix() const {
		return _output_postfix;
	}

private:
	boost::filesystem::path _output_prefix;
	boost::filesystem::path _output_postfix;
};
