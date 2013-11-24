#include <algorithm>
#include <cassert>
#include <cstring>

#include "Buffer.h"

Buffer::Buffer (size_t size) :
	Buffer(nullptr, size, size) {
}

Buffer::Buffer (size_t size, size_t capacity) :
	Buffer(nullptr, size, capacity) {
}

Buffer::Buffer (const void *data, size_t size) :
	Buffer(data, size, size) {
}

Buffer::Buffer (const void *data, size_t size, size_t capacity) {
	size_ = size;
	capacity_ = std::max(size, capacity);
	data_ = malloc(capacity);
	if (capacity_ > 0) {
		if (data != nullptr) {
			memcpy(data_, data, size);
		}
	}
}

Buffer::Buffer (const Buffer &buffer) {
	*this  = buffer;
}

Buffer::Buffer (Buffer &&buffer) {
	size_ = buffer.size_;
	capacity_ = buffer.capacity_;
	data_ = buffer.data_;
	buffer.size_ = buffer.capacity_ = 0;
	buffer.data_ = nullptr;
}

Buffer::~Buffer () {
	if (data_ != nullptr) {
		free(data_);
	}
}

Buffer & Buffer::operator=(const Buffer &buffer) {
	size_ = buffer.size_;
	capacity_ = std::max(size_, buffer.capacity_);
	data_ = realloc(data_, capacity_);
	if (capacity_ > 0) {
		if (buffer.data_ != nullptr) {
			memcpy(data_, buffer.data_, size_);
		}
	}
	return *this;
}

void Buffer::assign (const void *d, size_t l) {
	reserve(capacity_ = std::max(size_, l));
	memcpy((uint8_t *)data_, d, l);
	size_ = l;
}

void Buffer::append (const void *d, size_t l) {
	reserve(std::max(capacity_, size_ + l));
	memcpy((uint8_t *)data_ + size_, d, l);
	size_ += l;
}

void Buffer::append (const Buffer &buffer) {
	append(buffer.data_, buffer.size_);
}

void Buffer::append (const std::string &s) {
	append(s.c_str(), s.length() + 1);
}

void Buffer::clear () {
	size_ = 0;
}

void Buffer::fill (uint8_t value) {
	memset(data_, value, size_);
}

uint8_t & Buffer::at (size_t pos) {
	return ((uint8_t *)data_)[pos];
}

uint8_t Buffer::at (size_t pos) const {
	return ((uint8_t *)data_)[pos];
}

void Buffer::reserve (size_t c) {
	data_ = realloc(data_, c);
	capacity_ = c;
}

void Buffer::extend (size_t c) {
	reserve(capacity_ + c);
}

void Buffer::shrink (size_t c) {
	reserve(capacity_ < c ? 0 : capacity_ - c);
}

void Buffer::swap (Buffer &buffer) {
	std::swap(data_, buffer.data_);
	std::swap(size_, buffer.size_);
	std::swap(capacity_, buffer.capacity_);
}
