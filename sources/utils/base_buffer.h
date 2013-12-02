#ifndef UTILS_BASE_BUFFER_H_
#define UTILS_BASE_BUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

namespace com {
namespace nealrame {
namespace utils {

class base_buffer {
public:
	virtual void assign (void *, size_t) = 0;
	void clear ();
	virtual void copy (const void *data, size_t len, size_t offset) = 0;
	const void * data () const { return data_; }
	bool empty () const { return size_ == 0; }
	size_t size () const { return size_; }
	template <typename T> unsigned int count () const {
		return size()/sizeof(T);
	}

public:
	template <typename T> 
	class iterator : std::iterator<std::random_access_iterator_tag, T> {
	public:
		iterator (T *ptr) : ptr_(ptr) { }
		iterator () : iterator(nullptr) { }
		iterator (const iterator &it) : iterator(it.ptr_) { }
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

protected:
	size_t size_;
	void *data_;
};

} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILS_BASE_BUFFER_H_ */