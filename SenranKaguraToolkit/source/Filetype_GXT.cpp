#include "Filetype_GXT.h"

#include <fstream>
#include <assert.h>
#include <iterator>
#include <ios>

#include "FileProcessing.h"

void processGXT(std::ifstream& file, const std::streamoff gxt_start, const std::vector<CAT_Resource_Entry::Sub_Entry>& sub_entries, std::vector<GXT_Entry>& entries_out) {
	
	file.seekg(gxt_start, std::ios::beg);

	GXT_Header header{ 0, 0, 0 };
	read(file, &header.size);
	read(file, &header.resource_count);
	read(file, &header.container_size);

	// These Counts should match
	assert(header.resource_count == sub_entries.size());

	for (auto res(0); res < header.resource_count; res++) {
		header.offsets.push_back(read<uint32_t>(file));
	}


	file.seekg(header.size + gxt_start, std::ios::beg);
	const std::streamoff offset_base = file.tellg();
	const std::streamoff data_end = offset_base + header.container_size;
	for (auto res(0); res < header.resource_count; res++) {
		const std::streamoff resource_start = offset_base + header.offsets[res];
		std::streamoff resource_size = 0;
		if (res == header.resource_count - 1) {
			resource_size = data_end - resource_start;
		} else {
			resource_size = (offset_base + header.offsets[res + 1]) - resource_start;
		}
		GXT_Entry entry;
		entry.package = sub_entries[res].package;
		entry.resource = sub_entries[res].resource;

		file.seekg(resource_start, std::ios::beg);
		const std::streamoff curr_pos = file.tellg();
		static_assert(sizeof(char) == sizeof(byte_t), "Size Missmatch");
		entry.data.resize(resource_size);
		file.read(reinterpret_cast<char*>(entry.data.data()), resource_size);
		entries_out.push_back(entry);
	}


}
