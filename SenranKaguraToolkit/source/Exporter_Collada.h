#pragma once

#include "Exporter_Base.h"

class ColladaExporter :  public Exporter {
public:
	ColladaExporter();
	~ColladaExporter() = default;
	
	void save(const TMD::PP::Data_t& data) override;

	boost::filesystem::path& dae_suffix();
	
private:

	void write_document(std::ostream& out, const TMD::PP::Data_t& data);

	boost::filesystem::path _dae_suffix;

};
