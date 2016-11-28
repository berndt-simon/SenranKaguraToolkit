#include "TMD.h"

#include "FileProcessing.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>


namespace TMD {

	// Uncomment one of these Defines to skip the corresponding Readout-Step
	// Certain Combination lead to Runtime Errors

// #define _DEBUG

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
		Header_t& header = data_out.header;
		read(file, &header);
		if (!header.verify()) {
			throw std::runtime_error("Verification Failed");
		}
		const std::streamoff tmd_header_end(file.tellg());
		// Header is 160 Bytes long
		if (tmd_header_end - tmd_start != 160) {
			throw std::runtime_error("Header-Length Mismatch");
		}

#ifndef SKIP_READ_BVHS
		// Read BVHs
		const std::streamoff bvh_offset(tmd_start + header.bvh_range.start);
		file.seekg(bvh_offset, std::ios::beg);
		for (auto i(0U); i < header.bvh_range.count; ++i) {
			BVH_t bvh;
			read(file, &bvh.bounds);
			read(file, &bvh.unknown_0);
			read(file, &bvh.unknown_1);
			data_out.bvhs.push_back(bvh);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_BVHS	

#ifndef SKIP_READ_RIGS
		// Read Rigs
		const std::streamoff rig_offset(tmd_start + header.rig_range.start);
		file.seekg(rig_offset, std::ios::beg);
		for (auto i(0U); i < header.rig_range.count; ++i) {
			Rig_t rig;
			read(file, &rig.count);
			for (auto& bone : rig.bone) {
				read(file, &bone);
			}
			data_out.rigs.push_back(rig);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_RIGS

#ifndef SKIP_READ_SHADERS
		static_assert(false, "Not Implemented Yet!");
#endif // READ_SHADERS

#ifndef SKIP_READ_POLY_GROUPS
		// Read Polygroups
		const std::streamoff poly_group_offset(tmd_start + header.poly_group_range.start);
		file.seekg(poly_group_offset, std::ios::beg);
		for (auto i(0U); i < header.poly_group_range.count; ++i) {
			PolyGroup_t poly_group;
			read(file, &poly_group.count);
			read(file, &poly_group.unknown);
			read(file, &poly_group.offset);
			data_out.poly_groups.push_back(poly_group);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_POLY_GROUPS

#ifndef SKIP_READ_FACES
		// Read Faces
		const std::streamoff face_offset(tmd_start + header.face_range.start);
		file.seekg(face_offset, std::ios::beg);
		for (auto i(0U); i < header.face_range.count; ++i) {
			Face_t face;
			for (auto& index : face.vertex_index) {
				read(file, &index);
			}
			data_out.faces.push_back(face);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_FACES

#ifndef SKIP_READ_TEXTURES
		// Read Texture Infos
		const std::streamoff tex_info_offset(tmd_start + header.tex_info_range.start);
		file.seekg(tex_info_offset, std::ios::beg);
		for (auto i(0U); i < header.tex_info_range.count; ++i) {
			Texture_t tex;
			read(file, &tex.id);
			read(file, &tex.blank);
			read(file, &tex.unknown);
			for (auto& uv : tex.uv) {
				read(file, &uv);
			}
			data_out.texture_infos.push_back(tex);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_TEXTURES

#ifndef SKIP_READ_VERTICES
		// Read Vertices
		const std::streamoff vertex_offset(tmd_start + header.vertex_range.start);
		file.seekg(vertex_offset, std::ios::beg);
		const auto& fl = header.feature_level;

#ifdef _DEBUG
#include <assert.h>
		{
			{
				const FeatureLevel_t test_fl_0{ 0xC7U };
				assert(test_fl_0._position);
				assert(test_fl_0.normals);
				assert(test_fl_0.uv_0);
				assert(!test_fl_0.uv_1);
				assert(!test_fl_0.color);
				assert(test_fl_0.rigging);
			}

			{
				const FeatureLevel_t test_fl_1{ 0xCFU };
				assert(test_fl_1._position);
				assert(test_fl_1.normals);
				assert(test_fl_1.uv_0);
				assert(test_fl_1.uv_1);
				assert(!test_fl_1.color);
				assert(test_fl_1.rigging);
			}

		}
#endif // DEBUG

		for (auto i(0U); i < header.vertex_range.count; ++i) {
			Vertex_t vert;
			if (fl._position) {
				for (auto& pos : vert.pos) {
					read(file, &pos);
				}
			}
			if (fl.rigging) {
				for (auto& weight : vert.weight) {
					read(file, &weight);
				}
				for (auto& bone : vert.bone) {
					read(file, &bone);
				}
			}
			if (fl.normals) {
				for (auto& normal : vert.normal) {
					read(file, &normal);
				}
			}
			if (fl.color) {
				// Unused Ver_Color
				for (auto& color : vert.color) {
					read(file, &color);
				}
			}
			if (fl.uv_0) {
				for (auto& uv : vert.tex) {
					read(file, &uv);
				}
			}
			if (fl.uv_1) {
				// Unused duplicate Tex_Coord
				read<int16_t>(file);
				read<int16_t>(file);
			}
			data_out.vertices.push_back(vert);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_VERTICES

#ifndef SKIP_READ_BONE_HIERARCHIES
		// Read Bone Hierarchies
		const std::streamoff bone_hierarchies_offset(tmd_start + header.bone_hierarchy_range.start);
		file.seekg(bone_hierarchies_offset, std::ios::beg);
		for (auto i(0U); i < header.bone_hierarchy_range.count; ++i) {
			BoneHierarchy_t bone_hierarchy;
			read(file, &bone_hierarchy.hash);
			for (auto& head_pos : bone_hierarchy.head_pos) {
				read(file, &head_pos);
			}
			read(file, &bone_hierarchy.parent);
			read(file, &bone_hierarchy.head);
			data_out.bone_hierarchies.push_back(bone_hierarchy);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_BONE_HIERARCHIES

#ifndef SKIP_READ_BONES
		// Read Bone Hierarchies
		const std::streamoff bones_offset(tmd_start + header.bone_range.start);
		file.seekg(bones_offset, std::ios::beg);
		for (auto i(0U); i < header.bone_range.count; ++i) {
			Bone_t bone;
			for (auto& row : bone.mat) {
				for (auto& cell : row) {
					read(file, &cell);
				}
			}
			data_out.bones.push_back(bone);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_BONES

#ifndef SKIP_READ_OPERATIONS
		// Read Operations
		const std::streamoff operation_offset(tmd_start + header.operation_range.start);
		file.seekg(operation_offset, std::ios::beg);
		for (auto i(0U); i < header.operation_range.count; ++i) {
			Operation_t operation;
			read(file, &operation.value);
			read(file, &operation.type);
			data_out.operations.push_back(operation);
		}
		if (!file) {
			throw std::runtime_error(nullptr);
		}
#endif // READ_OPERATIONS
	}

	void post_process(const RAW::Data_t& data, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& sub_entries, PostProcessed::Data_t& data_out) {
		const FeatureLevel_t& fl = data.header.feature_level;
		// Copy Commons
		for (auto vIdx(0U); vIdx < data.vertices.size(); ++vIdx) {
			const RAW::Vertex_t& vert = data.vertices[vIdx];
			if (fl._position) {
				data_out.vertices.push_back(vert.pos);
			}
			if (fl.normals) {
				data_out.normals.push_back(vert.normal);
			}
			if (fl.color) {
				data_out.colors.push_back(vert.color);
			}
			if (fl.uv_0) {
				data_out.uvs.push_back(vert.tex);
			}
		}

		// Copy Materials
		for (const auto& sub_entry : sub_entries) {
			data_out.materials.push_back(PostProcessed::MaterialEntry_t({ sub_entry.package, sub_entry.resource }));
		}

		uint8_t curr_mat(0U);
		uint8_t curr_rig(0U);


		for (const auto& operation : data.operations) {
			switch (operation.type) {
				case 0x10:
					{
						//goto operation_processing_end;
						break;
					}
				case 0x20:
					{
						curr_mat = operation.value;
						break;
					}
				case 0x30:
					{
						PostProcessed::Mesh_t mesh;
						mesh.material_id = curr_mat;

						// Copy Faces
						for (auto i(0U); i < data.poly_groups[operation.value].count; ++i) {
							mesh.faces.push_back(data.faces[data.poly_groups[operation.value].offset + i].vertex_index);
						}

						for (auto vIdx(0U); vIdx < data.vertices.size(); ++vIdx) {
							const RAW::Vertex_t& vert = data.vertices[vIdx];
							if (fl.rigging) {
								const auto& rel_bone_ids = vert.bone;
								std::array<uint32_t, 4> abs_bone_ids;
								if (rel_bone_ids.size() != abs_bone_ids.size()) {
									throw std::runtime_error("Bone-Count Mismatch");
								}

								for (auto b(0U); b < 4U; ++b) {
									abs_bone_ids[b] = data.rigs[curr_rig].bone[rel_bone_ids[b]];
								}
								mesh.bones.push_back(abs_bone_ids);

								std::array<PostProcessed::BoneWeight_t, 4> bone_weights;
								if (bone_weights.size() != vert.weight.size()) {
									throw std::runtime_error("BoneWeight-Count Mismatch");
								}
								for (auto wIdx(0U); wIdx < vert.weight.size(); ++wIdx) {
									if (vert.weight[wIdx] > 0) {
										bone_weights[wIdx].weight = static_cast<float>(vert.weight[wIdx]);
										bone_weights[wIdx].bone_id = mesh.bones[vIdx][wIdx];
									}
								}
								mesh.weights.push_back(bone_weights);
							}
						}

						data_out.meshes.push_back(mesh);
						break;
					}
				case 0x40:
					{
						curr_rig = operation.value;
						break;
					}
			}
		}
operation_processing_end:;
	}

	bool Header_t::verify() {
		// "tmd0" as uint32_t;
		return magic_number == 0x30646D74U;
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
	for (auto& unknown : dst->unknowns) {
		read(file, &unknown);
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
	for (auto& unknown_offset : dst->unknown_offsets) {
		read(file, &unknown_offset);
	}
	read(file, &(dst->bone_range));
	read(file, &(dst->bone_hierarchy_range));
	read(file, &(dst->tex_info_range));
	read(file, &(dst->blank));
}

std::ostream& operator<<(std::ostream& out, const TMD::PostProcessed::MaterialEntry_t& material_entry) {
	out << material_entry.package << '_' << material_entry.material_name;
	return out;
}


std::array<float, 2> TMD::PostProcessed::Data_t::normalize_uvs(const decltype(RAW::Vertex_t::tex)& uvs) {
	return std::array<float, 2>{static_cast<float>(uvs[0]) / 1024, static_cast<float>(uvs[1]) / -1024};
}


std::array<float, 3> TMD::PostProcessed::Data_t::normalize_normals(const decltype(RAW::Vertex_t::normal)& normals) {
	using inner_t = decltype(RAW::Vertex_t::normal)::value_type;
	return std::array<float, 3>{
		std::max(static_cast<float>(normals[0]) / std::numeric_limits<inner_t>::max(), -1.0f),
		std::max(static_cast<float>(normals[1]) / std::numeric_limits<inner_t>::max(), -1.0f),
		std::max(static_cast<float>(normals[2]) / std::numeric_limits<inner_t>::max(), -1.0f)};
}