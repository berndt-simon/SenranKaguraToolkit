#include <tclap\CmdLine.h>

#include <iostream>
#include <iomanip>
#include <string>

#include "FileProcessing.h"
#include <fstream>
#include <sstream>

#include "Filetype_CAT.h"
#include "Filetype_GXT.h"
#include "Filetype_TMD.h"
#include "Filename.h"

#include "Exporter_OBJ.h"
#include "Exporter_Collada.h"
#include "ResourceDumper.h"

#define EXPORTER_OBJ

bool hasEnding(const std::string& string, const std::string& ending) {
	if (string.length() >= ending.length()) {
		return (0 == string.compare(string.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

void process(const boost::filesystem::path& in_file, const boost::filesystem::path& out_folder) {

	std::cout << "Try to load '" << in_file << "'..." << std::endl;
	std::string filename_lc = in_file.string();
	std::transform(filename_lc.begin(), filename_lc.end(), filename_lc.begin(), tolower);
	if (hasEnding(filename_lc, ".cat")) {
		std::cout << "'" << in_file << "' seems to be a CAT-File." << std::endl;
		std::ifstream file;
		openToRead(file, in_file.string());
		std::vector<CAT::ResourceEntry_t> cat_entries;
		CAT::load(file, cat_entries, true);
		uint32_t cntr(0U);
		for (auto entryItter(cat_entries.begin()); entryItter != cat_entries.end(); entryItter++) {
			switch (entryItter->type) {
				case CAT::ResourceEntry_t::GXT:
					{	
						std::cout << "Load GXT Sub-File" << std::endl;
						std::vector<GXT::Entry_t> gxt_entries;
						GXT::load(file, entryItter->offset, entryItter->sub_entries, gxt_entries);
						ResourceDumper dumper;
						dumper.output_prefix() = out_folder;
						dumper.output_prefix() /= "img";
						dumper.output_suffix() = ".dds";
						for (auto gxt_entry(gxt_entries.begin()); gxt_entry != gxt_entries.end(); gxt_entry++) {
							dumper.dump({ gxt_entry->package, "/", gxt_entry->resource }, gxt_entry->data);
							std::cout << "Dump GXT Resource: " << gxt_entry->package << "/" << gxt_entry->resource << std::endl;
						}
					}
					break;
				case CAT::ResourceEntry_t::TMD:
				case CAT::ResourceEntry_t::TMD_TOON:
					{
						std::cout << "Load TMD Sub-File" << std::endl;
						TMD::RAW::Data_t tmd_data_raw;
						TMD::load_raw(file, entryItter->offset, tmd_data_raw);
						TMD::PP::Data_t tmd_data_pp;
						TMD::post_process(tmd_data_raw, entryItter->sub_entries, tmd_data_pp);

#ifdef EXPORTER_OBJ
						ObjExporter exporter;
						exporter.mtl_resource_prefix() = "..\\img\\";
#else
						ColladaExporter exporter;
#endif // EXPORTER_OBJ
						exporter.set_flip_normals(true);
						exporter.set_rescale_factor(0.01f);
						exporter.export_folder() = out_folder;
						std::stringstream obj_folder_name;
						obj_folder_name << "obj_" << std::setfill('0') << std::setw(2) << cntr;
						exporter.export_folder() /= obj_folder_name.str();
						
						exporter.save(tmd_data_pp);
						std::cout << "Dump TMD Resources to Obj " << std::endl;
						cntr++;
					}
					break;
			}
		}
		file.close();
	}
	if (hasEnding(filename_lc, "filename.bin")) {
		std::cout << "'" << in_file << "' seems to be the Filename-File." << std::endl;
		std::ifstream file;
		openToRead(file, in_file.string());
		std::vector<std::string> filenames;
		Filename::load(file, filenames);
		file.close();

		const boost::filesystem::path parent = in_file.parent_path();

		std::cout << "Contend of File:" << std::endl;
		for (auto fn_itter(filenames.begin()); fn_itter != filenames.end(); fn_itter++) {
			std::cout << "\t" << *fn_itter << std::endl;
		}
	}
}

int main(int argc, char** argv) {


	// Cmd Handling
	try {
		TCLAP::CmdLine cmd("Senran Kagura Shinobi Versus Toolkit", ' ', "0.42");
		TCLAP::ValueArg<std::string> inFileArg("i", "input-file", "Input-File", true, "", "string");
		TCLAP::ValueArg<std::string> outFolderArg("o", "output-folder", "Output-Folder", false, "out", "string");
		cmd.add(inFileArg);
		cmd.add(outFolderArg);
		cmd.parse(argc, argv);

		boost::filesystem::path inFile(inFileArg.getValue());
		boost::filesystem::path outFolder(outFolderArg.getValue());
		
		process(inFile, outFolder);
	} catch (TCLAP::ArgException &e) {
		std::cerr << "Error while parsing Cmd-Args: " << e.error() << " for Argument " << e.argId() << std::endl;
	}

	// Uncomment the following two Lines to keep the CMD open
	std::cout << "press [ENTER] to close" << std::endl;
	std::cin.ignore();
	return EXIT_SUCCESS;
}