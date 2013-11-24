#ifndef UTILSINTERLEAVEDITERATOR_H_
#define UTILSINTERLEAVEDITERATOR_H_

#include <array>
#include "Buffer.h"

template<typename T, size_t N>
class interleaved_iterator : 
	std::iterator<std::random_access_iterator_tag, std::array<T &, N>> {
private:
	typedef Buffer::iterator<T> iterator;

public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}

	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>()/N;
	}

public:
	interleaved_iterator (iterator it) : it_(it) { }
	interleaved_iterator () : interleaved_iterator(iterator()) { }
	interleaved_iterator(const interleaved_iterator &it) : 
		interleaved_iterator(it.it_) {
	}
	interleaved_iterator & operator= (const interleaved_iterator& it) {
		it_ = it.it_;
	}
	bool operator<  (const interleaved_iterator &it) const {
		return it_ < it.it_;
	}
	bool operator>  (const interleaved_iterator &it) const {
		return it_ > it.it_;
	}
	bool operator<= (const interleaved_iterator &it) const {
		return ! (*this > it);
	}
	bool operator>= (const interleaved_iterator &it) const {
		return ! (*this < it);
	}
	bool operator== (const interleaved_iterator &it) const {
		return it_ == it.it_; 
	}
	bool operator!= (const interleaved_iterator &it) const {
		return it_ != it.it_;
	}
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	template<typename IntegralType> 
	interleaved_iterator & operator+= (IntegralType i) {
		it_ += N*i;
		return *this;
	}
	template<typename IntegralType>
	interleaved_iterator & operator-= (IntegralType i) {
		it_ -= N*i;
		return *this;
	}
	interleaved_iterator & operator++ () { return *this += 1; }
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator & operator-- () { return *this -= 1; }
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<T &, N> operator* ();
	std::array<const T &, N> operator* () const;
	std::array<T &, N> operator[] (unsigned int i);
	std::array<T, N> operator[] (unsigned int i) const;

private:
	iterator it_;
};

template<typename T, size_t N>
Buffer interlace (const std::array<Buffer, N> &buffer) {
	return Buffer();
}

template<typename T>
std::array<T &, 1> interleaved_iterator<T, 1>::operator* () {
	return std::array<T, 1>{ *it_ };
}
template<typename T>
std::array<const T &, 1> interleaved_iterator<T, 1>::operator* () const {
	return std::array<T, 1>{ *it_ };
}
template<typename T>
std::array<T &, 2> interleaved_iterator<T, 2>::operator* () {
	return std::array<T, 2>{ *it_, *(it_ + 1) };
}
template<typename T>
std::array<const T &, 2> interleaved_iterator<T, 2>::operator* () const {
	return std::array<T, 2>{ *it_, *(it_ + 1) };
}
template<typename T>
std::array<T &, 3> interleaved_iterator<T, 3>::operator* () {
	return std::array<T, 3>{ *it_, *(it_ + 1), *(it_ + 2) };
}
template<typename T>
std::array<const T &, 3> interleaved_iterator<T, 3>::operator* () const {
	return std::array<T, 3>{ *it_, *(it_ + 1), *(it_ + 2) };
}
template<typename T>
std::array<T &, 4> interleaved_iterator<T, 4>::operator* () {
	return std::array<T, 4>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3) };
}
template<typename T>
std::array<const T &, 4> interleaved_iterator<T, 4>::operator* () const {
	return std::array<T, 4>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3) };
}
template<typename T>
std::array<T &, 5> interleaved_iterator<T, 5>::operator* () {
	return std::array<T, 5>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3), *(it_ + 4) };
}
template<typename T>
std::array<const T &, 5> interleaved_iterator<T, 5>::operator* () const {
	return std::array<T, 5>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3), *(it_ + 4) };
}
template<typename T>
std::array<T &, 6> interleaved_iterator<T, 6>::operator* () {
	return std::array<T, 6>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3), *(it_ + 4), *(it_ + 5) };
}
template<typename T>
std::array<const T &, 6> interleaved_iterator<T, 6>::operator* () const {
	return std::array<T, 6>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3), *(it_ + 4), *(it_ + 5) };
}
template<typename T>
std::array<T &, 7> interleaved_iterator<T, 7>::operator* () {
	return std::array<T, 7>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3), *(it_ + 4), *(it_ + 5), *(it_ + 6) };
}
template<typename T>
std::array<const T &, 7> interleaved_iterator<T, 7>::operator* () const {
	return std::array<T, 7>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3), *(it_ + 4), *(it_ + 5), *(it_ + 6) };
}
template<typename T>
std::array<T &, 8> interleaved_iterator<T, 8>::operator* () {
	return std::array<T, 8>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3), *(it_ + 4), *(it_ + 5), *(it_ + 6), *(it_ + 7) };
}
template<typename T>
std::array<const T &, 8> interleaved_iterator<T, 8>::operator* () const {
	return std::array<T, 8>{ *it_, *(it_ + 1), *(it_ + 2), *(it_ + 3), *(it_ + 4), *(it_ + 5), *(it_ + 6), *(it_ + 7) };
}

template<typename T>
std::array<T &, 1> operator[] (unsigned int i) {
	return std::array<T, 1>{ *(it_ + i) };
}
template<typename T>
std::array<T, 1> operator[] (unsigned int i) const {
	return std::array<T, 1>{ *(it_ + i) };
}
template<typename T>
std::array<T &, 2> operator[] (unsigned int i) {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1) };
}
template<typename T>
std::array<T, 2> operator[] (unsigned int i) const {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1) };
}
template<typename T>
std::array<T &, 3> operator[] (unsigned int i) {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2) };
}
template<typename T>
std::array<T, 3> operator[] (unsigned int i) const {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2) };
}
template<typename T>
std::array<T &, 4> operator[] (unsigned int i) {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3) };
}
template<typename T>
std::array<T, 4> operator[] (unsigned int i) const {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3) };
}
template<typename T>
std::array<T &, 5> operator[] (unsigned int i) {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3), *(it_ + i + 4) };
}
template<typename T>
std::array<T, 5> operator[] (unsigned int i) const {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3), *(it_ + i + 4) };
}
template<typename T>
std::array<T &, 6> operator[] (unsigned int i) {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3), *(it_ + i + 4), *(it_ + i + 5) };
}
template<typename T>
std::array<T, 6> operator[] (unsigned int i) const {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3), *(it_ + i + 4), *(it_ + i + 5) };
}
template<typename T>
std::array<T &, 7> operator[] (unsigned int i) {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3), *(it_ + i + 4), *(it_ + i + 5), *(it_ + i + 6) };
}
template<typename T>
std::array<T, 7> operator[] (unsigned int i) const {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3), *(it_ + i + 4), *(it_ + i + 5), *(it_ + i + 6) };
}
template<typename T>
std::array<T &, 8> operator[] (unsigned int i) {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3), *(it_ + i + 4), *(it_ + i + 5), *(it_ + i + 6), *(it_ + i + 7) };
}
template<typename T>
std::array<T, 8> operator[] (unsigned int i) const {
	return std::array<T, 1>{ *(it_ + i), *(it_ + i + 1), *(it_ + i + 2), *(it_ + i + 3), *(it_ + i + 4), *(it_ + i + 5), *(it_ + i + 6), *(it_ + i + 7) };
}

#endif /* UTILSINTERLEAVEDITERATOR_H_ */