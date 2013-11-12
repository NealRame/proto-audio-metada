extern "C" {
#	include <byteswap.h>
}

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/detail/endian.hpp>
#include <boost/scope_exit.hpp>

template<typename T> T htoid3 (T);
template<> uint32_t htoid3<uint32_t> (uint32_t v) {
#if defined (BOOST_LITTLE_ENDIAN)
	return bswap_32(v);
#else
	return v;
#endif	
}
template<> uint16_t htoid3<uint16_t> (uint16_t v) {
#if defined (BOOST_LITTLE_ENDIAN)
	return bswap_16(v);
#else
	return v;
#endif
}

template<typename T> T id3toh (T);
template<> uint32_t id3toh<uint32_t> (uint32_t v) {
#if defined (BOOST_LITTLE_ENDIAN)
	return bswap_32(v);
#else
	return v;
#endif
}
template<> uint16_t id3toh<uint16_t> (uint16_t v) {
#if defined (BOOST_LITTLE_ENDIAN)
	return bswap_16(v);
#else
	return v;
#endif
}

uint32_t synchsafe (uint32_t in) {
	uint32_t out, mask = 0x7F;
	while (mask ^ 0x7FFFFFFF) {
		out = in & ~mask;
		out <<= 1;
		out |= in & mask;
		mask = ((mask + 1) << 8) - 1;
		in = out;
	}
	return out;
}

uint32_t unsynchsafe (uint32_t in) {
	uint32_t out = 0, mask = 0x7F000000;
	while (mask) {
		out >>= 1;
		out |= in & mask;
		mask >>= 8;
	}
	return out;
}


struct ID3TagError : public std::exception {
	enum Status {
		NoTag,
		BadFrame,
		UnsupportedVersion,
	};

	explicit ID3TagError (Status status) throw() :
		status(status) {
	}

	const Status status;
};

class ID3Tag {
public:
	ID3Tag (std::ifstream &in) {
		std::ios::iostate state = in.exceptions();

		// Reset the exceptions state of in on exit
		BOOST_SCOPE_EXIT_ALL(&) {
			in.exceptions(state);
		};

		in.exceptions(std::ifstream::badbit);

		readHeader_(in);
	}

public:
	std::string version () const {
		std::ostringstream oss;
		oss << "2." << majorVersion() << "." << revisionVersion();
		return oss.str();
	}

	unsigned int majorVersion () const {
		return (version_ & 0xff00) >> 8;
	}

	unsigned int revisionVersion () const {
		return (version_ & 0x00ff);
	}

	bool unsynchronized () const {
		return unsynchronized_;
	}

	bool hasExtendedHeader () const {
		return extentedHeader_;
	}

	bool isExperimental () const {
		return experimental_;
	}

	bool hasFooter () const {
		return footer_;
	}

	size_t size () const {
		return size_;
	}

private:
	uint16_t version_;
	bool unsynchronized_;
	bool extentedHeader_;
	bool experimental_;
	bool footer_;

	size_t size_;
	size_t updateSize_() {
		return 0; // TODO
	}

private:
	// the following are used for serialization/unserialization
	struct Header {
		char id[3];
		uint16_t version;
		uint8_t unsynchronized : 1;
		uint8_t extendedHeader : 1;
		uint8_t experimental : 1;
		uint8_t footer : 1;
		uint32_t size;
	} __attribute__((packed));

	void readHeader_ (std::ifstream &in) {
		Header header;
		
		in.read((char *)&header, sizeof(Header));

		if (! (in.gcount() == sizeof(Header)
				&& std::string(header.id, 3) == "ID3")) {
			throw ID3TagError(ID3TagError::NoTag);
		}

		version_ = id3toh(header.version);
		unsynchronized_ = header.unsynchronized;
		extentedHeader_ = header.extendedHeader;
		experimental_ = header.experimental;
		footer_ = header.footer;
		size_ = unsynchsafe(id3toh(header.size));
	}

