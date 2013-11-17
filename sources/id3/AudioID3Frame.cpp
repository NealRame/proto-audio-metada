#include <sstream>

#include "AudioID3Error.h"
#include "AudioID3Frame.h"
#include "AudioID3Tag.h"



std::istringstream & operator>> (std::istringstream &in, TextIso8859_1 &t) {
	int c;
	while (! (in.eof() || (c = in.get()) == 0)) {
		t.push_back(c);
	}
	return in;
}

std::istringstream & operator>> (std::istringstream &in, TextUtf8 &t) {
	TextIso8859_1 s;
	
	in >> s;

	wchar_t *buf = new wchar_t[s.size()];
	size_t count = mbstowcs(buf, s.c_str(), s.size());

	t = std::wstring(buf, count);

	return in;
}

std::istringstream & operator>> (std::istringstream &in, TextUtf16 &t) {
	t.clear();
	while (! in.eof()) {
		char16_t c;
		in.read((char *)&c, sizeof(c));
		if (c != 0) {
			t.push_back(c);
		} else break;
	}
	return in;
}

std::istringstream & operator>> (std::istringstream &in, Text &t) {
	t.encoding = static_cast<Text::Encoding>(in.get());

	switch (t.encoding) {
	case Text::Iso8859_1:
		in >> boost::get<TextIso8859_1>(t.value);
		break;

	case Text::Utf16:
	case Text::Utf16BE:
		in >> boost::get<TextUtf16>(t.value);
		break;

	case Text::Utf8:
		in >> boost::get<TextUtf8>(t.value);
		break;
	}

	return in;
}

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
	uint16_t : 1;
	uint16_t : 1;
} __attribute__((packed));

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

template <typename FrameHeader>
FrameHeader readFrameHeader_ (std::ifstream &in) {
	FrameHeader header;

	in.read((char *)&header, sizeof(header));

	if (in.gcount() < sizeof(header)) {
		throw ID3Error(ID3Error::BadFrame);		
	}

	return header;
}

ID3Frame::ID3Frame (ID3Tag &container) :
	container_(container) {
}

std::ifstream & ID3Frame::operator>> (std::ifstream &in) {
	unsigned int major = container_.majorVersion();
	if (major == 3) {
		HeaderV3 header = readFrameHeader_<HeaderV3>(in);
		
		discardOnTagAlteration_ = header.discardOnTagAlteration;
		discardOnFileAlteration_ = header.discardOnFileAlteration;
		readOnly_ = header.readOnly;
		grouped_ = header.grouped;
		deflated_ = header.deflated;
		encrypted_ = header.encrypted;
		unsynchronized_ = false;
		dataLengthIndicator_ = false;
	} else
	if (major == 4) {
		HeaderV4 header = readFrameHeader_<HeaderV4>(in);
		
		discardOnTagAlteration_ = header.discardOnTagAlteration;
		discardOnFileAlteration_ = header.discardOnFileAlteration;
		readOnly_ = header.readOnly;
		grouped_ = header.grouped;
		deflated_ = header.deflated;
		encrypted_ = header.encrypted;
		unsynchronized_ = header.unsynchronized;
		dataLengthIndicator_ = header.dataLengthIndicator;
	} else {
		throw ID3Error(ID3Error::UnsupportedVersion);
	}

	return in;
}

std::ofstream & ID3Frame::operator<< (std::ofstream &out) {
	return out;
}

std::string ID3Frame::id () const {
	return id_;
}

size_t ID3Frame::size () const {
	switch (container_.revisionVersion()) {
	case 3: return sizeof(HeaderV3);
	case 4: return sizeof(HeaderV4);
	}
	throw ID3Error(ID3Error::UnsupportedVersion);
}

bool ID3Frame::discardOnTagAlteration () const {
	return discardOnTagAlteration_;
}

bool ID3Frame::discardOnFileAlteration () const {
	return discardOnFileAlteration_;
}

bool ID3Frame::isReadOnly () const {
	return readOnly_;
}

bool ID3Frame::grouped () const {
	return grouped_;
}

bool ID3Frame::deflated () const {
	return deflated_;
}

bool ID3Frame::encrypted () const {
	return encrypted_;
}

bool ID3Frame::unsynchronized () const {
	return unsynchronized_;
}

bool ID3Frame::hasDataLengthIndicator () const {
	return dataLengthIndicator_;
}
