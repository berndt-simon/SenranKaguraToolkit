#pragma once
#include <inttypes.h>
#include <iosfwd>
#include <array>

#include "FileProcessing.h"
#include "CAT.h"

namespace TMD {

	struct Bounds_t {
		struct {
			float x;
			float y;
			float z;
		} min, max;
	};

	struct Range_t {
		uint32_t start;
		uint32_t count;
	};

	struct RangeInv_t {
		uint32_t count;
		uint32_t start;
	};

	union FeatureLevel_t {
		uint16_t as_short;
		struct {
			union {
				uint8_t lsb;
				struct {
					uint8_t _position : 1;
					uint8_t normals : 1;
					uint8_t uv_0 : 1;
					uint8_t uv_1 : 1;
					uint8_t _uv_2 : 1;
					uint8_t color : 1;
					uint8_t _unknown : 1;
					uint8_t rigging : 1;
				};
			};
			uint8_t msb;
		};
	};


	struct Header_t {
		uint32_t					magic_number;
		uint16_t					unknown_0;
		FeatureLevel_t				feature_level;
		std::array<uint16_t, 4>		unknowns;
		Bounds_t					bounds;
		Range_t						bvh_range;
		Range_t						operation_range;
		Range_t						unknown_range;
		Range_t						rig_range;
		Range_t						shader_range;
		Range_t						poly_group_range;
		Range_t						face_range;
		RangeInv_t					vertex_range;
		std::array<uint32_t, 7>		unknown_offsets;
		Range_t						bone_range;
		Range_t						bone_hierarchy_range;
		Range_t						tex_info_range;
		uint32_t					blank;

		bool verify();
	};

	namespace RAW {
		struct Vertex_t {
			std::array<float, 3>	pos;
			std::array<uint8_t, 4>	weight;
			std::array<uint8_t, 4>	bone;
			std::array<int8_t, 4>	normal;
			std::array<uint8_t, 4>	color;
			std::array<int16_t, 2>	tex;

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
			Bounds_t bounds;
			uint32_t unknown_0;
			uint32_t unknown_1;

			BVH_t();
		};

		struct Texture_t {
			uint32_t id;
			uint32_t blank;
			uint32_t unknown;
			std::array<int16_t, 2> uv;

			Texture_t();
		};

		struct Rig_t {
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

		struct Data_t {
			Header_t header;

			std::vector<BVH_t> bvhs;
			std::vector<Rig_t> rigs;
			std::vector<PolyGroup_t> poly_groups;
			std::vector<Face_t> faces;
			std::vector<Texture_t> texture_infos;
			std::vector<Vertex_t> vertices;
			std::vector<BoneHierarchy_t> bone_hierarchies;
			std::vector<Bone_t> bones;
			std::vector<Operation_t> operations;
		};

	}

	namespace PP {
		struct BoneWeight_t {
			uint32_t bone_id;
			float weight;

			BoneWeight_t();
		};

		struct Mesh_t {
			std::vector<decltype(RAW::Face_t::vertex_index)> faces;
			std::vector<std::array<uint32_t, 4>> bones; 
			std::vector<std::array<BoneWeight_t, 4>> weights;
			uint8_t material_id;
		};

		struct MaterialEntry_t {
			std::string package;
			std::string material_name;
		};


		std::string toString(const MaterialEntry_t& material_entry);

		struct Data_t {
			std::vector<decltype(RAW::Vertex_t::pos)> vertices;
			std::vector<decltype(RAW::Vertex_t::normal)> normals;
			std::vector<decltype(RAW::Vertex_t::color)> colors;
			std::vector<decltype(RAW::Vertex_t::tex)> uvs;

			std::vector<MaterialEntry_t> materials;

			std::vector<Mesh_t> meshes;
		};
	}



	void load_raw(std::istream& file, const std::streamoff tmd_start, RAW::Data_t& data_out);

	void post_process(const RAW::Data_t& data_in, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& sub_entries, PP::Data_t& data_out);


}

template<>
void read(std::istream& file, TMD::Range_t* dst);

template<>
void read(std::istream& file, TMD::RangeInv_t* dst);

template<>
void read(std::istream& file, TMD::FeatureLevel_t* dst);

template<>
void read(std::istream& file, TMD::Bounds_t* dst);

template<>
void read(std::istream& file, TMD::Header_t* dst);