	void writeHeader_ (std::ofstream &out) {
		Header header;

		memset(&header, 0, sizeof(Header));

		header.id[0] = 'I';
		header.id[1] = 'D';
		header.id[2] = '3';

		header.experimental = isExperimental();
		header.unsynchronized = unsynchronized();
		header.footer = hasFooter();
		header.version = htoid3(version_);
		header.size = synchsafe(htoid3(size()));

		out.write((char *)&header, sizeof(Header));
	}
};

class ID3Frame {
public:
	ID3Frame (ID3Tag &container, std::ifstream &in) :
		container_(container) {
		readHeader_(in);
	}

public:
	std::string id () const {
		return id_;
	}

	size_t size () const {
		switch (container_.revisionVersion()) {
		case 3: return sizeof(HeaderV3);
		case 4: return sizeof(HeaderV4);
		}
		throw ID3TagError(ID3TagError::UnsupportedVersion);
	}

	bool discardOnTagAlteration () const {
		return discardOnTagAlteration_;
	}

	bool discardOnFileAlteration () const {
		return discardOnFileAlteration_;
	}

	bool isReadOnly () const {
		return readOnly_;
	}

	bool grouped () const {
		return grouped_;
	}

	bool deflated () const {
		return deflated_;
	}

	bool encrypted () const {
		return encrypted_;
	}

	bool unsynchronized () const {
		return unsynchronized_;
	}

	bool hasDataLengthIndicator () const {
		return dataLengthIndicator_;
	}

private:
	ID3Tag &container_;
	std::string id_;
	bool discardOnTagAlteration_;
	bool discardOnFileAlteration_;
	bool readOnly_;
	bool grouped_;
	bool deflated_;
	bool encrypted_;
	bool unsynchronized_;
	bool dataLengthIndicator_;

private:
	// the following are used for serialization/unserialization

	struct HeaderV4 {
		char id[4];
		uint32_t size;
		uint16_t : 1;
		uint16_t discardOnTagAlteration : 1;
		uint16_t discardOnFileAlteration : 1;
		uint16_t readOnly : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t grouped : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t deflated : 1;
		uint16_t encrypted : 1;
		uint16_t unsynchronized : 1;
		uint16_t dataLengthIndicator : 1;
	} __attribute__((packed));

	struct HeaderV3 {
		char id[4];
		uint32_t size;
		uint16_t discardOnTagAlteration : 1;
		uint16_t discardOnFileAlteration : 1;
		uint16_t readOnly : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t deflated : 1;
		uint16_t encrypted : 1;
		uint16_t grouped : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t : 1;
		uint16_t unsynchronized : 1;
		uint16_t : 1;
	} __attribute__((packed));

	void readHeaderV4_ (std::ifstream &in) {
		HeaderV4 header;
		in.read((char *)&header, sizeof(HeaderV4));

		if (in.gcount() != sizeof(HeaderV4)) {
			throw ID3TagError(ID3TagError::BadFrame);
		}
	}

	void readHeaderV3_ (std::ifstream &in) {
		HeaderV3 header;
		in.read((char *)&header, sizeof(HeaderV3));

		if (in.gcount() != sizeof(HeaderV3)) {
			throw ID3TagError(ID3TagError::BadFrame);
		}
	}

	void readHeader_ (std::ifstream &in) {
		if (container_.revisionVersion() == 3) {

		}




	}
};

class ID3FrameFactory {
public:

};

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Argument missing!" << std::endl;
		return 1;
	}

	std::ifstream input(argv[1], std::ifstream::in|std::ifstream::binary);

	try {
		ID3Tag tag(input);
		std::cout 
		<< "version:             " << tag.version() << std::endl
		<< "size:                " << tag.size() << std::endl
		<< "unsynchronized:      " << tag.unsynchronized() << std::endl
		<< "has extended header: " << tag.hasExtendedHeader() << std::endl
		<< "is experimental:     " << tag.isExperimental() << std::endl
		<< "has footer:          " << tag.hasFooter() << std::endl;
	} catch (ID3TagError e) {
		std::cerr << "No tag found!" << std::endl;
		return 1;
	} catch (std::ifstream::failure e) {
		std::cerr << "I/O error!" << std::endl;
		return 1;
	}

	return 0;
}