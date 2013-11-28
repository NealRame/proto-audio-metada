#ifndef UTILSBUFFER_H_
#define UTILSBUFFER_H_

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <vector>

class Buffer {
public:
	template<typename T>
	static Buffer createBuffer(unsigned int count) {
		return Buffer(sizeof(T)*count); 
	}
	
public:
	explicit Buffer (size_t size = 0);
	Buffer (size_t size, size_t capacity);
	Buffer (const void *, size_t size);
	Buffer (const void *, size_t size, size_t capacity);
	Buffer (const Buffer &);
	Buffer (Buffer &&);
	virtual ~Buffer ();

public:
	Buffer & operator=(const Buffer &);

public:
	void append (const Buffer &);
	void append (const std::string &);
	void append (const void *, size_t);
	void assign (const void *, size_t);
	uint8_t & at (size_t);
	uint8_t at (size_t) const;
	uint8_t & operator[] (size_t pos) { return this->at(pos); }
	uint8_t operator[] (size_t pos) const { return this->at(pos); }
	void clear ();
	void * data () { return data_; }
	const void * data () const { return data_; }
	bool empty () const { return size_ == 0; }
	void fill (uint8_t value);
	size_t size () const { return size_; }
	template <typename T> unsigned int count () const {
		return size()/sizeof(T);
	}

public:
	size_t capacity () const { return capacity_; }
	/**
	 * Set the buffer capacity to the given value.
	 * If c is smaller than the buffer current size then elements beyond
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
	void swap (Buffer &);

public:
	template <typename T> 
	class iterator : std::iterator<std::random_access_iterator_tag, T> {
	public:
		iterator (T *ptr) : ptr_(ptr) { }
		iterator () : iterator(nullptr) { }
		iterator(const iterator &it) : iterator(it.ptr_) { }
		iterator & operator= (const iterator& it) { 
			ptr_ = it.ptr_; 
			return *this;
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
		template<typename IntegralType>
		iterator operator+ (IntegralType i) const {
			iterator it(*this);
			it += i;
			return it;
		}
		template<typename IntegralType>
		iterator operator- (IntegralType i) const { 
			return iterator(*this) -= i; 
		}
		template<typename IntegralType> 
		iterator & operator+= (IntegralType i) {
			ptr_ += i;
			return *this;
		}
		template<typename IntegralType>
		iterator & operator-= (IntegralType i) {
			ptr_ -= i;
			return *this;
		}
		iterator & operator++ () { return *this += 1; }
		iterator operator++ (int) {
			iterator it(*this);
			++(*this);
			return it;
		}
		iterator & operator-- () { return *this -= 1; }
		iterator operator-- (int) {
			iterator it(*this);
			--(*this);
			return it;
		}
		T & operator* () { return *ptr_; }
		T * operator-> () { return ptr_; }
		T & operator[] (unsigned int i) { return *(ptr_ + i); }
		T operator* () const { return *ptr_; }
		T operator[] (unsigned int i) const {
			return (*const_cast<iterator<T> *>(this))[i];
		}
	private:
		T *ptr_;
	};

	template <typename T> iterator<T> begin () {
		return iterator<T>(static_cast<T *>(data_));
	}

	template <typename T> iterator<T> end () {
		return begin<T>() + count<T>();
	}

	template <typename T> iterator<const T> begin () const {
		return iterator<const T>(static_cast<const T *>(data_));
	}

	template <typename T> iterator<const T> end () const {
		return begin<const T>() + count<T>();
	}

	template <typename T> std::reverse_iterator<iterator<T>> rbegin () {
		return std::reverse_iterator<iterator<T>>(end<T>());
	}

	template <typename T> std::reverse_iterator<iterator<T>> rend () {
		return std::reverse_iterator<iterator<T>>(begin<T>());
	}

	template <typename T> std::reverse_iterator<iterator<const T>> rbegin () const {
		return std::reverse_iterator<iterator<T>>(end<const T>());
	}

	template <typename T> std::reverse_iterator<iterator<const T>> rend () const {
		return std::reverse_iterator<iterator<T>>(begin<const T>());
	}

public:
	template<typename T>
	static Buffer interlace (const Buffer &a, const Buffer &b) {
		Buffer r = Buffer(2*sizeof(T)*(std::min(a.size(), b.size())/sizeof(T)));
		
		auto a_it = a.begin<T>(), a_end = a.end<T>();
		auto b_it = b.begin<T>(), b_end = b.end<T>();
		auto r_it = r.begin<T>();

		while (a_it != a_end && b_it != b_end) {
			*r_it++ = *a_it++;
			*r_it++ = *b_it++;
		}

		return r;
	}

	template<typename T>
	static std::pair<Buffer, Buffer> deinterlace (const Buffer &buffer) {
		unsigned int count = buffer.count<T>()/2;
		Buffer a(count*sizeof(T)), b(count*sizeof(T));

		auto a_it = a.begin<T>(), b_it = b.begin<T>();

		for (auto it = buffer.begin<T>(), end = it + 2*count;
			it != end; 
			it += 2) {
			*a_it++ = *(it);
			*b_it++ = *(it + 1);
		}

		return std::pair<Buffer, Buffer>(a, b);
	}

private:
	size_t size_;
	size_t capacity_;
	void *data_;
};

#endif /* UTILSBUFFER_H_ */