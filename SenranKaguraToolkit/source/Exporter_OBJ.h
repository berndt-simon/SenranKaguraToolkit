#pragma once

#include <string>

#include "Exporter_Base.h"

class ObjExporter : public Exporter {
public:
	ObjExporter();
	~ObjExporter() = default;
	
	boost::filesystem::path& obj_suffix();
	boost::filesystem::path& mtl_suffix();

	void save(const TMD::PP::Data_t& data) override;

protected:
	void write_mtl(std::ostream& file, const TMD::PP::Data_t& data);
	void write_obj(std::ostream& file, const TMD::PP::Data_t& data);

	void write_face_vertex(std::ostream& file, const std::array<uint16_t, 3>& vertices, uint8_t idx);

private:
	boost::filesystem::path _obj_suffix;
	boost::filesystem::path _mtl_suffix;
	
};