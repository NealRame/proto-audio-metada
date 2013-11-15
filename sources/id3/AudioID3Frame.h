#include <fstream>
#include <string>

class ID3Tag;
class ID3Frame {
public:
	ID3Frame (ID3Tag &container, std::ifstream &in);

public:
	std::string id () const;
	size_t size () const;
	bool discardOnTagAlteration () const;
	bool discardOnFileAlteration () const;
	bool isReadOnly () const;
	bool grouped () const;
	bool deflated () const;
	bool encrypted () const;
	bool unsynchronized () const;
	bool hasDataLengthIndicator () const;

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

	void readHeaderV3_ (std::ifstream &in);
	void readHeaderV4_ (std::ifstream &in);
	void readHeader_ (std::ifstream &in);
};