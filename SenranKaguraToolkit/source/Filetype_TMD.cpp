#include "Filetype_TMD.h"

#include "FileProcessing.h"
#include <fstream>



void processTMD(std::ifstream& file) {
	const std::streampos fileStart = file.tellg();

	TMD_Header header;
	read(file, &header);
}