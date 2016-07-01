#include "Filetype_TMD.h"

#include "FileProcessing.h"
#include <fstream>


template<>
void read(std::ifstream& file, TMD_Range* dst) {
	read(file, &(dst->start));
	read(file, &(dst->count));
}

template<>
void read(std::ifstream& file, FeatureLevel* dst) {
	read(file, &(dst->as_short));
}

template<>
void read(std::ifstream& file, TMD_Bounds* dst) {
	read(file, &(dst->min.x));
	read(file, &(dst->min.y));
	read(file, &(dst->min.z));
	read(file, &(dst->max.x));
	read(file, &(dst->max.y));
	read(file, &(dst->max.z));
}

template<>
void read(std::ifstream& file, TMD_Header* dst) {
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

void processTMD(std::ifstream& file) {
	const std::streampos fileStart = file.tellg();

	TMD_Header header;
	read(file, &header);
}