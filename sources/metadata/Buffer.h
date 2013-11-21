#ifndef AUDIOMETADATATEXT_H_
#define AUDIOMETADATATEXT_H_

#include <cstdint>
#include <string>

class Buffer {
public:
	explicit Buffer (size_t size = 0);
	Buffer (size_t size, size_t capacity);
	Buffer (const void *, size_t size);
	Buffer (const void *, size_t size, size_t capacity);
	Buffer (void *, size_t, bool = false);
	Buffer (const Buffer &);
	Buffer (Buffer &&);
	virtual ~Buffer ();

public:
	Buffer & operator=(const Buffer &);

public:
	size_t capacity () const { return capacity_; }
	const void * data () const { return data_; }
	bool empty () const { return size_ == 0; }
	size_t size () const { return size_; }

public:
	void swap (Buffer &);

public:
	void assign (const void *, size_t);
	void append (const Buffer &);
	void append (const std::string &);
	void append (const void *, size_t);
	void fill (uint8_t value);
	void clear ();
	void shrink (size_t);

public:
	uint8_t& at (size_t);
	uint8_t at (size_t) const;
	uint8_t& operator[] (size_t pos) { return this->at(pos); }
	uint8_t operator[] (size_t pos) const { return this->at(pos); }

private:
	size_t size_;
	size_t capacity_;
	void *data_;
};

#endif