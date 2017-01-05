#include "Exporter_OBJ.h"

#include "FileProcessing.h"
#include <iostream>
#include <iomanip>


void ObjExporter::save(const TMD::PostProcessed::Data_t& data, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& references) {
	boost::filesystem::path output_path(_export_folder);
	output_path /= "tmd";
	boost::filesystem::path obj_filename(output_path);
	obj_filename += _obj_suffix;

	std::ofstream obj_out;
	open_to_write(obj_out, obj_filename);
	write_obj(obj_out, data);
	obj_out.close();

	if (_export_materials) {
		boost::filesystem::path mtl_filename(output_path);
		mtl_filename += _mtl_suffix;

		std::ofstream mtl_out;
		open_to_write(mtl_out, mtl_filename);
		write_mtl(mtl_out, references);
		mtl_out.close();
	}
}

void ObjExporter::write_mtl(std::ostream& mtl, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& references) {
	auto mat_cnt(0);
	for (const auto& ref : references) {
		mtl << "newmtl mat_" << mat_cnt << std::endl;
		++mat_cnt;
		mtl << "illum 0" << std::endl;
		mtl << "map_Ka " << _material_resource_prefix << ref.package << "\\" << ref.resource << _material_resource_suffix << std::endl << std::endl;
	}
}

void ObjExporter::write_obj(std::ostream& obj, const TMD::PostProcessed::Data_t& data) {
	obj << "# Converted TMD" << std::endl;
	obj << "o TMD-Object" << std::endl;
	for (const auto& vert : data.vertices) {
		obj << "v ";
		obj << static_cast<double>(vert[0]) * _rescale_fator << ' ';
		obj << static_cast<double>(vert[1]) * _rescale_fator << ' ';
		obj << static_cast<double>(vert[2]) * _rescale_fator << std::endl;
	}
	if (_export_uvs) {
		for (const auto& uv : data.uvs) {
			obj << "vt " << uv[0] << ' ' << uv[1] << std::endl;
		}
	}
	if (_export_normals) {
		auto flip_fac = 1;
		if (_flip_normals) {
			flip_fac = -1;
		}
		for (const auto& normal : data.normals) {
			obj << "vn ";
			obj << flip_fac * normal[0] << ' ';
			obj << flip_fac * normal[1] << ' ';
			obj << flip_fac * normal[2] << std::endl;
		}
	}
	for (auto mIdx(0U); mIdx < data.meshes.size(); ++mIdx) {
		const TMD::PostProcessed::Mesh_t& mesh = data.meshes[mIdx];

		obj << "g TMD_Mesh_" << std::setfill('0') << std::setw(2) << mIdx << std::endl;
		if (_export_materials) {
			obj << "usemtl mat_" << static_cast<int>(mesh.material_id) << std::endl;
		}
		for (const auto& face : mesh.faces) {
			obj << "f ";
			write_face_vertex(obj, face, 0U);
			write_face_vertex(obj, face, 1U);
			write_face_vertex(obj, face, 2U);
			obj << std::endl;
		}
	}

}

void ObjExporter::write_face_vertex(std::ostream& obj, const std::array<uint16_t, 3>& vertices,  uint8_t idx) {
	const uint16_t vIdx(vertices[idx] + 1);
	obj << vIdx;
	if (_export_uvs || _export_normals) {
		obj << '/';
		if (_export_uvs) {
			obj << vIdx;
		}
	}
	if (_export_normals) {
		obj << '/' << vIdx;
	}
	obj << ' ';
}

ObjExporter::ObjExporter()
	: Exporter()
	, _obj_suffix(".obj")
	, _mtl_suffix(".mtl") {
}

boost::filesystem::path& ObjExporter::obj_suffix() {
	return _obj_suffix;
}
const boost::filesystem::path& ObjExporter::obj_suffix() const {
	return _obj_suffix;
}

boost::filesystem::path& ObjExporter::mtl_suffix() {
	return _mtl_suffix;
}
const boost::filesystem::path& ObjExporter::mtl_suffix() const {
	return _mtl_suffix;
}