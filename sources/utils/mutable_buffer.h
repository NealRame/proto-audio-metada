#ifndef UTILS_MUTABLE_BUFFER_H_
#define UTILS_MUTABLE_BUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

#include "base_buffer.h"

namespace com {
namespace nealrame {
namespace utils {

class mutable_buffer : public base_buffer {
public:
	explicit mutable_buffer (size_t capacity);
	mutable_buffer (mutable_buffer &&);
	virtual ~mutable_buffer ();

public:
	mutable_buffer & operator=(const mutable_buffer &);

public:
	void append (const mutable_buffer &);
	void append (const std::string &);
	void append (const void *data, size_t len);
	
	virtual void assign (void *, size_t);
	virtual void copy (const void *data, size_t len, size_t offset);

	void fill (uint8_t value, size_t);
	template <typename T> unsigned int count () const {
		return size()/sizeof(T);
	}
	template <typename T>
	void push_back(const T &value) {
		*(reinterpret_cast<T *>((uint8_t *)data_ + size_)) = value;
		size_ += sizeof(T);
	}

public:
	size_t capacity () const { return capacity_; }
	/**
	 * Set the mutable_buffer capacity to the given value.
	 * If c is smaller than the mutable_buffer current size then elements beyond
	 * c will be lost.
	 */
	void reserve (size_t c);
	/**
	 * Increase current capacity by specified value.
	 * `b.extend(c)` is equivalent to calling `b.reserve(b.capacity()+c)`
	 */
	void extend (size_t c);
	/**
	 * Decrease current capacity by specified value.
	 * `b.shrink(c)` is equivalent to calling `b.reserve(b.capacity()-c)`
	 */
	void shrink (size_t c);

public:
	void swap (mutable_buffer &);

private:
	size_t capacity_;
};

} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILS_MUTABLE_BUFFER_H_ */