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

// Variable Length Header of a CAT-File
struct CAT_Header {
	uint32_t				size;
	std::vector<uint32_t>	offsets;
};

// A CAT-Resource Entry
struct CAT_Resource_Entry {
	enum Type {
		GXT, TMD, TMD_TOON, UNDEFINED
	};

	struct Sub_Entry {
		std::string package;
		std::string resource;
	};

	Type type;
	uint32_t offset;
	std::vector<Sub_Entry> sub_entries;
};

// Convert a CAT-Resource-Type to the corresponding String-Representation
std::string toString(CAT_Resource_Entry::Type type);
// Convert a CAT-Resource-Type from the corresponding String-Representation
CAT_Resource_Entry::Type toType(const std::string& type);


std::ostream& operator<<(std::ostream& out, const std::vector<CAT_Resource_Entry>& entries);
std::ostream& operator<<(std::ostream& out, const CAT_Header& header);

// Read the Contents of a CAT-File
// Store Descriptors of all found Entries into a given Vector
void processCAT(std::istream& file, std::vector<CAT_Resource_Entry>& cat_entries);



