#ifndef UTILS_BUFFER_H_
#define UTILS_BUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

#include "abstract_buffer.h"

namespace com {
namespace nealrame {
namespace utils {

class buffer : public abstract_buffer {
public:
	buffer (abstract_buffer &other) : buffer(other.data(), other.length()) { }
	buffer (void *, size_t size);
	buffer () : buffer(nullptr, 0) { }
	buffer (buffer &&);

public:
	virtual void copy (const void *data, size_t len, size_t offset);
	virtual void fill (uint8_t value, size_t count, size_t offset);
	void set (buffer &);
	void set (void *data, size_t length);
	buffer slice(size_t begin);
	buffer slice(size_t begin, size_t end);
};
} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILSBUFFER_H_ */