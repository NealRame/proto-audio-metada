#include <cstdint>
#include <fstream>
#include <string>

class ID3Tag {
public:
	ID3Tag (std::ifstream &in);

public:
	std::string version () const;
	unsigned int majorVersion () const;
	unsigned int revisionVersion () const;
	bool unsynchronized () const;
	bool hasExtendedHeader () const;
	bool isExperimental () const;
	bool hasFooter () const;
	size_t size () const;

private:
	uint16_t version_;
	bool unsynchronized_;
	bool extentedHeader_;
	bool experimental_;
	bool footer_;

	size_t size_;
	size_t updateSize_();
};


