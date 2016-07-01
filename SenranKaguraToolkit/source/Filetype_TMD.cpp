#include "Filetype_TMD.h"

#include "FileProcessing.h"
#include <assert.h>
#include <iostream>

template<>
void read(std::istream& file, TMD_Range* dst) {
	read(file, &(dst->start));
	read(file, &(dst->count));
}

template<>
void read(std::istream& file, TMD_Range_Inv* dst) {
	read(file, &(dst->count));
	read(file, &(dst->start));
}

template<>
void read(std::istream& file, FeatureLevel* dst) {
	read(file, &(dst->as_short));
}

template<>
void read(std::istream& file, TMD_Bounds* dst) {
	read(file, &(dst->min.x));
	read(file, &(dst->min.y));
	read(file, &(dst->min.z));
	read(file, &(dst->max.x));
	read(file, &(dst->max.y));
	read(file, &(dst->max.z));
}

template<>
void read(std::istream& file, TMD_Header* dst) {
	read(file, &(dst->magic_number));
	read(file, &(dst->unknown_0));
	read(file, &(dst->feature_level));
	for (auto i(0); i < 4; i++) {
		read(file, &(dst->unknowns[i]));
	}
	read(file, &(dst->bounds));
	read(file, &(dst->bvh_range));
	for (auto i(0); i < 4; i++) {
		read(file, &(dst->unknown_range[i]));
	}
	read(file, &(dst->poly_group_range));
	read(file, &(dst->face_range));
	read(file, &(dst->vertex_range));
	for (auto i(0); i < 7; i++) {
		read(file, &(dst->unknown_offsets[i]));
	}
	read(file, &(dst->bone_range));
	read(file, &(dst->bone_hierarchy_range));
	read(file, &(dst->tex_info_range));
	read(file, &(dst->blank));
}

void processTMD(std::istream& file, const std::streamoff tmd_start, const std::vector<CAT_Resource_Entry::Sub_Entry>& sub_entries, TMD_Data& data_out) {
	
	// Read Header
	file.seekg(tmd_start, std::ios::beg);
	TMD_Header header;
	read(file, &header);
	assert(header.verify());
	const std::streamoff tmd_header_end = file.tellg();

	// Read Faces
	const std::streamoff face_offset = tmd_start + header.face_range.start;
	file.seekg(face_offset, std::ios::beg);
	for (auto i(0U); i < header.face_range.count; i++) {
		TMD_Data::Face_t face{ 0,0,0 };
		read(file, &face.f0);
		read(file, &face.f1);
		read(file, &face.f2);
		data_out.faces.push_back(face);
	}
	assert(file);

	// Read Vertices
	const std::streamoff vertex_offset = tmd_start + header.vertex_range.start;
	file.seekg(vertex_offset, std::ios::beg);
	const uint8_t vf = header.feature_level.lsb;
	for (auto i(0U); i < header.vertex_range.count; i++) {
		TMD_Data::Vertex_t vert;
		read(file, &vert.pos.x);
		read(file, &vert.pos.y);
		read(file, &vert.pos.z);
		if (vf > 0x6F) {
			read(file, &vert.weight.w0);
			read(file, &vert.weight.w1);
			read(file, &vert.weight.w2);
			read(file, &vert.weight.w3);
			read(file, &vert.bone.b0);
			read(file, &vert.bone.b1);
			read(file, &vert.bone.b2);
			read(file, &vert.bone.b3);
		}
		if (vf == 0xC7 || vf == 0xCF || vf == 0xE7) {
			read(file, &vert.normal.x);
			read(file, &vert.normal.y);
			read(file, &vert.normal.z);
			read(file, &vert.normal.q);
		}
		if (vf == 0x61 || vf == 0x65 || vf == 0xE7) {
			// Unused Ver_Color
			read<uint8_t>(file);
			read<uint8_t>(file);
			read<uint8_t>(file);
			read<uint8_t>(file);
		}
		if (vf > 0x61) {
			read(file, &vert.tex.u);
			read(file, &vert.tex.v);
		}		
		if (vf == 0xCF) {
			// Unused duplicate Tex_Coord
			read<int16_t>(file);
			read<int16_t>(file);
		}
		data_out.vertices.push_back(vert);
	}
	assert(file);
}

bool TMD_Header::verify() {
	return magic_number == 0x30646D74;
}
