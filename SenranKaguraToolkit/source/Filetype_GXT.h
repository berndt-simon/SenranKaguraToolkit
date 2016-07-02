#pragma once

#include <inttypes.h>
#include <vector>
#include <iosfwd>

#include "FileProcessing.h"
#include "Filetype_CAT.h"

// Varibale Length Header of a GXT-File
struct GXT_Header {
	uint32_t					size;
	uint32_t					resource_count;
	uint32_t					container_size;
	std::vector<uint32_t>		offsets;

	GXT_Header()
		: size(0U), resource_count(0U), container_size(0U), offsets() {
	}
};


// A GXT-Resource Entry
struct GXT_Entry {
	std::string package;
	std::string resource;

	blob_t data;

	GXT_Entry()
		:package(), resource(), data() {
	}
};

// Read the Contents of a GXT-File
// Stores the Content of all found Entries into a given Vector
void processGXT(std::istream& file, const std::streamoff gxt_start, const std::vector<CAT_Resource_Entry::Sub_Entry>& sub_entries, std::vector<GXT_Entry>& entries_out);