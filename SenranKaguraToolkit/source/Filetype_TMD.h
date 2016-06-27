#pragma once
#include <inttypes.h>
#include <iosfwd>


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

struct TMD_Header {
	uint32_t	magic_number;
	uint16_t	unknown_0;
	uint8_t		vertex_format[2];
	uint16_t	unknown_1;
	uint16_t	unknown_2;
	uint16_t	unknown_3;
	uint16_t	unknown_4;
	TMD_Bounds	bounds;
	TMD_Range	unknown_range_0;
	TMD_Range	commands;
	TMD_Range	unknown_range_1;
	TMD_Range	rig;
	TMD_Range	shader;
	TMD_Range	poly_group;
	TMD_Range	face;
	TMD_Range	vertex;
	uint32_t	unknown_offsets[7];
	TMD_Range	bone;
	TMD_Range	bone_hierarchy;
	TMD_Range	tex_info;
	uint32_t	blank;
};


void processTMD(std::ifstream& file);

