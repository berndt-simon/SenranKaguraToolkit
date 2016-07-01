#pragma once
#include <inttypes.h>
#include <iosfwd>

#include "FileProcessing.h"
#include "Filetype_CAT.h"

struct TMD_Bounds {
	struct {
		float x;
		float y;
		float z;
	} min, max;
};

struct TMD_Range {
	uint32_t start;
	uint32_t count;
};

struct TMD_Range_Inv {
	uint32_t count;
	uint32_t start;
};

union FeatureLevel {
	uint16_t as_short;
	struct {
		uint8_t lsb;
		uint8_t msb;
	};
};

struct TMD_Header {
	uint32_t		magic_number;
	uint16_t		unknown_0;
	FeatureLevel	feature_level;
	uint16_t		unknowns[4];
	TMD_Bounds		bounds;
	TMD_Range		bvh_range;
	TMD_Range		unknown_range[4];
	TMD_Range		poly_group_range;
	TMD_Range		face_range;
	TMD_Range_Inv	vertex_range;
	uint32_t		unknown_offsets[7];
	TMD_Range		bone_range;
	TMD_Range		bone_hierarchy_range;
	TMD_Range		tex_info_range;
	uint32_t		blank;

	bool verify();
};

template<>
void read(std::istream& file, TMD_Range* dst);

template<>
void read(std::istream& file, TMD_Range_Inv* dst);

template<>
void read(std::istream& file, FeatureLevel* dst);

template<>
void read(std::istream& file, TMD_Bounds* dst);


template<>
void read(std::istream& file, TMD_Header* dst);


struct TMD_Data {
	struct Vertex_t {
		struct {
			float x, y, z;
		} pos;
		struct {
			uint8_t w0, w1, w2, w3;
		} weight;
		struct {
			uint8_t b0, b1, b2, b3;
		} bone;
		struct {
			int8_t x, y, z, q;
		} normal;
		struct {
			int16_t u, v;
		} tex;
	};
	struct Face_t {
		uint16_t f0, f1, f2;
	};

	std::vector<Vertex_t> vertices;
	std::vector<Face_t> faces;

};


void processTMD(std::istream& file, const std::streamoff tmd_start, const std::vector<CAT_Resource_Entry::Sub_Entry>& sub_entries, TMD_Data& data_out);

