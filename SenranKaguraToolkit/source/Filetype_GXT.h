#pragma once

#include <inttypes.h>
#include <vector>
#include <iosfwd>

#include "FileCommons.h"
#include "Filetype_CAT.h"

struct GXT_Header {
	// Size of the Header (bytes)
	uint32_t					size;
	uint32_t					resource_count;
	uint32_t					container_size;
	// Absolute Offsets which point to Chunks
	std::vector<uint32_t>		offsets;
};


struct GXT_Entry {
	std::string package;
	std::string resource;

	blob_t data;
};

void processGXT(std::ifstream& file, const std::streamoff gxt_start, const std::vector<CAT_Resource_Entry::Sub_Entry>& sub_entries, std::vector<GXT_Entry>& entries_out);