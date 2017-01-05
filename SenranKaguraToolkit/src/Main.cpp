#include <tclap\CmdLine.h>

#include <iostream>
#include <iomanip>
#include <string>

#include "FileProcessing.h"
#include <fstream>
#include <sstream>

#include "CAT.h"
#include "GXT.h"
#include "TMD.h"
#include "Filename.h"

#include "Exporter_OBJ.h"
#include "Exporter_Collada.h"
#include "ResourceDumper.h"

#define EXPORTER_OBJ

static bool hasEnding(const std::string& string, const std::string& ending) {
	if (string.length() >= ending.length()) {
		return (0 == string.compare(string.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

static void process_gxt(std::istream& file, const std::streamoff offset, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& entries, const boost::filesystem::path& out_folder) {
	using namespace GXT;
	std::vector<Entry_t> gxt_entries;
	load(file, offset, entries, gxt_entries);
	ResourceDumper dumper;
	dumper.output_prefix() = out_folder;
	dumper.output_prefix() /= "img";
	dumper.output_suffix() = ".dds";
	for (const auto& gxt_entry : gxt_entries) {
		dumper.dump({ gxt_entry.package, "/", gxt_entry.resource }, gxt_entry.data);
		std::cout << "Dump GXT Resource: " << gxt_entry.package << "/" << gxt_entry.resource << std::endl;
	}
}

static void process_gxt_raw(const boost::filesystem::path& in_file, const boost::filesystem::path& out_folder) {
	using namespace GXT;
	std::ifstream file;
	open_to_read(file, in_file.string());
	std::vector<blob_t> gxt_entries;
	load_raw(file, 0, gxt_entries);
	ResourceDumper dumper;
	dumper.output_prefix() = out_folder;
	dumper.output_suffix() = ".dds";
	for (auto i(0U); i < gxt_entries.size(); ++i) {
		const auto& gxt_entry = gxt_entries[i];
		std::stringstream number;
		number << '_' << std::setw(2) << std::setfill('0') << i;
		dumper.dump({ in_file.stem().string() + number.str() }, gxt_entry);
		std::cout << "Dump GXT Resource: " << in_file.stem().string() << number.str() << std::endl;
	}
}

static void process_tmd(std::istream& file, const std::streamoff offset, const std::vector<CAT::ResourceEntry_t::SubEntry_t>& references, const boost::filesystem::path& out_folder, uint32_t cntr = 0U) {
	using namespace TMD;
	RAW::Data_t tmd_data_raw;	
	load_raw(file, offset, tmd_data_raw); 
	PostProcessed::Data_t tmd_data_pp;
	post_process(tmd_data_raw, tmd_data_pp);

#ifdef EXPORTER_OBJ
	ObjExporter exporter;
#else // EXPORTER_OBJ
	ColladaExporter exporter;
	exporter.material_export() = true;
#endif // !EXPORTER_OBJ
	exporter.flip_normals() = true;
	exporter.rescale_factor() = 0.01f;
	exporter.export_folder() = out_folder;
	exporter.material_resource_prefix() = "..\\img\\";
	std::stringstream obj_folder_name;
	obj_folder_name << "obj_" << std::setfill('0') << std::setw(2) << cntr;
	exporter.export_folder() /= obj_folder_name.str();

	exporter.save(tmd_data_pp, references);
#ifdef EXPORTER_OBJ
	std::cout << "Dumped TMD Resources to Obj " << std::endl;
#else // EXPORTER_OBJ
	std::cout << "Dumped TMD Resources to Collada " << std::endl;
#endif // !EXPORTER_OBJ

}

static void process_cat(const boost::filesystem::path& in_file, const boost::filesystem::path& out_folder) {
	std::ifstream file;
	open_to_read(file, in_file.string());
	std::vector<CAT::ResourceEntry_t> cat_entries;

	CAT::load(file, cat_entries);
	uint32_t tmd_cntr(0U);
	for (const auto& entry : cat_entries) {
		switch (entry.type) {
			case ElementType_e::GXT:
				std::cout << "Load GXT Sub-File" << std::endl;
				process_gxt(file, entry.offset, entry.sub_entries, out_folder);
				break;
			case ElementType_e::TMD:
			case ElementType_e::TMD_TOON:
				std::cout << "Load TMD Sub-File" << std::endl;
				process_tmd(file, entry.offset, entry.sub_entries, out_folder, tmd_cntr);
				tmd_cntr++;
				break;
		}
	}
	file.close();
}

static void process_bin(const boost::filesystem::path& in_file, const boost::filesystem::path& out_folder) {
	using namespace Filename;
	std::ifstream file;
	open_to_read(file, in_file.string());
	std::vector<std::string> filenames;
	load(file, filenames);
	file.close();

	std::cout << "Contend of File:" << std::endl;
	std::ofstream file_out;
	file_out.open((out_folder / "filenames.txt").c_str(), std::ios::out);
	for (const auto& filename : filenames) {
		std::cout << "\t" << filename << std::endl;
		file_out << filename << std::endl;
	}
	file_out.close();
}

static void process_auto(const boost::filesystem::path& in_file, const boost::filesystem::path& out_folder) {
	std::cout << "Try to load '" << in_file << "'..." << std::endl;
	std::string filename_lc = in_file.string();
	std::transform(filename_lc.begin(), filename_lc.end(), filename_lc.begin(), tolower);
	std::cout << "Check File-Type..." << std::endl << "\t";
	if (hasEnding(filename_lc, ".cat")) {
		std::cout << "'" << in_file << "' seems to be a CAT-File." << std::endl;
		process_cat(in_file, out_folder);
	} else if (hasEnding(filename_lc, ".gxt")) {
		std::cout << "'" << in_file << "' seems to be a GXT-File." << std::endl;
		process_gxt_raw(in_file, out_folder);
	} else if (hasEnding(filename_lc, "filename.bin")) {
		std::cout << "'" << in_file << "' seems to be the Filename-File." << std::endl;
		process_bin(in_file, out_folder);
	} else {
		std::cout << "can't recognize File-Type" << std::endl;
	}
}

static void process_batch(const boost::filesystem::path& in_file, const boost::filesystem::path& out_folder) {
	std::cout << "Try to Batch-Process Content of '" << in_file << "'..." << std::endl;
	std::ifstream batchfile;
	batchfile.open(in_file.c_str(), std::ios::in);
	std::string line_filename;
	while (std::getline(batchfile, line_filename)) {
		try {
			const auto abs_path(boost::filesystem::canonical(line_filename, in_file.parent_path()));
			try {
				process_auto(abs_path, out_folder);
			} catch (const std::exception&) {
				std::cerr << "Can't process '" << abs_path << "'!" << std::endl;
			}
			
		} catch (const std::exception&) {
			std::cerr << "Can't find  '" << line_filename << "'!" << std::endl;
		}
	}
}

int main(int argc, char** argv) {


	// Cmd Handling
	try {
		const std::string default_out_folder("out");
		TCLAP::CmdLine cmd("SenranKagura-Toolkit", ' ', "0.3");
		TCLAP::ValueArg<std::string> outFolderArg("o", "output-folder", "Output-Folder - defaults to '" + default_out_folder + "'", false, default_out_folder, "Path as String");
		TCLAP::ValueArg<std::string> inFileArg("i", "input-file", "Input-File with auto type-recognition", true, "", "Filepath as String");
		TCLAP::SwitchArg autoClose("s", "silent", "Silent Mode", false);
		TCLAP::SwitchArg batch("b", "batch", "Batch Mode", false);
		cmd.add(inFileArg);
		cmd.add(outFolderArg);
		cmd.add(autoClose);
		cmd.add(batch);
		cmd.parse(argc, argv);

		boost::filesystem::path inFile(inFileArg.getValue());
		boost::filesystem::path outFolder(outFolderArg.getValue());

		if (batch.getValue()) {
			process_batch(inFile, outFolder);
		} else {
			process_auto(inFile, outFolder);
		}		

		if (!autoClose.getValue()) {
			// Uncomment the following two Lines to keep the CMD open
			std::cout << "Press [ENTER] to close" << std::endl;
			std::cin.ignore();
		}

	} catch (TCLAP::ArgException& e) {
		std::cerr << "Error while parsing Cmd-Args: " << e.error() << " for Argument " << e.argId() << std::endl;
	}
	return EXIT_SUCCESS;
}