#include "AudioID3Error.h"
#include "AudioID3Tag.h"
#include "AudioID3Utils.h"

#include <sstream>

#include <boost/scope_exit.hpp>

ID3Tag::ID3Tag (std::ifstream &in) {
	std::ios::iostate state = in.exceptions();

	// Reset the exceptions state of in on exit
	BOOST_SCOPE_EXIT_ALL(&) {
		in.exceptions(state);
	};

	in.exceptions(std::ifstream::badbit);

	readHeader_(in);

	if (extentedHeader_) {
	}
}

std::string ID3Tag::version () const {
	std::ostringstream oss;
	oss << "2." << majorVersion() << "." << revisionVersion();
	return oss.str();
}

unsigned int ID3Tag::majorVersion () const {
	return (version_ & 0xff00) >> 8;
}

unsigned int ID3Tag::revisionVersion () const {
	return (version_ & 0x00ff);
}

bool ID3Tag::unsynchronized () const {
	return unsynchronized_;
}

bool ID3Tag::hasExtendedHeader () const {
	return extentedHeader_;
}

bool ID3Tag::isExperimental () const {
	return experimental_;
}

bool ID3Tag::hasFooter () const {
	return footer_;
}

size_t ID3Tag::size () const {
	return size_;
}

size_t ID3Tag::updateSize_() {
	return 0; // TODO
}


struct Header {
	char id[3];
	uint16_t version;
	uint8_t unsynchronized : 1;
	uint8_t extendedHeader : 1;
	uint8_t experimental : 1;
	uint8_t footer : 1;
	uint32_t size;
} __attribute__((packed));

void ID3Tag::readHeader_ (std::ifstream &in) {
	Header header;
	
	in.read((char *)&header, sizeof(Header));

	if (! (in.gcount() == sizeof(Header)
			&& std::string(header.id, 3) == "ID3")) {
		throw ID3Error(ID3Error::NoTag);
	}

	version_ = id3toh(header.version);
	unsynchronized_ = header.unsynchronized;
	extentedHeader_ = header.extendedHeader;
	experimental_ = header.experimental;
	footer_ = header.footer;
	size_ = unsynchsafe(id3toh(header.size));
}

void ID3Tag::writeHeader_ (std::ofstream &out) {
	Header header;

	memset(&header, 0, sizeof(Header));

	header.id[0] = 'I';
	header.id[1] = 'D';
	header.id[2] = '3';

	header.experimental = isExperimental();
	header.unsynchronized = unsynchronized();
	header.footer = hasFooter();
	header.version = htoid3(version_);
	header.size = synchsafe(htoid3((uint32_t)size()));

	out.write((char *)&header, sizeof(Header));
}

struct ExtentedHeaderV3 {
	uint32_t size;
	uint16_t crcFlag : 1;
	uint32_t paddingSize;
} __attribute__((packed));

struct ExtentedHeaderV4 {
	uint32_t size;
	uint8_t flagByteCount;
	uint8_t : 1;
	uint8_t updateFlag : 1;
	uint8_t crcFlag : 1;
	uint8_t restrictionsFlags : 1;
} __attribute__((packed));

void ID3Tag::readExtendedHeader(std::ifstream &in) {
	switch (revisionVersion()) {
	case 3: break;
	case 4: break;
	default:
		throw ID3Error(ID3Error::UnsupportedVersion);
	}
}
