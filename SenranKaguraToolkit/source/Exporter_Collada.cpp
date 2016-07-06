#include "Exporter_Collada.h"

#include <iomanip>

ColladaExporter::ColladaExporter()
	: Exporter()
	, _dae_suffix(".dae") {
}

void ColladaExporter::save(const TMD::PP::Data_t& data) {
	boost::filesystem::path output_path(_export_folder);
	output_path /= "tmd";
	boost::filesystem::path dae_filename(output_path);
	dae_filename += _dae_suffix;

	std::ofstream dae_out;
	openToWrite(dae_out, dae_filename);
	LeftPad_t pad;
	write_document(dae_out, data, pad);
	dae_out.close();
}

boost::filesystem::path& ColladaExporter::dae_suffix() {
	return _dae_suffix;
}

void ColladaExporter::write_document(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad) {
	pad.level = 0;
	out << pad << "<COLLADA version=\"1.4\">" << std::endl;
	{
		pad++;
		if (_export_materials) {
			write_material_images(out, data, pad);
		}
		write_geometry(out, data, pad);
		pad--;
	}
	out << pad << "</COLLADA>" << std::endl;
}

void ColladaExporter::write_material_images(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad) {
	out << pad << "<library_images>" << std::endl;
	{
		pad++;
		auto matCtr(0U);
		for (const auto& mat : data.materials) {
			out << pad << "<image id=\"mat_" << std::setfill('0') << std::setw(2) << matCtr << "_img\" name=\"" << mat.package << "_" << mat.material_name << "\">" << std::endl;
			{
				pad++;
				out << pad << "<init_from>" << _material_resource_prefix << mat.package << "\\" << mat.material_name << _material_resource_suffix << "</init_from>" << std::endl;
				pad--;
			}
			out << pad << "</image>" << std::endl;
			matCtr++;
		}
		pad--;
	}
	out << pad << "</library_images>" << std::endl;
}

void ColladaExporter::write_geometry(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad) {
	out << pad << "<library_geometries>" << std::endl;
	{
		pad++;
		out << pad << "<geometry id=\"tmd_obj\" name=\"TMD Object\">" << std::endl;
		{
			pad++;
			out << pad << "<mesh>" << std::endl;
			{
				pad++;
				write_geometry_position(out, data, pad);
				if (_export_normals) {
					write_geometry_normal(out, data, pad);
				}
				if (_export_uvs) {
					write_geometry_uv(out, data, pad);
				}
				out << pad << "<vertices id=\"tmd_obj_vert\">" << std::endl;
				{
					pad++;
					out << pad << "<input semantic=\"POSITION\" source=\"#tmd_obj_pos\"/>" << std::endl;
					if (_export_normals) {
						out << pad << "<input semantic=\"NORMAL\" source=\"#tmd_obj_norm\"/>" << std::endl;
					}
					if (_export_normals) {
						out << pad << "<input semantic=\"TEXCOORD\" source=\"#tmd_obj_uv\"/>" << std::endl;
					}
					pad--;
				}
				out << pad << "</vertices>" << std::endl;

				for (const auto& mesh : data.meshes) {
					out << pad << "<triangles count=\"" << mesh.faces.size() << "\">" << std::endl;
					{
						pad++;
						out << pad << "<input semantic=\"VERTEX\" offset=\"0\" source=\"#tmd_obj_vert\"/>" << std::endl;
						out << pad << "<p>" << std::endl;
						for (const auto& face : mesh.faces) {
							out << pad;
							write_geometry_face(out, face, pad);
							out << std::endl;
						}
						out << pad << "</p>";
						pad--;
					}
					out << pad << "</triangles>" << std::endl;
				}
				pad--;
			}
			out << pad << "</mesh>" << std::endl;
			pad--;
		}
		out << pad << "</geometry>" << std::endl;
		pad--;
	}
	out << pad << "</library_geometries>" << std::endl;

	out << pad << "<library_visual_scenes>" << std::endl;
	{
		pad++;
		out << pad << "<visual_scene id=\"root_node\" name=\"root_node\">" << std::endl;
		{
			pad++;
			out << pad << "<node id=\"TMD_OBJECT\" name=\"TMD Object\">" << std::endl;
			{
				pad++;
				out << pad << "<instance_geometry url=\"#tmd_obj\">" << std::endl;
				out << pad << "</instance_geometry>" << std::endl;
				pad--;
			}
			out << pad << "</node>" << std::endl;
			pad--;
		}
		out << pad << "</visual_scene>" << std::endl;
		pad--;
	}
	out << pad << "</library_visual_scenes>" << std::endl;

	out << pad << "<scene>" << std::endl;
	{
		pad++;
		out << pad << "<instance_visual_scene url=\"#root_node\"/>" << std::endl;
		pad--;
	}
	out << pad << "</scene>" << std::endl;
}


