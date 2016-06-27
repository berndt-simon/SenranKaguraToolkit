#include "Filetype_CAT.h"

#include "FileProcessing.h"
#include <fstream>

#include <iostream>
#include <iomanip>


std::string make_absolute(const CAT_ResourceID& resource) {
	std::string path(resource.level[0]);
	for (auto i = 1; i < CAT_ResourceID::MAX_LEVEL; i++) {
		path += '/';
		path += resource.level[i];
	}
	return path;
}

std::ostream& operator<<(std::ostream& out, const CAT_Header& header) {
	out << "Cat-Header:" << std::endl;
	out << "\tSize: " << std::dec << header.size << " bytes" << std::endl;
	for (auto offset(header.chunk_offsets.begin()); offset < header.chunk_offsets.end(); offset++) {
		out << "\tChunk-Offset: 0x" << std::hex << std::uppercase << *offset << std::endl;
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const CAT_FileTable& filetable) {
	out << "Cat-Filetable:" << std::endl;
	for (auto res(filetable.begin()); res < filetable.end(); res++) {
		out << "\tResource: " << make_absolute(*res) << std::endl;
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const CAT_Chunk& chunk) {
	out << "Cat-Chunk:" << std::endl;
	out << "\tSize: " << std::dec << chunk.size << " bytes" << std::endl;
	out << "\tResource-Count: " << chunk.resource_count << std::endl;
	out << "\tSegment-Size: " << chunk.segment_size << " bytes" << std::endl;
	for (auto offset(chunk.data_offsets.begin()); offset < chunk.data_offsets.end(); offset++) {
		out << "\tData-Offset: 0x" << std::hex << std::uppercase << *offset << std::endl;
	}
	return out;
}


void processCAT(std::ifstream& file) {
	const std::streampos fileStart = file.tellg();

	// Read Header
	CAT_Header header;
	read(file, &header.size);
	int32_t chunk_offset;
	do {
		read(file, &chunk_offset);
		if (chunk_offset != -1) {
			header.chunk_offsets.push_back(chunk_offset);
		} else {
			break;
		}
	} while (file.tellg() < header.size + fileStart);
	std::cout << header;

	// Read FileTable Data
	CAT_FileTable fileTable;
	file.seekg(header.size + fileStart, std::ios::beg);
	CAT_ResourceID resource_id;
	CAT_ResourceID::Level level = CAT_ResourceID::Type;
	char c;
	do {
		c = file.get();
		switch (c) {
			case ',':
			{
				int next_level = level + 1;
				if (next_level >= CAT_ResourceID::MAX_LEVEL) {
					reportAndThrow("Next Level is unsupported!");
				}
				level = static_cast<CAT_ResourceID::Level>(next_level);
				break;
			}
			case 10:
			{
				fileTable.push_back(resource_id);
				for (auto i = 0; i < CAT_ResourceID::MAX_LEVEL; i++) {
					resource_id.dirty[i] = true;
				}
				level = CAT_ResourceID::Type;
				break;
			}
			default:
			{
				if (resource_id.dirty[level]) {
					resource_id.dirty[level] = false;
					resource_id.level[level].clear();
				}
				resource_id.level[level] += c;
				break;
			}
		}
	} while (file.tellg() < header.chunk_offsets[0] + fileStart);
	std::cout << fileTable << std::endl;

	// Read Chunks-Infos
	std::vector<CAT_Chunk> chunks;
	CAT_Chunk chunk;
	for (auto offset(header.chunk_offsets.begin()); offset != header.chunk_offsets.end(); offset++) {
		file.seekg(*offset + fileStart, std::ios::beg);
		read(file, &chunk.size);
		read(file, &chunk.resource_count);
		read(file, &chunk.segment_size);
		chunk.data_offsets.clear();
		int32_t data_offset;
		do {
			read(file, &data_offset);
			chunk.data_offsets.push_back(data_offset);
		} while (file.tellg() < *offset + chunk.size + fileStart);
		std::cout << chunk << std::endl;
		chunks.push_back(chunk);
	}

	std::cout << std::endl << std::endl;

	int resCnt = 0;
	int cnkCnt = 0;
	for (auto chunk(chunks.begin()); chunk != chunks.end(); chunk++) {
		const int chunkEnd = header.chunk_offsets[cnkCnt] + (*chunk).size;
		for (auto offset((*chunk).data_offsets.begin()); offset != (*chunk).data_offsets.end(); offset++) {
			std::cout << "Resource: " << make_absolute(fileTable[resCnt]);
			std::cout << " at 0x" << std::hex << std::uppercase << chunkEnd + *offset << std::endl;
			file.seekg(chunkEnd + *offset + fileStart);
			resCnt++;
		}
		cnkCnt++;
	}
}
