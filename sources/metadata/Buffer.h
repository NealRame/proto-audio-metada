#ifndef AUDIOMETADATATEXT_H_
#define AUDIOMETADATATEXT_H_

#include <cstdint>
#include <string>
#include <iterator>

class Buffer {
public:
	template <typename T> 
	class iterator : std::iterator<std::random_access_iterator_tag, T> {
	public:
		iterator () : iterator(nullptr) { }
		iterator(const iterator &it) : iterator(it.ptr_) { }
		iterator & operator= (const iterator& it) { ptr_ = it.ptr_; }
		iterator & operator++ () { 
			ptr_ += 1; 
			return *this;
		}
		iterator operator++ (int) {
			iterator it(*this);
			(*this)++;
			return it;	
		}
		iterator & operator-- () { 
			ptr_ -= 1; 
			return *this;
		}
		iterator operator-- (int) {
			iterator it(*this);
			(*this)--;
			return it;
		}
		bool operator<  (const iterator &it) const {
			return ptr_ < it.ptr_;
		}
		bool operator>  (const iterator &it) const {
			return ptr_ > it.ptr_;
		}
		bool operator<= (const iterator &it) const {
			return ! (*this > it);
		}
		bool operator>= (const iterator &it) const {
			return ! (*this < it);
		}
		bool operator== (const iterator &it) const {
			return ptr_ == it.ptr_; 
		}
		bool operator!= (const iterator &it) const {
			return ptr_ != it.ptr_;
		}
		iterator operator+ (int i) {
			return iterator(ptr_ + i);
		}
		iterator operator+ (unsigned int i) {
			return iterator(ptr_ + i);
		}
		iterator operator- (int i) {
			return iterator(ptr_ - i);
		}
		iterator operator- (unsigned int i) {
			return iterator(ptr_ - i);
		}
		iterator & operator+= (int i) {
			ptr_ += i;
			return *this;
		}
		iterator & operator+= (unsigned int i) {
			ptr_ += i;
			return *this;
		}
		iterator & operator-= (int i) {
			ptr_ -= i;
			return *this;
		}
		iterator & operator-= (unsigned int i) {
			ptr_ -= i;
			return *this;
		}
		T & operator[] (unsigned int i) {
			return *(ptr_ + i);
		}
		T operator[] (unsigned int i) const {
			return *(ptr_ + i);
		}
		T & operator* () { return *static_cast<T *>(ptr_); }
		T * operator-> () { return static_cast<T *>(ptr_); }

	private:
		iterator (void *ptr) : ptr_(ptr) { }
		T *ptr_;
	};

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