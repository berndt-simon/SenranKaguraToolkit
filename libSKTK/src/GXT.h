#pragma once

#include <inttypes.h>
#include <vector>
#include <iosfwd>

#include "FileProcessing.h"
#include "CAT.h"

namespace GXT {
	
	// Varibale Length Header of a GXT-File
	struct Header_t {
		uint32_t					size;
		uint32_t					resource_count;
		uint32_t					container_size;
		std::vector<uint32_t>		offsets;
	};


	// A GXT-Resource Entry
	struct Entry_t {
		std::string package;
		std::string resource;
		blob_t data;
	};

	std::string to_string(const Entry_t& entry);

	// Read the Contents of a GXT-File
	// Stores the Content of all found Entries into a given Vector
	void load(std::istream& file, const std::streamoff gxt_start, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& sub_entries, std::vector<Entry_t>& entries_out);

	void load_raw(std::istream& file, const std::streamoff gxt_start, std::vector<blob_t>& entries_out);
}
