#pragma once

#include <inttypes.h>
#include <vector>
#include <string>
#include <iosfwd>


	struct CAT_Header {
		// Size of the Header (bytes)
		int32_t					size;
		// Absolute Offsets which point to Chunks
		std::vector<int32_t>	chunk_offsets;
	};

	struct CAT_ResourceID {
		enum Level {
			Type, Package, Resource, MAX_LEVEL
		};
		std::string level[MAX_LEVEL];
		bool dirty[MAX_LEVEL];
	};

	typedef std::vector<CAT_ResourceID> CAT_FileTable;

	struct CAT_Chunk {
		// Size of the Chunk-Header (bytes)
		int32_t					size;
		// Count of Sub-Resources within this Chunk
		int32_t					resource_count;
		// Size of the Data-Block
		int32_t					segment_size;
		// Relative Offsets which point to Data
		std::vector<int32_t>	data_offsets;
	};

	std::string make_absolute(const CAT_ResourceID& resourcePath);

	std::ostream& operator<<(std::ostream& out, const CAT_Header& header);
	std::ostream& operator<<(std::ostream& out, const CAT_FileTable& intermediateData);
	std::ostream& operator<<(std::ostream& out, const CAT_Chunk& chunk);

	void processCAT(std::ifstream& file);



