#pragma once
#include <inttypes.h>
#include <iosfwd>
#include <array>

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
	uint32_t					magic_number;
	uint16_t					unknown_0;
	FeatureLevel				feature_level;
	std::array<uint16_t, 4>		unknowns;
	TMD_Bounds					bounds;
	TMD_Range					bvh_range;
	TMD_Range					operation_range;
	TMD_Range					unknown_range;
	TMD_Range					rig_range;
	TMD_Range					shader_range;
	TMD_Range					poly_group_range;
	TMD_Range					face_range;
	TMD_Range_Inv				vertex_range;
	std::array<uint32_t, 7>		unknown_offsets;
	TMD_Range					bone_range;
	TMD_Range					bone_hierarchy_range;
	TMD_Range					tex_info_range;
	uint32_t					blank;

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


struct TMD_RAW_Data {
	struct Vertex_t {
		std::array<float, 3> pos;
		std::array<uint8_t, 4> weight;
		std::array<uint8_t, 4> bone;
		std::array<int8_t, 4> normal;
		std::array<uint8_t, 4> color;
		std::array<int16_t, 2> tex;

		Vertex_t();
	};

	struct Face_t {
		std::array<uint16_t, 3> vertex_index;

		Face_t();
	};

	struct PolyGroup_t {
		uint16_t count;
		uint16_t unknown;
		uint32_t offset;

		PolyGroup_t();
	};

	struct Operation_t {
		uint8_t value;
		uint8_t type;

		Operation_t();
	};

	struct BVH_t {
		TMD_Bounds bounds;
		uint32_t unknown_0;
		uint32_t unknown_1;

		BVH_t();
	};

	struct Tex_t {
		uint32_t id;
		uint32_t blank;
		uint32_t unknown;
		std::array<int16_t, 2> uv;

		Tex_t();
	};

	struct Rig_t {
		static const int32_t EMPTY_BONE = 0xFFFFFFFF;

		int32_t count;
		std::array<int32_t, 32> bone;

		Rig_t();
	};

	struct Bone_t {
		std::array<std::array<float, 4>, 4> mat;

		Bone_t();
	};

	struct BoneHierarchy_t {
		uint32_t hash;
		std::array<float, 3> head_pos;
		int32_t parent;
		uint32_t head;

		BoneHierarchy_t();
	};

	std::vector<BVH_t> bvhs;
	std::vector<Rig_t> rigs;
	std::vector<PolyGroup_t> poly_groups;
	std::vector<Face_t> faces;
	std::vector<Tex_t> tex_infos;
	std::vector<Vertex_t> vertices;
	std::vector<BoneHierarchy_t> bone_hierarchies;
	std::vector<Bone_t> bones;
	std::vector<Operation_t> operations;

};


void processTMD(std::istream& file, const std::streamoff tmd_start, TMD_RAW_Data& data_out);


void postProcessTMD(TMD_RAW_Data& data_out, const std::vector<CAT_Resource_Entry::Sub_Entry>& sub_entries);



