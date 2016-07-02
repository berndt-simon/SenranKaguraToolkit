#pragma once

#include <inttypes.h>
#include <vector>
#include <iosfwd>

#include "FileProcessing.h"
#include "Filetype_CAT.h"

namespace GXT {
	
	// Varibale Length Header of a GXT-File
	struct Header_t {
		uint32_t					size;
		uint32_t					resource_count;
		uint32_t					container_size;
		std::vector<uint32_t>		offsets;

		Header_t();
	};


	// A GXT-Resource Entry
	struct Entry_t {
		std::string package;
		std::string resource;
		blob_t data;

		Entry_t();
	};

	// Read the Contents of a GXT-File
	// Stores the Content of all found Entries into a given Vector
	void load(std::istream& file, const std::streamoff gxt_start, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& sub_entries, std::vector<Entry_t>& entries_out);
}
