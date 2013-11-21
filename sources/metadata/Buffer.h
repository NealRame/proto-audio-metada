#ifndef AUDIOMETADATATEXT_H_
#define AUDIOMETADATATEXT_H_

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iterator>

template<typename T>
class Buffer {
	typedef T type;

private:
	template <typename Iterator_Type>
	class base_iterator : public std::iterator<std::bidirectional_iterator_tag, Iterator_Type> {
		base_iterator() { ptr = nullptr; }
		base_iterator(const base_iterator &other) { *this = other; }
		base_iterator & operator=(const base_iterator &other) { 
			ptr = other.ptr; 
			return *this;
		}
		bool operator==(const base_iterator &other) const { return ptr == other.ptr; }
		bool operator!=(const base_iterator &other) const { return ! *this == other; }
		Iterator_Type & operator*() { return *ptr; }
		Iterator_Type * operator->() { return ptr; }
		base_iterator & operator++() {
			ptr++;
			return *this;
		}
		base_iterator operator++(int) {
			base_iterator tmp(*this);
			ptr++;
			return tmp;
		}
		base_iterator & operator--() {
			ptr--;
			return *this;
		}
		base_iterator operator--(int) {
			base_iterator tmp(*this);
			ptr--;
			return tmp;
		}
	private:
		Iterator_Type *ptr;
	};

	template <typename Iterator_Type>
	class base_reverse_iterator : public std::iterator<std::bidirectional_iterator_tag, Iterator_Type> {
		base_reverse_iterator & operator++() {
			itr_--;
			return *this;
		}
		base_reverse_iterator operator++(int) {
			base_reverse_iterator tmp(*this);
			itr_--;
			return tmp;
		}
		base_reverse_iterator & operator--() {
			itr_++;
			return *this;
		}
		base_reverse_iterator operator--(int) {
			base_reverse_iterator tmp(*this);
			itr_++;
			return tmp;
		}		
	private:
		base_iterator<Iterator_Type> itr_;
	};

public:
	typedef base_iterator<T> iterator;
	typedef base_iterator<const T> const_iterator;
	typedef base_reverse_iterator<T> reverse_iterator;
	typedef base_reverse_iterator<const T> const_reverse_iterator;

public:
	explicit Buffer (uint32_t length = 0) :
		Buffer(nullptr, length, length) { }

	Buffer (size_t length, size_t capacity) :
		Buffer(nullptr, length, capacity) { }

	Buffer (const void *data, size_t length) :
		Buffer(data, length, length) { }

	Buffer (const void *data, size_t length, size_t capacity) {
		length_ = length;
		capacity_ = std::max(length, capacity);
		data_ = malloc(capacity*sizeof(T));
		if (capacity_ > 0) {
			if (data != nullptr) {
				memcpy(data_, data, size());
			}
		}
	}

	Buffer (const Buffer &buffer) {
		*this  = buffer;
	}

	Buffer (Buffer &&buffer) {
		length_ = buffer.length_;
		capacity_ = buffer.capacity_;
		data_ = buffer.data_;
		buffer.length_ = buffer.capacity_ = 0;
		buffer.data_ = nullptr;
	}

	virtual ~Buffer () {
		if (data_ != nullptr) {
			free(data_);
		}
	}

public:
	Buffer & operator=(const Buffer &buffer) {
		length_ = buffer.length_;
		capacity_ = std::max(length_, buffer.capacity_);
		data_ = realloc(data_, capacity_*sizeof(T));
		if (capacity_ > 0) {
			if (buffer.data_ != nullptr) {
				memcpy(data_, buffer.data_, size());
			}
		}
		return *this;
	}

public:
	uint32_t capacity () const { return capacity_; }
	const T * data () const { return data_; }
	bool empty () const { return length_ == 0; }
	size_t size () const { return length_*sizeof(T); }
	uint32_t length () const { return length_; }

public:
	void swap (Buffer &buffer) {
		std::swap(data_, buffer.data_);
		std::swap(length_, buffer.length_);
		std::swap(capacity_, buffer.capacity_);
	}

	void assign (const void *d, uint32_t l)  {
		length_ = l;
		capacity_ = std::max(length_, l);
		data_ = realloc(data_, capacity_);
		memcpy(data_, d, size());
	}

	void append (const void *d, uint32_t l) {
		uint32_t new_length = length_ + l;
		capacity_ = std::max(capacity_, new_length);
		data_ = realloc(data_, capacity_*sizeof(T));
		memcpy(data_ + length_, d, l);
		length_ = new_length;
	}

	void append (const Buffer &buffer) {
		append(buffer.data_, buffer.length_);
	}

	void clear () {
		length_ = 0;
	}

	void shrink (uint32_t length) {
		length_ = capacity_ = std::min(size, capacity_);
		data_ = realloc(data_, size());
	}

public:
	T & at (uint32_t pos) { return (data_)[pos]; }
	T at (uint32_t pos) const { return (data_)[pos]; }
	T & operator[] (uint32_t pos) { return this->at(pos); }
	T operator[] (uint32_t pos) const { return this->at(pos); }

public:
	iterator begin () {
		iterator it;
		it.ptr_ = data_;
		return it;
	}

	iterator end () {
		iterator it;
		it.ptr_ = data_ + length_;
		return it;
	}

	const_iterator begin () const {
		const_iterator it;
		it.ptr_ = data_;
		return it;
	}

	iterator const_end () {
		const_iterator it;
		it.ptr_ = data_ + length_;
		return it;
	}

	reverse_iterator rbegin () {
		reverse_iterator it;
		it.itr_.ptr_ = data_ + length_ - 1;
		return it;
	}

	reverse_iterator rend () {
		reverse_iterator it;
		it.itr_.ptr_ = data_ - 1;
		return it;
	}

	const_reverse_iterator rbegin () const {
		const_reverse_iterator it;
		it.itr_.ptr_ = data_ + length_ - 1;
		return it;
	}

	const_reverse_iterator rend () const {
		const_reverse_iterator it;
		it.itr_.ptr_ = data_ - 1;
		return it;
	}

private:
	uint32_t length_;
	uint32_t capacity_;
	T *data_;
};

#endif