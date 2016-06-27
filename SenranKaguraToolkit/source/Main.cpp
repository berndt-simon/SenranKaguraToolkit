#include <tclap\CmdLine.h>

#include <iostream>
#include <string>

#include "FileProcessing.h"
#include <fstream>

#include "Filetype_CAT.h"
#include "Filename.h"


bool hasEnding(const std::string& string, const std::string& ending) {
	if (string.length() >= ending.length()) {
		return (0 == string.compare(string.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

int main(int argc, char** argv) {

	TCLAP::CmdLine cmd("Senran Kagura Shinobi Versus Toolkit", ' ', "0.42");
	TCLAP::ValueArg<std::string> file("i", "input-file", "Input-File", true, "", "string");
	cmd.add(file);
	cmd.parse(argc, argv);

	std::cout << "Try to load '" << file.getValue() << "'..." << std::endl;
	const std::string& filename = file.getValue();
	std::string filename_lc = filename;
	std::transform(filename_lc.begin(), filename_lc.end(), filename_lc.begin(), tolower);
	if (hasEnding(filename_lc, ".cat")) {
		std::cout << "'" << filename << "' seems to be a CAT-File." << std::endl;
		std::ifstream file;
		open(file, filename);
		processCAT(file);
	}
	if (hasEnding(filename_lc, ".bin")) {
		std::cout << "'" << filename << "' seems to be a Bin-File." << std::endl;
		std::ifstream file;
		open(file, filename);
		readFilenames(file);
	}

	std::cin.ignore();
	return EXIT_SUCCESS;
}