#ifndef UTILSBUFFER_H_
#define UTILSBUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

namespace com {
namespace nealrame {
namespace utils {

class buffer_ownership_error : public std::exception {
	const char * what () const noexcept {
		return "Attempting to change the capacity of a non-owner buffer";
	}
};

class buffer {
public:
	template<typename T>
	static buffer createBuffer(unsigned int count) {
		return buffer(sizeof(T)*count); 
	}
	
public:
	explicit buffer (size_t size = 0);
	buffer (size_t size, size_t capacity);
	buffer (void *, size_t size, bool take_ownership);
	buffer (void *, size_t size, size_t capacity, bool take_ownership);
	buffer (const void *, size_t size);
	buffer (const void *, size_t size, size_t capacity);
	buffer (const buffer &);
	buffer (buffer &&);
	virtual ~buffer ();

public:
	buffer & operator=(const buffer &);

public:
	void append (const buffer &) throw(buffer_ownership_error);
	void append (const std::string &) throw(buffer_ownership_error);
	void append (const void *data, size_t len) throw(buffer_ownership_error);
	void assign (void *, size_t, bool take_ownership);
	void copy (const void *data, size_t len, size_t offset) throw(buffer_ownership_error);

	void clear ();
	void * data () { return data_; }
	const void * data () const { return data_; }
	bool empty () const { return size_ == 0; }
	void fill (uint8_t value);
	size_t size () const { return size_; }
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
	 * Set the buffer capacity to the given value.
	 * If c is smaller than the buffer current size then elements beyond
	 * c will be lost.
	 */
	void reserve (size_t c) throw(buffer_ownership_error);
	/**
	 * Increase current capacity by specified value.
	 * `b.extend(c)` is equivalent to calling `b.reserve(b.capacity()+c)`
	 */
	void extend (size_t c) throw(buffer_ownership_error);
	/**
	 * Decrease current capacity by specified value.
	 * `b.shrink(c)` is equivalent to calling `b.reserve(b.capacity()-c)`
	 */
	void shrink (size_t c) throw(buffer_ownership_error);

public:
	void swap (buffer &);

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

private:
	size_t size_;
	size_t capacity_;
	bool own_;
	void *data_;
};

} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILSBUFFER_H_ */