void ColladaExporter::write_geometry_position(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad) {
	out << pad << "<source id=\"tmd_obj_pos\">" << std::endl;
	{
		pad++;
		const uint32_t floatCnt(data.vertices.size() * 3);
		out << pad << "<float_array id=\"tmd_obj_pos_array\" count=\"" << floatCnt << "\">" << std::endl;
		{
			pad++;
			for (const auto& vert : data.vertices) {
				out << pad << vert[0] << ' ' << vert[1] << ' ' << vert[2] << std::endl;
			}
			pad--;
		}
		out << pad << "</float_array>" << std::endl;
		write_geometry_accessor_3d(out, "#tmd_obj_pos_array", data.normals.size(), pad);
		pad--;
	}
	out << pad << "</source>" << std::endl;
}

void ColladaExporter::write_geometry_normal(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad) {
	out << pad << "<source id=\"tmd_obj_norm\">" << std::endl;
	{
		pad++;
		const uint32_t floatCnt(data.normals.size() * 3);
		out << pad << "<float_array id=\"tmd_obj_norm_array\" count=\"" << floatCnt << "\">" << std::endl;
		{
			pad++;
			auto flip_fac = 1;
			if (_flip_normals) {
				flip_fac = -1;
			}
			for (const auto& normal : data.normals) {
				out << pad
					<< static_cast<float>(flip_fac * normal[0]) << ' '
					<< static_cast<float>(flip_fac * normal[1]) << ' '
					<< static_cast<float>(flip_fac * normal[2]) << std::endl;
			}
			pad--;
		}
		out << pad << "</float_array>" << std::endl;
		write_geometry_accessor_3d(out, "#tmd_obj_norm_array", data.normals.size(), pad);
		pad--;
	}
	out << pad << "</source>" << std::endl;
}


void ColladaExporter::write_geometry_uv(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad) {
	out << pad << "<source id=\"tmd_obj_uv\">" << std::endl;
	{
		pad++;
		const uint32_t floatCnt(data.uvs.size() * 2);
		out << pad << "<float_array id=\"tmd_obj_uv_array\" count=\"" << floatCnt << "\">" << std::endl;
		{
			pad++;
			for (const auto& uv : data.uvs) {
				out << pad
					<< static_cast<float>(uv[0]) << ' '
					<< static_cast<float>(uv[1]) << std::endl;
			}
			pad--;
		}
		out << pad << "</float_array>" << std::endl;
		write_geometry_accessor_uv(out, "#tmd_obj_uv_array", data.uvs.size(), pad);
		pad--;
	}
	out << pad << "</source>" << std::endl;
}

void ColladaExporter::write_geometry_face(std::ostream& out, const std::array<uint16_t, 3>& face, LeftPad_t& pad) {
	//out << pad << "<p>";
	out << face[0] << ' ' << face[1] << ' ' << face[2];
	/*
	if (_export_normals) {
		out << ' ' << face[0] << ' ' << face[1] << ' ' << face[2];
	}
	if (_export_uvs) {
		out << ' ' << face[0] << ' ' << face[1] << ' ' << face[2];
	}
	*/
	//out << "</p>";
}

void ColladaExporter::write_geometry_accessor_3d(std::ostream& out, const std::string& source_id, uint32_t count, LeftPad_t& pad) {
	out << pad << "<technique_common>" << std::endl;
	{
		pad++;
		out << pad << "<accessor source=\"" << source_id << "\" offset=\"0\" count=\"" << count << "\" stride=\"3\">" << std::endl;
		{
			pad++;
			out << pad << "<param name=\"X\" type=\"float\"/>" << std::endl;
			out << pad << "<param name=\"Y\" type=\"float\"/>" << std::endl;
			out << pad << "<param name=\"Z\" type=\"float\"/>" << std::endl;
			pad--;
		}
		out << pad << "</accessor>" << std::endl;
		pad--;
	}
	out << pad << "</technique_common>" << std::endl;
}

void ColladaExporter::write_geometry_accessor_uv(std::ostream& out, const std::string& source_id, uint32_t count, LeftPad_t& pad) {
	out << pad << "<technique_common>" << std::endl;
	{
		pad++;
		out << pad << "<accessor source=\"" << source_id << "\" offset=\"0\" count=\"" << count << "\" stride=\"2\">" << std::endl;
		{
			pad++;
			out << pad << "<param name=\"S\" type=\"float\"/>" << std::endl;
			out << pad << "<param name=\"T\" type=\"float\"/>" << std::endl;
			pad--;
		}
		out << pad << "</accessor>" << std::endl;
		pad--;
	}
	out << pad << "</technique_common>" << std::endl;
}

ColladaExporter::LeftPad_t& ColladaExporter::LeftPad_t::operator++(int) {
	level++;
	return *this;
}

ColladaExporter::LeftPad_t& ColladaExporter::LeftPad_t::operator--(int) {
	level--;
	return *this;
}

std::ostream& operator<<(std::ostream& out, const ColladaExporter::LeftPad_t& pad) {
	for (auto i(0U); i < pad.level; i++) {
		out << "\t";
	}
	return out;
}
