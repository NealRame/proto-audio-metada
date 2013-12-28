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
#include "utils/mutable_buffer.h"
#include "utils/interleaved_iterator.h"

int main(int argc, char **argv) {

	using namespace com::nealrame;

	unsigned char data1[] = { 0x01, 0x01, 0x01, 0x01 };
	unsigned char data2[] = { 0x02, 0x02, 0x02, 0x02 };
	unsigned char data3[] = { 0x03, 0x03, 0x03, 0x03 };

	unsigned char data4[sizeof(data1) + sizeof(data2) + sizeof(data3)];

	std::iostream out(std::cout.rdbuf());

	for (uint i = 0, pos = 1; i < sizeof(data4); ++i) {
		out
			<< std::setw(2)
			<< std::noshowbase
			<< std::hex
			<< (unsigned int) data4[i] << (((pos + i)%6) ? ", " :"\n");
	}

	out << "======" << std::endl;

	utils::buffer buf1(data1, sizeof(data1));
	utils::buffer buf2(data2, sizeof(data2));
	utils::buffer buf3(data3, sizeof(data3));

	utils::buffer buf4(data4, sizeof(data4));

	utils::interlace<int16_t, 3>({{ buf1, buf2, buf3 }}, buf4);

	for (uint i = 0, pos = 1; i < sizeof(data4); ++i) {
		out
			<< std::setw(2)
			<< std::noshowbase
			<< std::hex
			<< (unsigned int) data4[i] << (((pos + i)%6) ? ", " :"\n");
	}

	std::cout << "======" << std::endl;

	unsigned char data5[sizeof(data4)];

	utils::buffer buf5(data5,     4);
	utils::buffer buf6(data5 + 4, 4);
	utils::buffer buf7(data5 + 8, 4);	

	utils::deinterlace<uint16_t, 3>(buf4, {{ buf5, buf6, buf7 }});

	for (unsigned int i = 0, pos = 1; i < sizeof(data5); ++i) {
		out
			<< std::setw(2)
			<< std::noshowbase
			<< std::hex
			<< (unsigned int ) data5[i] << (((pos + i)%4) ? ", " : "\n");
	}

	return 0;
}