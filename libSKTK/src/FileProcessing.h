#pragma once

#include <boost\filesystem.hpp>

#include <string>
#include <fstream>
#include <vector>
#include <array>

// Typedef to represent a single Byte
using byte_t = unsigned char;

// Typedef to represent a Blob of Data
using blob_t = std::vector<byte_t>;

// Read any Type from a iStream into a corresponding Pointer-Target
template<typename T>
inline void read(std::istream& file, T* dst) {
	file.read(reinterpret_cast<char*>(dst), sizeof(T));
}

template<typename T>
inline T swap_endian(T u) {
	union {
		T value;
		std::array<uint8_t, sizeof(T)> byte;
	};

	value = u;
	std::reverse(byte.begin(), byte.end());

	return value;
}

// Read any Type from a iStream and return the Value by Copy
template<typename T>
inline T read(std::istream& file) {
	union {
		T buffer;
		char as_chars[sizeof(T)];
	};
	file.read(as_chars, sizeof(T));
	return buffer;
}

std::string archive_name(boost::filesystem::path file, bool full = false);

// Open a File in Binary-Mode to read its Contend
void open_to_read(std::ifstream& file, const boost::filesystem::path& filename);

// Open a File in Binary-Mode to write its Contend
void open_to_write(std::ofstream& file, const boost::filesystem::path& filename);

// Report a Message to the Standard Error Output and throw a Exception afterwards
void report_and_throw(const std::string& msg);