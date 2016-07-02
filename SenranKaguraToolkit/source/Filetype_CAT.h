#pragma once

#include <inttypes.h>
#include <vector>
#include <array>
#include <string>
#include <iosfwd>
#include <set>


namespace CAT {

	const std::string TYPE_KEY_GXT = "GXT";
	const std::string TYPE_KEY_TMD = "TMD";
	const std::string TYPE_KEY_TMD_TOON = "TMD_TOON";

	// Variable Length Header of a CAT-File
	struct Header_t {
		uint32_t				size;
		std::vector<uint32_t>	offsets;
	};

	// A CAT-Resource Entry
	struct ResourceEntry_t {
		enum Type_e {
			GXT, TMD, TMD_TOON, UNDEFINED
		};

		struct SubEntry_t {
			std::string package;
			std::string resource;
		};

		Type_e type;
		uint32_t offset;
		std::vector<SubEntry_t> sub_entries;
	};

	// Convert a CAT-Resource-Type from the corresponding String-Representation
	ResourceEntry_t::Type_e toType(const std::string& type);


	// Read the Contents of a CAT-File
	// Store Descriptors of all found Entries into a given Vector
	void load(std::istream& file, std::vector<ResourceEntry_t>& entries, bool debug_out = false);
}


// Convert a CAT-Resource-Type to the corresponding String-Representation
std::string toString(CAT::ResourceEntry_t::Type_e type);

std::ostream& operator<<(std::ostream& out, const std::vector<CAT::ResourceEntry_t>& entries);
std::ostream& operator<<(std::ostream& out, const CAT::Header_t& header);




