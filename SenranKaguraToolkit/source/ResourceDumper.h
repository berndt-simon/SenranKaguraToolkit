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
		:_outputPrefix(), _outputPostfix() {
	};
	~ResourceDumper() = default;

	void dump(std::initializer_list<std::string> resource_name, blob_t data);
	void dump(std::initializer_list<std::string> resource_name, const char* data, uint32_t count);
	
	boost::filesystem::path& outputPrefix() {
		return _outputPrefix;
	}

	const boost::filesystem::path& outputPrefix() const {
		return _outputPrefix;
	}

	boost::filesystem::path& outputPostfix() {
		return _outputPostfix;
	}

	const boost::filesystem::path& outputPostfix() const {
		return _outputPostfix;
	}

private:
	boost::filesystem::path _outputPrefix;
	boost::filesystem::path _outputPostfix;
};
