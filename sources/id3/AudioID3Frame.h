#ifndef AUDIOID3FRAME_H_
#define AUDIOID3FRAME_H_

#include <chrono>
#include <fstream>
#include <string>
#include <vector>

#include <boost/variant.hpp>
#include <boost/variant/variant.hpp>

class ID3Tag;

typedef std::string TextIso8859_1;
typedef std::wstring TextUtf8;
typedef std::u16string TextUtf16;

// Used when char encoding is specified 
struct Text {
	enum Encoding {
		Utf8      = 0x00,
		Utf16     = 0x01,
		Utf16BE   = 0x02,
		Iso8859_1 = 0x03,
	} encoding;
	boost::variant<TextUtf8, TextUtf16, TextIso8859_1> value;
};

std::istringstream & operator>> (std::istringstream &, TextIso8859_1 &);
std::istringstream & operator>> (std::istringstream &, TextUtf8 &);
std::istringstream & operator>> (std::istringstream &, TextUtf16 &);
std::istringstream & operator>> (std::istringstream &in, Text &t);

template <typename Type>
struct ID3FrameField {
	typedef Type type;
	std::string name;
	type value;
};

class ID3Frame {
public:
	ID3Frame (ID3Tag &container);

public:
	virtual std::ifstream & operator>> (std::ifstream &) = 0;
	virtual std::ofstream & operator<< (std::ofstream &) = 0;
	virtual size_t size () const = 0;

public:
	std::string id () const;
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
};

#endif
