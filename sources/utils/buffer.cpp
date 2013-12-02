#include <algorithm>
#include <cstring>

#include "buffer.h"

using namespace com::nealrame::utils;

buffer::buffer (size_t size) :
	buffer(nullptr, size, size, true) {
}

buffer::buffer (size_t size, size_t capacity) :
	buffer(nullptr, size, capacity, true) {
}

buffer::buffer (void *data, size_t size, bool take_ownership) :
	buffer(data, size, size, take_ownership) {
}

buffer::buffer (void *data, size_t size, size_t capacity, bool take_ownership) {
	if (take_ownership) {
		capacity_ = std::max(size, capacity);
		data_ = capacity_ > 0 ? realloc(data, capacity_) :  nullptr;
	} else {
		capacity_ = size_;
		data_ = data;
	}
	own_ = take_ownership;
	size_ = size;
}

buffer::buffer (const void *data, size_t size) :
	buffer(data, size, size) {
}

buffer::buffer (const void *data, size_t size, size_t capacity) {
	own_ = true;
	size_ = size;
	capacity_ = std::max(size_, capacity_);
	data_ = capacity_ > 0  ? malloc(capacity_) : nullptr;
	memcpy(data_, data, size);
}

buffer::buffer (const buffer &other) :
	buffer((const void *)other.data_, other.size_, other.capacity_) {
	*this  = other;
}

buffer::buffer (buffer &&buffer) {
	own_ = buffer.own_;
	size_ = buffer.size_;
	capacity_ = buffer.capacity_;
	data_ = buffer.data_;
	buffer.size_ = buffer.capacity_ = 0;
	buffer.data_ = nullptr;
}

buffer::~buffer () {
	if (own_ && data_ != nullptr) {
		free(data_);
	}
}

buffer & buffer::operator=(const buffer &buffer) {
	if (! own_) {
		data_ = nullptr;
		own_ = true;
	}

	size_ = buffer.size_;
	reserve(buffer.capacity_);

	if (data_ != nullptr) {
		memcpy(data_, buffer.data_, size_);
	}

	return *this;
}

void buffer::assign (void *data, size_t length, bool take_ownership) {
	if (own_) {
		free(data_);
	}
	capacity_ = size_ = length;
	data_ = data;
	own_ = take_ownership;
}

void buffer::copy (const void *data, size_t length, size_t offset)
	throw (buffer_ownership_error) {
	if ((offset + length) > capacity_) {
		extend((offset + length) - capacity_);
	}
	memcpy(static_cast<uint8_t *>(data_) + offset, data, length);
	size_ = std::max(size_, offset + length);
}

void buffer::append (const void *data, size_t length)
	throw (buffer_ownership_error) {
	copy(data, length, size_);
}

void buffer::append (const buffer &buffer)
	throw (buffer_ownership_error) {
	copy(buffer.data_, buffer.size_, size_);
}

void buffer::append (const std::string &s)
	throw (buffer_ownership_error) {
	copy(s.c_str(), s.length() + 1, size_);
}

void buffer::clear () {
	size_ = 0;
}

void buffer::fill (uint8_t value) {
	memset(data_, value, size_);
}

void buffer::reserve (size_t c) 
	throw(buffer_ownership_error) {
	if (! own_) {
		throw buffer_ownership_error();
	}
	data_ = realloc(data_, c);
	capacity_ = c;
}

void buffer::extend (size_t c) 
	throw(buffer_ownership_error) {
	reserve(capacity_ + c);
}

void buffer::shrink (size_t c) 
	throw(buffer_ownership_error) {
	reserve(capacity_ < c ? 0 : capacity_ - c);
}

void buffer::swap (buffer &buffer) {
	std::swap(capacity_, buffer.capacity_);
	std::swap(data_, buffer.data_);
	std::swap(own_, buffer.own_);
	std::swap(size_, buffer.size_);
}
