#include "Exporter_Collada.h"

#include <iomanip>

ColladaExporter::ColladaExporter()
	: Exporter()
	, _dae_suffix(".dae") {
}

boost::filesystem::path& ColladaExporter::dae_suffix() {
	_dae_suffix;
}
const boost::filesystem::path& ColladaExporter::dae_suffix() const {
	_dae_suffix;
}

void ColladaExporter::save(const TMD::PostProcessed::Data_t& data) {
	boost::filesystem::path output_path(_export_folder);
	output_path /= "tmd";
	boost::filesystem::path dae_filename(output_path);
	dae_filename += _dae_suffix;

	std::ofstream dae_out;
	open_to_write(dae_out, dae_filename);
	LeftPad_t pad{ 0 };
	PaddedOstream pad_out(dae_out, pad);
	write_document(pad_out, data);
	dae_out.close();
}

void ColladaExporter::write_asset_header(PaddedOstream& out) {
	out << "<asset>" << std::endl;
	{
		Block_t b(out.padding());
		out << "<up_axis>" << "Y_UP" << "</up_axis>" << std::endl;
	}
	out << "</asset>" << std::endl;
}

void ColladaExporter::write_document(PaddedOstream& out, const TMD::PostProcessed::Data_t& data) {
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
	out << "<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">" << std::endl;
	{
		write_asset_header(out);
		Block_t b(out.padding());
		if (_export_materials) {
			write_material_images(out, data);
		}
		write_geometry(out, data);
	}
	out << "</COLLADA>" << std::endl;
}

void ColladaExporter::write_material_images(PaddedOstream& out, const TMD::PostProcessed::Data_t& data) {
	out << "<library_images>" << std::endl;
	{
		Block_t b(out.padding());
		auto matCtr(0U);
		for (const auto& mat : data.materials) {
			out << "<image id=\"mat_" << std::setfill('0') << std::setw(2) << matCtr << "_img\" name=\"" << mat.package << "_" << mat.material_name << "\">" << std::endl;
			{
				Block_t b(out.padding());
				out << "<init_from>" << _material_resource_prefix << mat.package << "\\" << mat.material_name << _material_resource_suffix << "</init_from>" << std::endl;
			}
			out << "</image>" << std::endl;
			matCtr++;
		}
	}
	out << "</library_images>" << std::endl;
}

void ColladaExporter::write_geometry(PaddedOstream& out, const TMD::PostProcessed::Data_t& data) {
	out << "<library_geometries>" << std::endl;
	{
		Block_t b(out.padding());
		out << "<geometry id=\"tmd_obj\" name=\"TMD Object\">" << std::endl;
		{
			Block_t b(out.padding());
			out << "<mesh>" << std::endl;
			{
				Block_t b(out.padding());
				write_geometry_position(out, data);
				if (_export_normals) {
					write_geometry_normal(out, data);
				}
				if (_export_uvs) {
					write_geometry_uv(out, data);
				}
				out << "<vertices id=\"tmd_obj_vert\">" << std::endl;
				{
					Block_t b(out.padding());
					out << "<input semantic=\"POSITION\" source=\"#tmd_obj_pos\"/>" << std::endl;
					if (_export_normals) {
						out << "<input semantic=\"NORMAL\" source=\"#tmd_obj_norm\"/>" << std::endl;
					}
					if (_export_normals) {
						out << "<input semantic=\"TEXCOORD\" source=\"#tmd_obj_uv\"/>" << std::endl;
					}
				}
				out << "</vertices>" << std::endl;

				for (const auto& mesh : data.meshes) {
					out << "<triangles count=\"" << mesh.faces.size() << "\">" << std::endl;
					{
						Block_t b(out.padding());
						out << "<input semantic=\"VERTEX\" offset=\"0\" source=\"#tmd_obj_vert\"/>" << std::endl;
						out << "<p>" << std::endl;
						{
							Block_t b(out.padding());
							out << "";
							for (const auto& face : mesh.faces) {
								static_cast<std::ostream&>(out) << face[0] << ' ' << face[1] << ' ' << face[2] << ' ';
							}
						}
						static_cast<std::ostream&>(out) << std::endl;
						out << "</p>";
					}
					out << "</triangles>" << std::endl;
				}

			}
			out << "</mesh>" << std::endl;

		}
		out << "</geometry>" << std::endl;

	}
	out << "</library_geometries>" << std::endl;

	out << "<library_visual_scenes>" << std::endl;
	{
		Block_t b(out.padding());
		out << "<visual_scene id=\"root_node\" name=\"root_node\">" << std::endl;
		{
			Block_t b(out.padding());
			out << "<node id=\"TMD_OBJECT\" name=\"TMD Object\">" << std::endl;
			{
				Block_t b(out.padding());
				out << "<instance_geometry url=\"#tmd_obj\">" << std::endl;
				out << "</instance_geometry>" << std::endl;
			}
			out << "</node>" << std::endl;
		}
		out << "</visual_scene>" << std::endl;
	}
	out << "</library_visual_scenes>" << std::endl;

	out << "<scene>" << std::endl;
	{
		Block_t b(out.padding());
		out << "<instance_visual_scene url=\"#root_node\"/>" << std::endl;
	}
	out << "</scene>" << std::endl;
}


