#include "Filetype_CAT.h"

#include "FileProcessing.h"

#include <iostream>
#include <iomanip>

#include <assert.h>

std::string toString(CAT::ResourceEntry_t::Type_e type) {
	switch (type) {
		case CAT::ResourceEntry_t::GXT:
			return CAT::TYPE_KEY_GXT;
		case CAT::ResourceEntry_t::TMD:
			return CAT::TYPE_KEY_TMD;
		case CAT::ResourceEntry_t::TMD_TOON:
			return CAT::TYPE_KEY_TMD_TOON;
		default:
			return "UNDEFINED";
	}
}

namespace CAT {

	ResourceEntry_t::Type_e toType(const std::string& type) {
		if (TYPE_KEY_GXT.compare(type) == 0) {
			return ResourceEntry_t::Type_e::GXT;
		} else if (TYPE_KEY_TMD.compare(type) == 0) {
			return ResourceEntry_t::Type_e::TMD;
		} else if (TYPE_KEY_TMD_TOON.compare(type) == 0) {
			return ResourceEntry_t::Type_e::TMD_TOON;
		}
		std::cerr << "Undefined CAT-Resource-Type: " << type << std::endl;
		return ResourceEntry_t::Type_e::UNDEFINED;
	}



	void load(std::istream& file, std::vector<ResourceEntry_t>& entries, bool debug_out) {
		const std::streampos fileStart = file.tellg();

		// Read Header
		const uint32_t header_size = read<uint32_t>(file);
		ResourceEntry_t entry;
		do {
			read(file, &entry.offset);
			if (entry.offset != 0xFFFFFFFF) {
				entries.push_back(entry);
			} else {
				break;
			}
		} while (file.tellg() < header_size + fileStart);

		// Read FileTable Data
		file.seekg(header_size + fileStart, std::ios::beg);

		const auto entryItter(entries.begin());
		const auto entriesEnd(entries.end());

		const auto max_level = 3;
		std::array<std::string, 3> level_build;
		auto curr_level = 0;
		auto itterOffset = -1;
		char c;
		do {
			c = file.get();
			switch (c) {
				case ',':
					{
						int next_level = curr_level + 1;
						if (next_level >= max_level) {
							reportAndThrow("Next Level is unsupported!");
						}
						curr_level = next_level;
						break;
					}
				case 10:
					{
						if (!level_build[0].empty()) {
							// Absolute-Entry
							itterOffset++;
							if (itterOffset >= entries.size()) {
								std::cerr << "Entry out of scope" << std::endl;
								itterOffset--;
								assert((entryItter + itterOffset)->type == toType(level_build[0]));
							} else {
								(entryItter + itterOffset)->type = toType(level_build[0]);
							}
							assert(!level_build[1].empty());
							assert(!level_build[2].empty());
						} else {
							// Relative-Entry -> Subentry
							if (level_build[1].empty()) {
								level_build[1] = (entryItter + itterOffset)->sub_entries.back().package;
							}
						}
						ResourceEntry_t::SubEntry_t subEntry{ level_build[1], level_build[2] };
						(entryItter + itterOffset)->sub_entries.push_back(subEntry);

						// Reset State
						for (auto i(0); i < max_level; i++) {
							level_build[i].clear();
						}
						curr_level = 0;
						break;
					}
				default:
					{
						level_build[curr_level] += c;
						break;
					}
			}
		} while (file.tellg() < entries[0].offset + fileStart);

		if (debug_out) {
			std::cout << entries << std::endl;
		}		
	}

}

std::ostream& operator<<(std::ostream& out, const CAT::Header_t& header) {
	out << "Cat-Header:" << std::endl;
	out << "\tSize: " << std::dec << header.size << " bytes" << std::endl;
	for (auto offset(header.offsets.begin()); offset < header.offsets.end(); offset++) {
		out << "\tData-Offset: 0x" << std::hex << std::uppercase << *offset << std::endl;
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<CAT::ResourceEntry_t>& filetable) {
	out << "Cat-Entries:" << std::endl;
	for (auto res(filetable.begin()); res < filetable.end(); res++) {
		out << "\t" << toString(res->type) << "-Entry (0x" << std::hex << res->offset << "): " << std::endl;
		for (auto subres(res->sub_entries.begin()); subres < res->sub_entries.end(); subres++) {
			out << "\t\tSub-Entry: " << subres->package << "/" << subres->resource << std::endl;
		}
	}
	return out;
}
