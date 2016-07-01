#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

typedef unsigned char byte_t;

typedef std::vector<byte_t> blob_t;

inline void reportAndThrow(const std::string& msg) {
	std::runtime_error error(msg);
	std::cerr << error.what() << std::endl;
	throw error;
}