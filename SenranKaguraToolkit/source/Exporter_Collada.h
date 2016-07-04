#pragma once

#include "Exporter_Base.h"

class ColladaExporter :  public Exporter {
public:
	ColladaExporter();
	~ColladaExporter() = default;
	
	void save(const TMD::PP::Data_t& data) override;

	boost::filesystem::path& dae_suffix();
	
private:
	struct LeftPad_t {
		uint8_t level = 0;

		LeftPad_t& operator++(int);
		LeftPad_t& operator--(int);
	};

	void write_document(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad);
	void write_material_images(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad);

	void write_geometry(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad);
	void write_geometry_position(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad);
	void write_geometry_normal(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad);
	void write_geometry_uv(std::ostream& out, const TMD::PP::Data_t& data, LeftPad_t& pad);

	void write_geometry_face(std::ostream& out, const std::array<uint16_t, 3>& face, LeftPad_t& pad);

	void write_geometry_accessor_3d(std::ostream& out, const std::string& source_id, uint32_t count, LeftPad_t& pad);
	void write_geometry_accessor_uv(std::ostream& out, const std::string& source_id, uint32_t count, LeftPad_t& pad);

	boost::filesystem::path _dae_suffix;


	friend	std::ostream& operator<<(std::ostream& out, const LeftPad_t& pad);

};

