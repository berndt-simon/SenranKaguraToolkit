#pragma once

#include <string>
#include <iosfwd>
#include <initializer_list>

#include <boost\filesystem.hpp>

#include "FileCommons.h"
#include "Filetype_CAT.h"

using boost::filesystem::path;

class ResourceDumper {
public:
	ResourceDumper()
		:_outputPrefix(), _outputPostfix() {
	};
	~ResourceDumper() = default;

	void dump(std::initializer_list<std::string> resource_name, blob_t data);
	void dump(std::initializer_list<std::string> resource_name, const char* data, uint32_t count);
	
	path& outputPrefix() {
		return _outputPrefix;
	}

	const path& outputPrefix() const {
		return _outputPrefix;
	}

	path& outputPostfix() {
		return _outputPostfix;
	}

	const path& outputPostfix() const {
		return _outputPostfix;
	}

private:
	path _outputPrefix;
	path _outputPostfix;
};
