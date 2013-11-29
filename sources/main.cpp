#include <fstream>
#include <locale>
#include <iostream>
#include <iomanip>
#include <string>
#include <functional>

#include "utf8.h"

#include "id3/AudioID3Tag.h"
#include "id3/AudioID3Error.h"

#include "utils/Buffer.h"
#include "utils/InterleavedIterator.h"


template<typename T> struct my_reference_wrapper {
	my_reference_wrapper(T & ref) : 
		ref_(ref) {
	}

	my_reference_wrapper & operator=(const T & v) {
		ref_ = v;
		return *this;
	}

	operator T & () {
		return ref_;
	}

	T & ref_;
};

int main(int argc, char **argv) {

	char data1[] = { 0x42, 0x41, 0x42, 0x41 };
	char data2[] = { 0x44, 0x43, 0x44, 0x43 };
	char data3[] = { 0x46, 0x45, 0x46, 0x45 };

	Buffer buffer1(data1, sizeof(data1));
	Buffer buffer2(data2, sizeof(data2));
	Buffer buffer3(data3, sizeof(data3));

	Buffer buffer4;
	interlace<int16_t, 3>({{ buffer1, buffer2, buffer3 }}, buffer4);
	
	for (auto it = buffer4.begin<uint8_t>(), end = buffer4.end<uint8_t>();
			it != end;
			++it) {
		std::cout 
			<< std::setw(2) 
			<< std::noshowbase
			<< std::hex 
			<< (unsigned int)*it << std::endl;
	}

	std::array<Buffer, 3> buffers;
	deinterlace<uint16_t, 3>(buffer4, buffers);

	for (unsigned int i = 0; i < 3; ++i) {
		for (auto it = buffers[i].begin<uint8_t>(), 
			end = buffers[i].end<uint8_t>(); it != end; ++it) {
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