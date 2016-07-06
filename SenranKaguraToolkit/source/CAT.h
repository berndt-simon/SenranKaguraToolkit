#pragma once

#include <inttypes.h>
#include <vector>
#include <array>
#include <string>
#include <iosfwd>
#include <set>

#include "ElementType.h"

namespace CAT {
	
	// Variable Length Header of a CAT-File
	struct Header_t {
		Header_t();

		uint32_t				size;
		std::vector<uint32_t>	offsets;
	};

	// A CAT-Resource Entry
	struct ResourceEntry_t {
		struct SubEntry_t {
			std::string package;
			std::string resource;
		};

		ResourceEntry_t();

		ElementType_e type;
		uint32_t offset;
		std::vector<SubEntry_t> sub_entries;

	};

	// Read the Contents of a CAT-File
	// Store Descriptors of all found Entries into a given Vector
	// archive_name is meant to be the name of the file which corresponds to the given istream - just the filename without the extension
	void load(const std::string& archive_name, std::istream& file, std::vector<ResourceEntry_t>& entries, bool debug_out = false);

	void sanatize_archive_name(const std::string& in, std::string& out);

}

std::ostream& operator<<(std::ostream& out, const std::vector<CAT::ResourceEntry_t>& entries);
std::ostream& operator<<(std::ostream& out, const CAT::Header_t& header);




