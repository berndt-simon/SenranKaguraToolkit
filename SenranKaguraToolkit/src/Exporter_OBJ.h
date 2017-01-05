#pragma once

#include "Exporter_Base.h"

class ObjExporter : public Exporter {
public:
	ObjExporter();
	~ObjExporter() = default;
	
	boost::filesystem::path& obj_suffix();
	const boost::filesystem::path& obj_suffix() const;

	boost::filesystem::path& mtl_suffix();
	const boost::filesystem::path& mtl_suffix() const;

	void save(const TMD::PostProcessed::Data_t& data, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& references) override;

private:
	void write_mtl(std::ostream& file, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& references);
	void write_obj(std::ostream& file, const TMD::PostProcessed::Data_t& data);

	void write_face_vertex(std::ostream& file, const std::array<uint16_t, 3>& vertices, uint8_t idx);

private:
	boost::filesystem::path _obj_suffix;
	boost::filesystem::path _mtl_suffix;
	
};