#include "Filetype_TMD.h"

#include "FileProcessing.h"
#include <assert.h>
#include <iostream>

namespace TMD {

	// Uncomment one of these Defines to skip the corresponding Readout-Step
	// Certain Combination lead to Runtime Errors

// #define SKIP_READ_FACES
// #define SKIP_READ_VERTICES
// #define SKIP_READ_RIGS
#define SKIP_READ_BVHS
#define SKIP_READ_SHADERS
// #define SKIP_READ_TEXTURES
// #define SKIP_READ_POLY_GROUPS
// #define SKIP_READ_BONES
// #define SKIP_READ_BONE_HIERARCHIES
// #define SKIP_READ_OPERATIONS

	void load_raw(std::istream& file, const std::streamoff tmd_start, RAW::Data_t& data_out) {
		using namespace RAW;

		// Read Header
		file.seekg(tmd_start, std::ios::beg);
		Header_t header;
		read(file, &header);
		assert(header.verify());
		const std::streamoff tmd_header_end = file.tellg();
		// Header is 160 Bytes long
		assert(tmd_header_end - tmd_start == 160);

#ifndef SKIP_READ_BVHS
		// Read BVHs
		const std::streamoff bvh_offset = tmd_start + header.bvh_range.start;
		file.seekg(bvh_offset, std::ios::beg);
		for (auto i(0U); i < header.bvh_range.count; i++) {
			BVH_t bvh;
			read(file, &bvh.bounds);
			read(file, &bvh.unknown_0);
			read(file, &bvh.unknown_1);
			data_out.bvhs.push_back(bvh);
		}
		assert(file);
#endif // READ_BVHS	

#ifndef SKIP_READ_RIGS
		// Read Rigs
		const std::streamoff rig_offset = tmd_start + header.rig_range.start;
		file.seekg(rig_offset, std::ios::beg);
		for (auto i(0U); i < header.rig_range.count; i++) {
			Rig_t rig;
			read(file, &rig.count);
			for (auto bItt(rig.bone.begin()); bItt != rig.bone.end(); bItt++) {
				read(file, &(*bItt));
			}
			data_out.rigs.push_back(rig);
		}
		assert(file);
#endif // READ_RIGS

#ifndef SKIP_READ_SHADERS
		static_assert(false, "Not Implemented Yet!");
#endif // READ_SHADERS

#ifndef SKIP_READ_POLY_GROUPS
		// Read Polygroups
		const std::streamoff poly_group_offset = tmd_start + header.poly_group_range.start;
		file.seekg(poly_group_offset, std::ios::beg);
		for (auto i(0U); i < header.poly_group_range.count; i++) {
			PolyGroup_t poly_group;
			read(file, &poly_group.count);
			read(file, &poly_group.unknown);
			read(file, &poly_group.offset);
			data_out.poly_groups.push_back(poly_group);
		}
		assert(file);
#endif // READ_POLY_GROUPS

#ifndef SKIP_READ_FACES
		// Read Faces
		const std::streamoff face_offset = tmd_start + header.face_range.start;
		file.seekg(face_offset, std::ios::beg);
		for (auto i(0U); i < header.face_range.count; i++) {
			Face_t face;
			for (auto iItt(face.vertex_index.begin()); iItt != face.vertex_index.end(); iItt++) {
				read(file, &(*iItt));
			}
			data_out.faces.push_back(face);
		}
		assert(file);
#endif // READ_FACES

#ifndef SKIP_READ_TEXTURES
		// Read Texture Infos
		const std::streamoff tex_info_offset = tmd_start + header.tex_info_range.start;
		file.seekg(tex_info_offset, std::ios::beg);
		for (auto i(0U); i < header.tex_info_range.count; i++) {
			Texture_t tex;
			read(file, &tex.id);
			read(file, &tex.blank);
			read(file, &tex.unknown);
			for (auto cItt(tex.uv.begin()); cItt != tex.uv.end(); cItt++) {
				read(file, &(*cItt));
			}
			data_out.texture_infos.push_back(tex);
		}
		assert(file);
#endif // READ_TEXTURES

#ifndef SKIP_READ_VERTICES
		// Read Vertices
		const std::streamoff vertex_offset = tmd_start + header.vertex_range.start;
		file.seekg(vertex_offset, std::ios::beg);
		const uint8_t vf = header.feature_level.lsb;
		for (auto i(0U); i < header.vertex_range.count; i++) {
			Vertex_t vert;
			for (auto pItt(vert.pos.begin()); pItt != vert.pos.end(); pItt++) {
				read(file, &(*pItt));
			}
			if (vf > 0x6F) {
				for (auto wItt(vert.weight.begin()); wItt != vert.weight.end(); wItt++) {
					read(file, &(*wItt));
				}
				for (auto bItt(vert.bone.begin()); bItt != vert.bone.end(); bItt++) {
					read(file, &(*bItt));
				}
			}
			if (vf == 0xC7 || vf == 0xCF || vf == 0xE7) {
				for (auto nItt(vert.normal.begin()); nItt != vert.normal.end(); nItt++) {
					read(file, &(*nItt));
				}
			}
			if (vf == 0x61 || vf == 0x65 || vf == 0xE7) {
				// Unused Ver_Color
				for (auto cItt(vert.color.begin()); cItt != vert.color.end(); cItt++) {
					read(file, &(*cItt));
				}
			}
			if (vf > 0x61) {
				for (auto tItt(vert.tex.begin()); tItt != vert.tex.end(); tItt++) {
					read(file, &(*tItt));
				}
			}
			if (vf == 0xCF) {
				// Unused duplicate Tex_Coord
				read<int16_t>(file);
				read<int16_t>(file);
			}
			data_out.vertices.push_back(vert);
		}
		assert(file);
#endif // READ_VERTICES

#ifndef SKIP_READ_BONE_HIERARCHIES
		// Read Bone Hierarchies
		const std::streamoff bone_hierarchies_offset = tmd_start + header.bone_hierarchy_range.start;
		file.seekg(bone_hierarchies_offset, std::ios::beg);
		for (auto i(0U); i < header.bone_hierarchy_range.count; i++) {
			BoneHierarchy_t bone_hierarchy;
			read(file, &bone_hierarchy.hash);
			for (auto hItt(bone_hierarchy.head_pos.begin()); hItt != bone_hierarchy.head_pos.end(); hItt++) {
				read(file, &(*hItt));
			}
			read(file, &bone_hierarchy.parent);
			read(file, &bone_hierarchy.head);
			data_out.bone_hierarchies.push_back(bone_hierarchy);
		}
		assert(file);
#endif // READ_BONE_HIERARCHIES

#ifndef SKIP_READ_BONES
		// Read Bone Hierarchies
		const std::streamoff bones_offset = tmd_start + header.bone_range.start;
		file.seekg(bones_offset, std::ios::beg);
		for (auto i(0U); i < header.bone_range.count; i++) {
			Bone_t bone;
			for (auto yItt(bone.mat.begin()); yItt != bone.mat.end(); yItt++) {
				for (auto xItt(yItt->begin()); xItt != yItt->end(); xItt++) {
					read(file, &(*xItt));
				}
			}
			data_out.bones.push_back(bone);
		}
		assert(file);
#endif // READ_BONES

#ifndef SKIP_READ_OPERATIONS
		// Read Operations
		const std::streamoff operation_offset = tmd_start + header.operation_range.start;
		file.seekg(operation_offset, std::ios::beg);
		for (auto i(0U); i < header.operation_range.count; i++) {
			Operation_t operation;
			read(file, &operation.value);
			read(file, &operation.type);
			data_out.operations.push_back(operation);
		}
		assert(file);
#endif // READ_OPERATIONS
	}

