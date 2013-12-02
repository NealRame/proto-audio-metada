#include <fstream>
#include <locale>
#include <iostream>
#include <iomanip>
#include <string>
#include <functional>

#include "utf8.h"

#include "id3/AudioID3Tag.h"
#include "id3/AudioID3Error.h"

#include "utils/buffer.h"
#include "utils/interleaved_iterator.h"

int main(int argc, char **argv) {

	using namespace com::nealrame;

	char data1[] = { 0x42, 0x41, 0x42, 0x41 };
	char data2[] = { 0x44, 0x43, 0x44, 0x43 };
	char data3[] = { 0x46, 0x45, 0x46, 0x45 };

	utils::buffer buf1(data1, sizeof(data1), false);
	utils::buffer buf2(data2, sizeof(data2), false);
	utils::buffer buf3(data3, sizeof(data3), false);

	utils::buffer buf(const_cast<const void *>((void *)data1), sizeof(data1));

	utils::buffer buf4;
	utils::interlace<int16_t, 3>({{ buf1, buf2, buf3 }}, buf4);
	
	for (auto it = buf4.begin<uint8_t>(), end = buf4.end<uint8_t>();
			it != end;
			++it) {
		std::cout
			<< std::setw(2)
			<< std::noshowbase
			<< std::hex
			<< (unsigned int)*it << std::endl;
	}

	std::array<utils::buffer, 3> bufs;
	utils::deinterlace<uint16_t, 3>(buf4, bufs);

	for (unsigned int i = 0; i < 3; ++i) {
		for (auto it = bufs[i].begin<uint8_t>(),
			end = bufs[i].end<uint8_t>(); it != end; ++it) {
			std::cout
				<< std::setw(2)
				<< std::noshowbase
				<< std::hex
				<< (unsigned int ) *it << ", ";
		}
		std::cout << std::endl;
	}

	// if (argc < 2) {
	// 	std::cerr << "Argument missing!" << std::endl;
	// 	return 1;
	// }

	// std::ifstream input(argv[1], std::ifstream::in|std::ifstream::binary);

	// try {
	// 	ID3Tag tag;

	// 	tag.init(input);

	// 	std::cout 
	// 	<< "version:             " << tag.version() << std::endl
	// 	<< "size:                " << tag.size() << std::endl
	// 	<< "unsynchronized:      " << tag.unsynchronized() << std::endl
	// 	<< "has extended header: " << tag.hasExtendedHeader() << std::endl
	// 	<< "is experimental:     " << tag.isExperimental() << std::endl
	// 	<< "has footer:          " << tag.hasFooter() << std::endl;
		
	// } catch (ID3Error e) {
	// 	std::cerr << "No tag found!" << std::endl;
	// 	return 1;
	// } catch (std::ifstream::failure e) {
	// 	std::cerr << "I/O error!" << std::endl;
	// 	return 1;
	// }

	return 0;
}