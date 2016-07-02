#include "Filetype_GXT.h"

#include <assert.h>
#include <iterator>
#include <ios>

#include "FileProcessing.h"


namespace GXT {
	void load(std::istream& file, const std::streamoff gxt_start, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& sub_entries, std::vector<Entry_t>& entries) {
		file.seekg(gxt_start, std::ios::beg);

		// Read Header
		Header_t header;
		read(file, &header.size);
		read(file, &header.resource_count);
		read(file, &header.container_size);

		// These Counts should match
		assert(header.resource_count == sub_entries.size());

		// Read Offsets
		for (auto res(0U); res < header.resource_count; res++) {
			header.offsets.push_back(read<uint32_t>(file));
		}

		// Read Data into Container
		file.seekg(header.size + gxt_start, std::ios::beg);
		const std::streamoff offset_base = file.tellg();
		const std::streamoff data_end = offset_base + header.container_size;
		for (auto res(0U); res < header.resource_count; res++) {
			const std::streamoff resource_start = offset_base + header.offsets[res];
			std::streamoff resource_size = 0;
			if (res == header.resource_count - 1) {
				resource_size = data_end - resource_start;
			} else {
				resource_size = (offset_base + header.offsets[res + 1]) - resource_start;
			}
			Entry_t entry;
			entry.package = sub_entries[res].package;
			entry.resource = sub_entries[res].resource;

			file.seekg(resource_start, std::ios::beg);
			const std::streamoff curr_pos = file.tellg();
			static_assert(sizeof(char) == sizeof(byte_t), "Size Missmatch");
			entry.data.resize(resource_size);
			file.read(reinterpret_cast<char*>(entry.data.data()), resource_size);
			entries.push_back(entry);
		}


	}

	Header_t::Header_t() 
		: size(0U)
		, resource_count(0U)
		, container_size(0U)
		, offsets() {
	}

	Entry_t::Entry_t()
		: package()
		, resource()
		, data() {
	}

}
