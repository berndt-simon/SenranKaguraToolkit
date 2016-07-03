#include "Exporter_Collada.h"

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
	write_document(dae_out, data);
	dae_out.close();
}

boost::filesystem::path& ColladaExporter::dae_suffix() {
	return _dae_suffix;
}

void ColladaExporter::write_document(std::ostream& out, const TMD::PP::Data_t& data) {
	out << "<COLLADA version=\"1.4\">" << std::endl;

	out << "</COLLADA>" << std::endl;
}
