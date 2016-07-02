#pragma once

#include <string>
#include <fstream>
#include <vector>

// Typedef to represent a single Byte
typedef unsigned char byte_t;
static_assert(sizeof(byte_t) == 1, "A Byte_t isn't one Byte");

// Typedef to represent a Blob of Data
typedef std::vector<byte_t> blob_t;

// Read any Type from a iStream into a corresponding Pointer-Target
template<typename T>
inline void read(std::istream& file, T* dst) {
	file.read(reinterpret_cast<char*>(dst), sizeof(T));
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

// Open a File in Binary-Mode to read its Contend
void openToRead(std::ifstream& file, const std::string& filename);

// Open a File in Binary-Mode to write its Contend
void openToWrite(std::ofstream& file, const std::string& filename);

// Report a Message to the Standard Error Output and throw a Exception afterwards
void reportAndThrow(const std::string& msg);