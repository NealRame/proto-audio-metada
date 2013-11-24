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
		it_ += 2*i;
		return *this;
	}
	template<typename IntegralType>
	interleaved_iterator & operator-= (IntegralType i) {
		it_ -= 2*i;
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
	std::pair<T &, T &> operator* () {
		return std::pair<T &, T &>(*it_, *(it_+1));
	}
	std::pair<T &, T &> * operator-> () {
		pair_ = *this;
		return &pair_;
	}
	std::pair<T &, T &> operator[] (unsigned int i) { return *(it_ + i); }
	std::pair<const T &, const T &> operator[] (unsigned int i) const {
		iterator it = *this + i;
		return std::pair<T, T>(*it_, *(it_ + 1));
	}

private:
	iterator it_;
	std::array<T &, size_t N> values_;
};



#endif /* UTILSINTERLEAVEDITERATOR_H_ */