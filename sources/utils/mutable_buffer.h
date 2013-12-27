#ifndef UTILS_MUTABLE_BUFFER_H_
#define UTILS_MUTABLE_BUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

#include "abstract_buffer.h"

namespace com {
namespace nealrame {
namespace utils {

class mutable_buffer : public abstract_buffer {
public:

	mutable_buffer (size_t length, size_t capacity);
	explicit mutable_buffer (size_t length);
	mutable_buffer ();
	mutable_buffer (const void *, size_t length, size_t capacity);
	mutable_buffer (const void *, size_t length);
	mutable_buffer (const abstract_buffer &);
	mutable_buffer (const mutable_buffer &);
	mutable_buffer (mutable_buffer &&);
	virtual ~mutable_buffer ();

public:
	mutable_buffer & operator=(const abstract_buffer &);
	mutable_buffer & operator=(const mutable_buffer &);

public:
	void append (const void *data, size_t len);
	void append (const std::string &);
	void append (const abstract_buffer &);

	virtual void copy (const void *data, size_t len, size_t offset);
	virtual void fill (uint8_t value, size_t count, size_t offset);

	void clear () { length_ = 0; }

	template <typename T>
	void push_back(const T &value) {
		if (! (length_ < capacity_)) {
			enlarge(sizeof(T));
		}
		*(reinterpret_cast<T *>((uint8_t *)data_ + length_)) = value;
		length_ += sizeof(T);
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
	void enlarge (size_t c);
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