#pragma once
#include <inttypes.h>
#include <iosfwd>

#include "FileProcessing.h"

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

union FeatureLevel {
	uint16_t as_short;
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
	TMD_Range		vertex_range;
	uint32_t		unknown_offsets[7];
	TMD_Range		bone_range;
	TMD_Range		bone_hierarchy_range;
	TMD_Range		tex_info_range;
	uint32_t		blank;
};

template<>
void read(std::ifstream& file, TMD_Range* dst);

template<>
void read(std::ifstream& file, FeatureLevel* dst);

template<>
void read(std::ifstream& file, TMD_Bounds* dst);


template<>
void read(std::ifstream& file, TMD_Header* dst);



void processTMD(std::ifstream& file);