	void post_process(RAW::Data_t& data, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& sub_entries) {
		/*for (auto i(0U); i < data.operations.size(); i++) {
			const TMD_Data::Operation_t& op = data.operations[i];
			switch (op.type) {
				case 0x30:
					for (auto y(0U); y < data.poly_groups[op.value].count; y++) {

					}
					break;
			}
		}*/
	}

	bool Header_t::verify() {
		return magic_number == 0x30646D74;
	}

	namespace RAW {
		Face_t::Face_t() {
			vertex_index.fill(0U);
		}

		Vertex_t::Vertex_t() {
			pos.fill(0.0f);
			weight.fill(0U);
			bone.fill(0U);
			normal.fill(0);
			color.fill(0U);
			tex.fill(0);
		}

		PolyGroup_t::PolyGroup_t()
			: count(0U)
			, unknown(0U)
			, offset(0U) {
		}

		Operation_t::Operation_t()
			: value(0U)
			, type(0U) {
		}

		BVH_t::BVH_t()
			: bounds()
			, unknown_0(0U)
			, unknown_1(0U) {
		}

		Texture_t::Texture_t()
			: id(0U)
			, blank(0U)
			, unknown(0U) {
			uv.fill(0);
		}

		Rig_t::Rig_t()
			: count(0) {
			bone.fill(0);
		}

		Bone_t::Bone_t() {
			for (auto rItt(mat.begin()); rItt != mat.end(); rItt++) {
				rItt->fill(0.0f);
			}
		}

		BoneHierarchy_t::BoneHierarchy_t()
			: hash(0U)
			, parent(0)
			, head(0U) {
			head_pos.fill(0.0f);
		}
	}


}


template<>
void read(std::istream& file, TMD::Range_t* dst) {
	read(file, &(dst->start));
	read(file, &(dst->count));
}

template<>
void read(std::istream& file, TMD::RangeInv_t* dst) {
	read(file, &(dst->count));
	read(file, &(dst->start));
}

template<>
void read(std::istream& file, TMD::FeatureLevel_t* dst) {
	read(file, &(dst->as_short));
}

template<>
void read(std::istream& file, TMD::Bounds_t* dst) {
	read(file, &(dst->min.x));
	read(file, &(dst->min.y));
	read(file, &(dst->min.z));
	read(file, &(dst->max.x));
	read(file, &(dst->max.y));
	read(file, &(dst->max.z));
}

template<>
void read(std::istream& file, TMD::Header_t* dst) {
	read(file, &(dst->magic_number));
	read(file, &(dst->unknown_0));
	read(file, &(dst->feature_level));
	for (auto uItt(dst->unknowns.begin()); uItt != dst->unknowns.end(); uItt++) {
		read(file, &(*uItt));
	}
	read(file, &(dst->bounds));
	read(file, &(dst->bvh_range));
	read(file, &(dst->operation_range));
	read(file, &(dst->unknown_range));
	read(file, &(dst->rig_range));
	read(file, &(dst->shader_range));
	read(file, &(dst->poly_group_range));
	read(file, &(dst->face_range));
	read(file, &(dst->vertex_range));
	for (auto uItt(dst->unknown_offsets.begin()); uItt != dst->unknown_offsets.end(); uItt++) {
		read(file, &(*uItt));
	}
	read(file, &(dst->bone_range));
	read(file, &(dst->bone_hierarchy_range));
	read(file, &(dst->tex_info_range));
	read(file, &(dst->blank));
}