void ColladaExporter::write_geometry_position(PaddedOstream& out, const TMD::PostProcessed::Data_t& data) {
	out << "<source id=\"tmd_obj_pos\">" << std::endl;
	{
		Block_t b(out.padding());
		const uint32_t floatCnt(data.vertices.size() * 3);
		out << "<float_array id=\"tmd_obj_pos_array\" count=\"" << floatCnt << "\">" << std::endl;
		{
			Block_t b(out.padding());
			out << "";
			for (const auto& vert : data.vertices) {
				static_cast<std::ostream&>(out) 
					<< vert[0] << ' ' 
					<< vert[1] << ' ' 
					<< vert[2] << ' ';
			} 
			static_cast<std::ostream&>(out) << std::endl;
		}
		out << "</float_array>" << std::endl;
		write_geometry_accessor_3d(out, "#tmd_obj_pos_array", data.normals.size());
	}
	out << "</source>" << std::endl;
}

void ColladaExporter::write_geometry_normal(PaddedOstream& out, const TMD::PostProcessed::Data_t& data) {
	out << "<source id=\"tmd_obj_norm\">" << std::endl;
	{
		Block_t b(out.padding());
		const uint32_t floatCnt(data.normals.size() * 3);
		out << "<float_array id=\"tmd_obj_norm_array\" count=\"" << floatCnt << "\">" << std::endl;
		{
			Block_t b(out.padding());
			auto flip_fac = 1;
			if (_flip_normals) {
				flip_fac = -1;
			}
			out << "";
			for (const auto& normal : data.normals) {
				static_cast<std::ostream&>(out) 
					<< static_cast<float>(flip_fac * normal[0]) << ' '
					<< static_cast<float>(flip_fac * normal[1]) << ' '
					<< static_cast<float>(flip_fac * normal[2]) << ' ';
			}
			static_cast<std::ostream&>(out) << std::endl;
		}
		out << "</float_array>" << std::endl;
		write_geometry_accessor_3d(out, "#tmd_obj_norm_array", data.normals.size());
	}
	out << "</source>" << std::endl;
}

static std::array<float, 2> normalize_uv_coords(const std::array<int16_t, 2>& uv_int) {
	return std::array<float, 2>{static_cast<float>(uv_int[0]) / 1024, static_cast<float>(uv_int[1]) / -1024};
}

void ColladaExporter::write_geometry_uv(PaddedOstream& out, const TMD::PostProcessed::Data_t& data) {
	out << "<source id=\"tmd_obj_uv\">" << std::endl;
	{
		Block_t b(out.padding());
		const uint32_t floatCnt(data.uvs.size() * 2);
		out << "<float_array id=\"tmd_obj_uv_array\" count=\"" << floatCnt << "\">" << std::endl;
		{
			Block_t b(out.padding());
			out << "";
			for (const auto& uv_int : data.uvs) {
				const auto uv = normalize_uv_coords(uv_int);
				static_cast<std::ostream&>(out)
					<< uv[0] << ' ' << uv[1] << ' ';
			}
			static_cast<std::ostream&>(out) << std::endl;
		}
		out << "</float_array>" << std::endl;
		write_geometry_accessor_uv(out, "#tmd_obj_uv_array", data.uvs.size());
	}
	out << "</source>" << std::endl;
}

void ColladaExporter::write_geometry_accessor_3d(PaddedOstream& out, const std::string& source_id, uint32_t count) {
	out << "<technique_common>" << std::endl;
	{
		Block_t b(out.padding());
		out << "<accessor source=\"" << source_id << "\" offset=\"0\" count=\"" << count << "\" stride=\"3\">" << std::endl;
		{
			Block_t b(out.padding());
			out << "<param name=\"X\" type=\"float\"/>" << std::endl;
			out << "<param name=\"Y\" type=\"float\"/>" << std::endl;
			out << "<param name=\"Z\" type=\"float\"/>" << std::endl;
		}
		out << "</accessor>" << std::endl;
	}
	out << "</technique_common>" << std::endl;
}

void ColladaExporter::write_geometry_accessor_uv(PaddedOstream& out, const std::string& source_id, uint32_t count) {
	out << "<technique_common>" << std::endl;
	{
		Block_t b(out.padding());
		out << "<accessor source=\"" << source_id << "\" offset=\"0\" count=\"" << count << "\" stride=\"2\">" << std::endl;
		{
			Block_t b(out.padding());
			out << "<param name=\"S\" type=\"float\"/>" << std::endl;
			out << "<param name=\"T\" type=\"float\"/>" << std::endl;
		}
		out << "</accessor>" << std::endl;
	}
	out << "</technique_common>" << std::endl;
}

ColladaExporter::LeftPad_t& ColladaExporter::LeftPad_t::operator++() {
	level++;
	return *this;
}

ColladaExporter::LeftPad_t& ColladaExporter::LeftPad_t::operator--() {
	level--;
	return *this;
}

ColladaExporter::PaddedOstream::PaddedOstream(std::ostream& out, LeftPad_t& padding)
	: _out(out)
	, _padding(padding) {
}

ColladaExporter::PaddedOstream::operator std::ostream&() {
	return _out;
}
ColladaExporter::PaddedOstream::operator const std::ostream&() const {
	return _out;
}
const ColladaExporter::PaddedOstream& ColladaExporter::PaddedOstream::operator<<(std::ostream& (*func)(std::ostream&)) const {
	func(_out);
	return *this;
}

ColladaExporter::LeftPad_t& ColladaExporter::PaddedOstream::padding() {
	return _padding;
}
const ColladaExporter::LeftPad_t& ColladaExporter::PaddedOstream::padding() const {
	return _padding;
}

ColladaExporter::Block_t::Block_t(LeftPad_t& padding)
	: _padding(++padding) {
}

ColladaExporter::Block_t::~Block_t() {
	--_padding;
}
