#include <tclap\CmdLine.h>

#include <iostream>
#include <string>

#include "FileProcessing.h"
#include <fstream>

#include "Filetype_CAT.h"
#include "Filetype_GXT.h"
#include "Filetype_TMD.h"
#include "Filename.h"

#include "ResourceDumper.h"


bool hasEnding(const std::string& string, const std::string& ending) {
	if (string.length() >= ending.length()) {
		return (0 == string.compare(string.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

void process(const boost::filesystem::path& in_file, boost::filesystem::path out_folder) {

	std::cout << "Try to load '" << in_file << "'..." << std::endl;
	std::string filename_lc = in_file.string();
	std::transform(filename_lc.begin(), filename_lc.end(), filename_lc.begin(), tolower);
	if (hasEnding(filename_lc, ".cat")) {
		std::cout << "'" << in_file << "' seems to be a CAT-File." << std::endl;
		std::ifstream file;
		openToRead(file, in_file.string());
		std::vector<CAT_Resource_Entry> cat_entries;
		processCAT(file, cat_entries);
		for (auto entryItter(cat_entries.begin()); entryItter != cat_entries.end(); entryItter++) {
			switch (entryItter->type) {
				case CAT_Resource_Entry::GXT:
				{
					std::vector<GXT_Entry> gxt_entries;
					processGXT(file, entryItter->offset, entryItter->sub_entries, gxt_entries);
					ResourceDumper dumper;
					dumper.outputPrefix() = out_folder;
					dumper.outputPostfix() = ".dds";
					for (auto gxt_entry(gxt_entries.begin()); gxt_entry != gxt_entries.end(); gxt_entry++) {
						dumper.dump({ "./", gxt_entry->package, "/", gxt_entry->resource }, gxt_entry->data);
					}
				}
				break;
				case CAT_Resource_Entry::TMD:
				case CAT_Resource_Entry::TMD_TOON:
				{
					TMD_RAW_Data tmd_data;
					processTMD(file, entryItter->offset, tmd_data);
					std::ofstream obj_out;
					openToWrite(obj_out, "tmd.obj");
					obj_out << std::dec;
					obj_out << "# Converted TMD" << std::endl;
					obj_out << "o TMD_Object" << std::endl;

					for (auto vItt(tmd_data.vertices.begin()); vItt != tmd_data.vertices.end(); vItt++) {
						obj_out << "v "
							<< vItt->pos[0] << " "
							<< vItt->pos[1] << " "
							<< vItt->pos[2] << " " << std::endl;
					}
					for (auto vItt(tmd_data.vertices.begin()); vItt != tmd_data.vertices.end(); vItt++) {
						obj_out << "vt "
							<< static_cast<float>(vItt->tex[0]) / 1024 << " "
							<< static_cast<float>(vItt->tex[1]) / -1024 << std::endl;
					}
					for (auto vItt(tmd_data.vertices.begin()); vItt != tmd_data.vertices.end(); vItt++) {
						obj_out << "vn "
							<< static_cast<float>(vItt->normal[0]) << " "
							<< static_cast<float>(vItt->normal[1]) << " "
							<< static_cast<float>(vItt->normal[2]) << " " << std::endl;
					}
					

					for (auto fItt(tmd_data.faces.begin()); fItt != tmd_data.faces.end(); fItt++) {
						obj_out << "f ";
						obj_out << fItt->vertex_index[0] + 1 << "/" << fItt->vertex_index[0] + 1 << "/" << fItt->vertex_index[0] + 1 << " ";
						obj_out << fItt->vertex_index[1] + 1 << "/" << fItt->vertex_index[1] + 1 << "/" << fItt->vertex_index[1] + 1 << " ";
						obj_out << fItt->vertex_index[2] + 1 << "/" << fItt->vertex_index[2] + 1 << "/" << fItt->vertex_index[2] + 1 << std::endl;
					}
					obj_out.close();
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
		readFilenames(file, filenames);
		file.close();

		const boost::filesystem::path parent = in_file.parent_path();

		std::cout << "Contend of File:" << std::endl;
		for (auto fn_itter(filenames.begin()); fn_itter != filenames.end(); fn_itter++) {
			std::cout << "\t" << *fn_itter << std::endl;
		}
	}
}

int main(int argc, char** argv) {

	TCLAP::CmdLine cmd("Senran Kagura Shinobi Versus Toolkit", ' ', "0.42");
	TCLAP::ValueArg<std::string> inFileArg("i", "input-file", "Input-File", true, "", "string");
	TCLAP::ValueArg<std::string> outFolderArg("o", "output-folder", "Output-Folder", false, "out", "string");
	cmd.add(inFileArg);
	cmd.add(outFolderArg);
	cmd.parse(argc, argv);

	boost::filesystem::path inFile(inFileArg.getValue());
	boost::filesystem::path outFolder(outFolderArg.getValue());

	process(inFile, outFolder);

	std::cin.ignore();
	return EXIT_SUCCESS;
}