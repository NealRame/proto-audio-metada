#ifndef UTILS_ABSTRACTBUFFER_H_
#define UTILS_ABSTRACTBUFFER_H_

#include <cstdint>
#include <cstring>
#include <iterator>
#include <string>

namespace com {
namespace nealrame {
namespace utils {
/// com::nealrame::abstract_buffer
/// ==============================
///
/// The `abstract_buffer` class is the base class for `buffer` and 
/// `mutable_buffer`.
///
/// It provides implementation for common routines like accessing the
/// underlying data or get its length or the count of a particular kind of 
/// elements this buffer can holds.
///
/// It also provides a generic templated random access iterator. For exemple,
/// it is possible to iterate over double or unsigned int element using thoses
/// iterators.
class abstract_buffer {
public:
	virtual ~abstract_buffer () {}
public:
	virtual void copy (const void *data, size_t len, size_t offset) = 0;
	virtual void fill (uint8_t value, size_t count, size_t offset) = 0;
	/// Get the underlying data of this buffer.
	virtual void * data () final { return length_ > 0 ? data_:nullptr; }
	/// Get the underlying data of this buffer.
	virtual const void * data () const final
	{ return const_cast<abstract_buffer *>(this)->data(); }
	/// Get the length of this buffer.
	virtual size_t length () const final { return length_; }
	/// Get the count of T element that this buffer holds.
	template <typename T>
	size_t count () const {
		return length_/sizeof(T);
	}
public: // Iterators /////////////////////////////////////////////////////////
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
	/// Return an _iterator_ pointing to the first `T` element of this
	/// buffer.
	template <typename T> iterator<T> begin () {
		return iterator<T>(static_cast<T *>(data_));
	}
	/// Return an _iterator_ pointing to the _past-the-end_ `T` element of
	/// this buffer.
	template <typename T> iterator<T> end () {
		return begin<T>() + count<T>();
	}
	/// Return a _constant iterator_ pointing to the first `T` element of
	/// this buffer.
	template <typename T> iterator<const T> begin () const {
		return iterator<const T>(static_cast<const T *>(data_));
	}
	/// Return a _constant iterator_ pointing to the _past-the-end_ `T`
	/// element of this buffer.
	template <typename T> iterator<const T> end () const {
		return begin<const T>() + count<T>();
	}
	/// Return a _reverse iterator_ pointing to the last `T` element of
	/// this buffer.
	template <typename T> std::reverse_iterator<iterator<T>> rbegin () {
		return std::reverse_iterator<iterator<T>>(end<T>());
	}
	/// Return a _reverse iterator_ pointing to the theorical `T` element
	// preceding the first element of this buffer.
	template <typename T> std::reverse_iterator<iterator<T>> rend () {
		return std::reverse_iterator<iterator<T>>(begin<T>());
	}
	/// Return a _constant reverse iterator_ pointing to the last `T`
	/// element of this buffer.
	template <typename T> std::reverse_iterator<iterator<const T>> rbegin () const {
		return std::reverse_iterator<iterator<T>>(end<const T>());
	}
	/// Return a _constant reverse iterator_ pointing to the theorical `T`
	/// element preceding the first element of this buffer.
	template <typename T> std::reverse_iterator<iterator<const T>> rend () const {
		return std::reverse_iterator<iterator<T>>(begin<const T>());
	}
protected:
	size_t capacity_;
	size_t length_;
	void *data_;
};
} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILS_BASE_BUFFER_H_ */