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

	char data1[] = { 0x42, 0x42, 0x42, 0x42 };
	char data2[] = { 0x43, 0x43, 0x43, 0x43 };

	Buffer buffer1(data1, sizeof(data1));
	Buffer buffer2(data2, sizeof(data2));

	std::cout << "buffer1 capacity : " << buffer1.capacity()<< std::endl;
	std::cout << "buffer1 size     : " << buffer1.size() << std::endl;

	std::cout << "buffer2 capacity : " << buffer2.capacity()<< std::endl;
	std::cout << "buffer2 size     : " << buffer2.size() << std::endl;

	std::array<Buffer, 2> a = {{ buffer1, buffer2 }};

	Buffer buffer3 = interlace<int16_t>(a);
	
	// std::cout << buffer3.size() << std::endl;

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