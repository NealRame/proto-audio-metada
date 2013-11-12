#include "AudioID3Error.h"
#include "AudioID3Frame.h"
#include "AudioID3Tag.h"

ID3Frame::ID3Frame (ID3Tag &container, std::ifstream &in) :
	container_(container) {
	readHeader_(in);
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

void ID3Frame::readHeaderV3_ (std::ifstream &in) {
	HeaderV3 header;
	in.read((char *)&header, sizeof(HeaderV3));

	if (in.gcount() != sizeof(HeaderV3)) {
		throw ID3Error(ID3Error::BadFrame);
	}

	discardOnTagAlteration_ = header.discardOnTagAlteration;
	discardOnFileAlteration_ = header.discardOnFileAlteration;
	readOnly_ = header.readOnly;
	grouped_ = header.grouped;
	deflated_ = header.deflated;
	encrypted_ = header.encrypted;
	unsynchronized_ = false;
}

void ID3Frame::readHeaderV4_ (std::ifstream &in) {
	HeaderV4 header;
	in.read((char *)&header, sizeof(HeaderV4));

	if (in.gcount() != sizeof(HeaderV4)) {
		throw ID3Error(ID3Error::BadFrame);
	}

	discardOnTagAlteration_ = header.discardOnTagAlteration;
	discardOnFileAlteration_ = header.discardOnFileAlteration;
	readOnly_ = header.readOnly;
	grouped_ = header.grouped;
	deflated_ = header.deflated;
	encrypted_ = header.encrypted;
	unsynchronized_ = header.unsynchronized;
}

void ID3Frame::readHeader_ (std::ifstream &in) {
	switch (container_.revisionVersion()) {
	case 3:
		readHeaderV3_(in);
		break;

	case 4:
		readHeaderV4_(in);
		break;

	default:
		throw ID3Error(ID3Error::UnsupportedVersion);
	}
}
