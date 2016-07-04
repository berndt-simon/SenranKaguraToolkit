#include "Exporter_OBJ.h"

#include "FileProcessing.h"
#include <iostream>
#include <iomanip>

ObjExporter::ObjExporter()
	: Exporter()
	, _obj_suffix(".obj")
	, _mtl_suffix(".mtl") {
}

void ObjExporter::save(const TMD::PP::Data_t& data) {
	boost::filesystem::path output_path(_export_folder);
	output_path /= "tmd";
	boost::filesystem::path obj_filename(output_path);
	obj_filename += _obj_suffix;

	std::ofstream obj_out;
	openToWrite(obj_out, obj_filename);
	write_obj(obj_out, data);
	obj_out.close();

	if (_export_materials) {
		boost::filesystem::path mtl_filename(output_path);
		mtl_filename += _mtl_suffix;

		std::ofstream mtl_out;
		openToWrite(mtl_out, mtl_filename);
		write_mtl(mtl_out, data);
		mtl_out.close();
	}
}

boost::filesystem::path& ObjExporter::obj_suffix() {
	return _obj_suffix;
}

boost::filesystem::path& ObjExporter::mtl_suffix() {
	return _mtl_suffix;
}


void ObjExporter::write_mtl(std::ostream& mtl, const TMD::PP::Data_t& data) {
	for (auto eItt(data.materials.begin()); eItt != data.materials.end(); eItt++) {
		mtl << "newmtl " << toString((*eItt)) << std::endl;
		mtl << "illum 0" << std::endl;
		mtl << "map_Ka " << _material_resource_prefix << eItt->package << "\\" << eItt->material_name << _material_resource_suffix << std::endl << std::endl;
	}
}

void ObjExporter::write_obj(std::ostream& obj, const TMD::PP::Data_t& data) {
	obj << "# Converted TMD" << std::endl;
	obj << "o TMD-Object" << std::endl;
	for (auto vItt(data.vertices.begin()); vItt != data.vertices.end(); vItt++) {
		obj << "v ";
		obj << static_cast<double>((*vItt)[0]) * _rescale_fator << " ";
		obj << static_cast<double>((*vItt)[1]) * _rescale_fator << " ";
		obj << static_cast<double>((*vItt)[2]) * _rescale_fator << std::endl;
	}
	if (_export_uvs) {
		for (auto vItt(data.uvs.begin()); vItt != data.uvs.end(); vItt++) {
			obj << "vt ";
			obj << static_cast<float>((*vItt)[0]) / 1024 << " ";
			obj << static_cast<float>((*vItt)[1]) / -1024 << std::endl;
		}
	}
	if (_export_normals) {
		auto flip_fac = 1;
		if (_flip_normals) {
			flip_fac = -1;
		}
		for (auto vItt(data.normals.begin()); vItt != data.normals.end(); vItt++) {
			obj << "vn ";
			obj << static_cast<float>(flip_fac * (*vItt)[0]) << " ";
			obj << static_cast<float>(flip_fac * (*vItt)[1]) << " ";
			obj << static_cast<float>(flip_fac * (*vItt)[2]) << std::endl;
		}
	}
	for (auto mIdx(0U); mIdx < data.meshes.size(); mIdx++) {
		const TMD::PP::Mesh_t& mesh = data.meshes[mIdx];

		obj << "g TMD_Mesh_" << std::setfill('0') << std::setw(2) << mIdx << std::endl;
		if (_export_materials) {
			obj << "usemtl " << toString(data.materials[mesh.material_id]) << std::endl;
		}
		for (auto fItt(mesh.faces.begin()); fItt != mesh.faces.end(); fItt++) {
			obj << "f ";
			write_face_vertex(obj, *fItt, 0U);
			write_face_vertex(obj, *fItt, 1U);
			write_face_vertex(obj, *fItt, 2U);
			obj << std::endl;
		}
	}

}

void ObjExporter::write_face_vertex(std::ostream& obj, const std::array<uint16_t, 3>& vertices,  uint8_t idx) {
	const uint16_t vIdx(vertices[idx] + 1);
	obj << vIdx;
	if (_export_uvs || _export_normals) {
		obj << "/";
		if (_export_uvs) {
			obj << vIdx;
		}
	}
	if (_export_normals) {
		obj << "/" << vIdx;
	}
	obj << " ";
}