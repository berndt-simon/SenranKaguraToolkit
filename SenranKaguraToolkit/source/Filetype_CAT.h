#pragma once

#include <inttypes.h>
#include <vector>
#include <array>
#include <string>
#include <iosfwd>
#include <set>


const std::string TYPE_KEY_GXT = "GXT";
const std::string TYPE_KEY_TMD = "TMD";
const std::string TYPE_KEY_TMD_TOON = "TMD_TOON";

struct CAT_Header {
	// Size of the Header (bytes)
	uint32_t				size;
	// Absolute Offsets which point to Chunks
	std::vector<uint32_t>	offsets;
};

struct CAT_Resource_Entry {
	enum Type {
		GXT, TMD, TMD_TOON, UNDEFINED
	};


	struct Sub_Entry {
		std::string package;
		std::string resource;
	};

	Type type = UNDEFINED;
	uint32_t offset = 0;
	std::vector<Sub_Entry> sub_entries;
};

std::string toString(CAT_Resource_Entry::Type type);
CAT_Resource_Entry::Type toType(const std::string& type);

std::ostream& operator<<(std::ostream& out, const std::vector<CAT_Resource_Entry>& entries);
std::ostream& operator<<(std::ostream& out, const CAT_Header& header);

void processCAT(std::ifstream& file, std::vector<CAT_Resource_Entry>& cat_entries);



