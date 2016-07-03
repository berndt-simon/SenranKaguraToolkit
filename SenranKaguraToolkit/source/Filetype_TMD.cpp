#include "Filetype_TMD.h"

#include "FileProcessing.h"
#include <assert.h>
#include <iostream>
#include <iomanip>

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
		Header_t& header = data_out.header;
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
		const auto fl = header.feature_level;

#ifdef _DEBUG
		{
			{
				const FeatureLevel_t test_fl_0{ 0xC7U };
				assert(test_fl_0.position);
				assert(test_fl_0.normals);
				assert(test_fl_0.uv_0);
				assert(!test_fl_0.uv_1);
				assert(!test_fl_0.color);
				assert(test_fl_0.rigging);
			}

			{
				const FeatureLevel_t test_fl_1{ 0xCFU };
				assert(test_fl_1.position);
				assert(test_fl_1.normals);
				assert(test_fl_1.uv_0);
				assert(test_fl_1.uv_1);
				assert(!test_fl_1.color);
				assert(test_fl_1.rigging);
			}

		}
#endif // DEBUG


		for (auto i(0U); i < header.vertex_range.count; i++) {
			Vertex_t vert;
			if (fl.position) {
				for (auto pItt(vert.pos.begin()); pItt != vert.pos.end(); pItt++) {
					read(file, &(*pItt));
				}
			}
			if (fl.rigging) {
				for (auto wItt(vert.weight.begin()); wItt != vert.weight.end(); wItt++) {
					read(file, &(*wItt));
				}
				for (auto bItt(vert.bone.begin()); bItt != vert.bone.end(); bItt++) {
					read(file, &(*bItt));
				}
			}
			if (fl.normals) {
				for (auto nItt(vert.normal.begin()); nItt != vert.normal.end(); nItt++) {
					read(file, &(*nItt));
				}
			}
			if (fl.color) {
				// Unused Ver_Color
				for (auto cItt(vert.color.begin()); cItt != vert.color.end(); cItt++) {
					read(file, &(*cItt));
				}
			}
			if (fl.uv_0) {
				for (auto tItt(vert.tex.begin()); tItt != vert.tex.end(); tItt++) {
					read(file, &(*tItt));
				}
			}
			if (fl.uv_1) {
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

	void post_process(const RAW::Data_t& data, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& sub_entries, PP::Data_t& data_out) {
		const FeatureLevel_t& fl = data.header.feature_level;
		// Copy Commons
		for (auto vIdx(0U); vIdx < data.vertices.size(); vIdx++) {
			const RAW::Vertex_t& vert = data.vertices[vIdx];
			if (fl.position) {
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
		for (auto eItt(sub_entries.begin()); eItt != sub_entries.end(); eItt++) {
			data_out.materials.push_back(PP::MaterialEntry_t({ eItt->package, eItt->resource }));
		}

		uint8_t curr_mat(0U);
		uint8_t curr_rig(0U);


		for (auto oItt(data.operations.begin()); oItt != data.operations.end(); oItt++) {
			switch (oItt->type) {
				case 0x10:
					// oItt = data.operations.end();
					break;
				case 0x20:
					curr_mat = oItt->value;
					break;
				case 0x30:
					{
						PP::Mesh_t mesh;
						mesh.material_id = curr_mat;

						// Copy Faces
						for (auto i(0U); i < data.poly_groups[oItt->value].count; i++) {
							mesh.faces.push_back(data.faces[data.poly_groups[oItt->value].offset + i].vertex_index);
						}

						for (auto vIdx(0U); vIdx < data.vertices.size(); vIdx++) {
							const RAW::Vertex_t& vert = data.vertices[vIdx];
							if (fl.rigging) {
								const auto& rel_bone_ids = vert.bone;
								std::array<uint32_t, 4> abs_bone_ids;
								assert(rel_bone_ids.size() == abs_bone_ids.size());

								for (auto b(0U); b < 4U; b++) {
									abs_bone_ids[b] = data.rigs[curr_rig].bone[rel_bone_ids[b]];
								}
								mesh.bones.push_back(abs_bone_ids);

								std::array<PP::BoneWeight_t, 4> bone_weights;
								assert(bone_weights.size() == vert.weight.size());
								for (auto wIdx(0U); wIdx < vert.weight.size(); wIdx++) {
									if (vert.weight[wIdx] > 0) {
										bone_weights[wIdx].weight = static_cast<float>(vert.weight[wIdx]);
										bone_weights[wIdx].bone_id = mesh.bones[vIdx][wIdx];
									}
								}
								mesh.weights.push_back(bone_weights);
							}
						}

						data_out.meshes.push_back(mesh);
					}
					break;
				case 0x40:
					curr_rig = oItt->value;
					break;
			}
		}
	}

	void write_mtl(std::ostream& mtl, const PP::Data_t& data, const std::string& path_prefix, const std::string& path_suffix) {
		auto cntr(0U);
		for (auto eItt(data.materials.begin()); eItt != data.materials.end(); eItt++) {
			mtl << "newmtl " << toString((*eItt)) << std::endl;
			mtl << "Ka 0 0 0" << std::endl;
			mtl << "Kd 0 0 0" << std::endl;
			mtl << "Ks 0 0 0" << std::endl;
			mtl << "Ns 0" << std::endl;
			mtl << "illum 0" << std::endl;
			mtl << "map_Kd " << path_prefix << eItt->package << "\\" << eItt->material_name << path_suffix << std::endl << std::endl;
			cntr++;
		}

	}

	void write_obj(std::ostream& obj, const PP::Data_t& data) {
		obj << "# Converted TMD" << std::endl;
		obj << "o TMD-Object" << std::endl;
		for (auto vItt(data.vertices.begin()); vItt != data.vertices.end(); vItt++) {
			obj << "v ";
			obj << (*vItt)[0] << " ";
			obj << (*vItt)[1] << " ";
			obj << (*vItt)[2] << std::endl;
		}
		for (auto vItt(data.uvs.begin()); vItt != data.uvs.end(); vItt++) {
			obj << "vt ";
			obj << static_cast<float>((*vItt)[0]) / 1024 << " ";
			obj << static_cast<float>((*vItt)[1]) / -1024 << std::endl;
		}
		for (auto vItt(data.normals.begin()); vItt != data.normals.end(); vItt++) {
			obj << "vn ";
			obj << static_cast<float>((*vItt)[0]) << " ";
			obj << static_cast<float>((*vItt)[1]) << " ";
			obj << static_cast<float>((*vItt)[2]) << std::endl;
		}

		for (auto mIdx(0U); mIdx < data.meshes.size(); mIdx++) {
			const PP::Mesh_t& mesh = data.meshes[mIdx];

			obj << "g TMD_Sub-Object_" << std::setfill('0') << std::setw(2) << mIdx << std::endl;
			obj << "usemtl " << toString(data.materials[mesh.material_id]) << std::endl;
			
			for (auto fItt(mesh.faces.begin()); fItt != mesh.faces.end(); fItt++) {
				obj << "f ";
				obj << (*fItt)[0] + 1 << "/" << (*fItt)[0] + 1 << "/" << (*fItt)[0] + 1 << " ";
				obj << (*fItt)[1] + 1 << "/" << (*fItt)[1] + 1 << "/" << (*fItt)[1] + 1 << " ";
				obj << (*fItt)[2] + 1 << "/" << (*fItt)[2] + 1 << "/" << (*fItt)[2] + 1 << std::endl;
			}
		} 
		
	}

	bool Header_t::verify() {
		// "tmd0" as uint32_t;
		return magic_number == 0x30646D74U;
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



	namespace PP {
		BoneWeight_t::BoneWeight_t()
			: bone_id(0U)
			, weight(0.0f) {
		}

		std::string toString(const MaterialEntry_t& material_entry) {
			return material_entry.package + "_" + material_entry.material_name;
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
