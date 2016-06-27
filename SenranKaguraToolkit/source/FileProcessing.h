#pragma once

#include <string>
#include <fstream>

template<typename T>
void read(std::ifstream& file, T* dst) {
	file.read(reinterpret_cast<char*>(dst), sizeof(T));
}

template<typename T>
T read(std::ifstream& file) {
	union {
		T buffer;
		char asChars[sizeof(T)];
	};
	file.read(asChars, sizeof(T));
	return buffer;
}

void open(std::ifstream& file, const std::string& filename);

void reportAndThrow(const std::string& msg);