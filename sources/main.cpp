#include <fstream>
#include <iostream>
#include <string>

#include "id3/AudioID3Tag.h"
#include "id3/AudioID3Error.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Argument missing!" << std::endl;
		return 1;
	}

	std::ifstream input(argv[1], std::ifstream::in|std::ifstream::binary);

	try {
		ID3Tag tag;

		tag.init(input);

		std::cout 
		<< "version:             " << tag.version() << std::endl
		<< "size:                " << tag.size() << std::endl
		<< "unsynchronized:      " << tag.unsynchronized() << std::endl
		<< "has extended header: " << tag.hasExtendedHeader() << std::endl
		<< "is experimental:     " << tag.isExperimental() << std::endl
		<< "has footer:          " << tag.hasFooter() << std::endl;
		
	} catch (ID3Error e) {
		std::cerr << "No tag found!" << std::endl;
		return 1;
	} catch (std::ifstream::failure e) {
		std::cerr << "I/O error!" << std::endl;
		return 1;
	}

	return 0;
}