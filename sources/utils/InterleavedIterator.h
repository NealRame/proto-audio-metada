#ifndef UTILSINTERLEAVEDITERATOR_H_
#define UTILSINTERLEAVEDITERATOR_H_

#include <array>
#include "Buffer.h"

template<typename T, size_t N>
class base_interleaved_iterator : 
	std::iterator<std::random_access_iterator_tag, std::array<T &, N>> {

protected:
	typedef Buffer::iterator<T> iterator;

public:
	base_interleaved_iterator (iterator it) : it_(it) { }
	base_interleaved_iterator () : base_interleaved_iterator(iterator()) { }
	base_interleaved_iterator(const base_interleaved_iterator &it) : 
		base_interleaved_iterator(it.it_) {
	}
	virtual base_interleaved_iterator & operator= (const base_interleaved_iterator& it) {
		it_ = it.it_;
	}
	virtual bool operator<  (const base_interleaved_iterator &it) const {
		return it_ < it.it_;
	}
	virtual bool operator>  (const base_interleaved_iterator &it) const {
		return it_ > it.it_;
	}
	virtual bool operator<= (const base_interleaved_iterator &it) const {
		return ! (*this > it);
	}
	virtual bool operator>= (const base_interleaved_iterator &it) const {
		return ! (*this < it);
	}
	virtual bool operator== (const base_interleaved_iterator &it) const {
		return it_ == it.it_; 
	}
	virtual bool operator!= (const base_interleaved_iterator &it) const {
		return it_ != it.it_;
	}
	virtual base_interleaved_iterator & operator+= (int8_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (int16_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (int32_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (int64_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (uint8_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (uint16_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (uint32_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (uint64_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (int8_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (int16_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (int32_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (uint8_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (uint16_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (uint32_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (uint64_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator++ () { return *this += 1; }
	virtual base_interleaved_iterator & operator-- () { return *this -= 1; }
	virtual std::array<T &, N> operator* () = 0;
	virtual std::array<const T &, N> operator* () const = 0;
	virtual std::array<T &, N> operator[] (unsigned int i) = 0;
	virtual std::array<T, N> operator[] (unsigned int i) const = 0;

protected:
	iterator it_;
};

template<typename T, size_t N>
class interleaved_iterator : public base_interleaved_iterator<T, N> {
};

template<typename T>
class interleaved_iterator<T, 1> : public base_interleaved_iterator<T, 1> {
public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}
	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>();
	}
public:
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	virtual std::array<T &, 1> operator* () {
		return std::array<T, 1>{
			*(this->it_)
		};
	}
	virtual std::array<const T &, 1> operator* () const {
		return std::array<T, 1>{
			*(this->it_)
		};
	}
	virtual std::array<T &, 1> operator[] (unsigned int i) {
		return std::array<T, 1>{
			*(this->it_ + i)
		};
	}
	virtual std::array<T, 1> operator[] (unsigned int i) const {
		return std::array<T, 1>{
			*(this->it_ + i)
		};
	}
};

template<typename T>
class interleaved_iterator<T, 2> : public base_interleaved_iterator<T, 2> {
public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}
	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>()/2;
	}
public:
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	virtual std::array<T &, 2> operator* () {
		return std::array<T, 2>{
			*(this->it_),
			*(this->it_ + 1)
		};
	}
	virtual std::array<const T &, 2> operator* () const {
		return std::array<T, 2>{
			*(this->it_),
			*(this->it_ + 1) 
		};
	}
	virtual std::array<T &, 2> operator[] (unsigned int i) {
		return std::array<T, 2>{
			*(this->it_ + i),
			*(this->it_ + i + 1)
		};
	}
	virtual std::array<T, 2> operator[] (unsigned int i) const {
		return std::array<T, 2>{
			*(this->it_ + i),
			*(this->it_ + i + 1)
		};
	}
};

template<typename T>
class interleaved_iterator<T, 3> : public base_interleaved_iterator<T, 3> {
public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}
	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>()/3;
	}
public:
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	virtual std::array<T &, 3> operator* () {
		return std::array<T, 3>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2)
		};
	}
	virtual std::array<const T &, 3> operator* () const {
		return std::array<T, 3>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2)
		};
	}
	virtual std::array<T &, 3> operator[] (unsigned int i) {
		return std::array<T, 3>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2)
		};
	}
	virtual std::array<T, 3> operator[] (unsigned int i) const {
		return std::array<T, 3>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2)
		};
	}
};

template<typename T>
class interleaved_iterator<T, 4> : public base_interleaved_iterator<T, 4> {
public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}
	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>()/4;
	}
public:
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	virtual std::array<T &, 4> operator* () {
		return std::array<T, 4>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3)
		};
	}
	virtual std::array<const T &, 4> operator* () const {
		return std::array<T, 4>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3)
		};
	}
	virtual std::array<T &, 4> operator[] (unsigned int i) {
		return std::array<T, 4>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3)
		};
	}
	virtual std::array<T, 4> operator[] (unsigned int i) const {
		return std::array<T, 4>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3)
		};
	}
};

template<typename T>
class interleaved_iterator<T, 5> : public base_interleaved_iterator<T, 5> {
public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}
	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>()/5;
	}
public:
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	virtual std::array<T &, 5> operator* () {
		return std::array<T, 5>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4)
		};
	}
	virtual std::array<const T &, 5> operator* () const {
		return std::array<T, 5>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4)
		};
	}
	virtual std::array<T &, 5> operator[] (unsigned int i) {
		return std::array<T, 5>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3),
			*(this->it_ + i + 4)
		};
	}
	virtual std::array<T, 5> operator[] (unsigned int i) const {
		return std::array<T, 5>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3),
			*(this->it_ + i + 4)
		};
	}
};

template<typename T>
class interleaved_iterator<T, 6> : public base_interleaved_iterator<T, 6> {
public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}
	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>()/6;
	}
public:
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	virtual std::array<T &, 6> operator* () {
		return std::array<T, 6>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5)
		};
	}
	virtual std::array<const T &, 6> operator* () const {
		return std::array<T, 6>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5)
		};
	}
	virtual std::array<T &, 6> operator[] (unsigned int i) {
		return std::array<T, 6>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3),
			*(this->it_ + i + 4),
			*(this->it_ + i + 5)
		};
	}
	virtual std::array<T, 6> operator[] (unsigned int i) const {
		return std::array<T, 6>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3),
			*(this->it_ + i + 4),
			*(this->it_ + i + 5)
		};
	}
};

template<typename T>
class interleaved_iterator<T, 7> : public base_interleaved_iterator<T, 7> {
public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}
	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>()/7;
	}
public:
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	virtual std::array<T &, 7> operator* () {
		return std::array<T, 7>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5),
			*(this->it_ + 6)
		};
	}
	virtual std::array<const T &, 7> operator* () const {
		return std::array<T, 7>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5),
			*(this->it_ + 6)
		};
	}
	virtual std::array<T &, 7> operator[] (unsigned int i) {
		return std::array<T, 7>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3),
			*(this->it_ + i + 4),
			*(this->it_ + i + 5),
			*(this->it_ + i + 6)
		};
	}
	virtual std::array<T, 7> operator[] (unsigned int i) const {
		return std::array<T, 7>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3),
			*(this->it_ + i + 4),
			*(this->it_ + i + 5),
			*(this->it_ + i + 6)
		};
	}
};

template<typename T>
class interleaved_iterator<T, 8> : public base_interleaved_iterator<T, 8> {
public:
	static interleaved_iterator begin (Buffer &buffer) {
		return interleaved_iterator(buffer.begin<T>());
	}
	static interleaved_iterator end (Buffer &buffer) {
		return begin(buffer) + buffer.count<T>()/8;
	}
public:
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	virtual std::array<T &, 8> operator* () {
		return std::array<T, 8>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5),
			*(this->it_ + 6),
			*(this->it_ + 7)
		};
	}
	virtual std::array<const T &, 8> operator* () const {
		return std::array<T, 8>{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5),
			*(this->it_ + 6),
			*(this->it_ + 7)
		};
	}
	virtual std::array<T &, 8> operator[] (unsigned int i) {
		return std::array<T, 8>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3),
			*(this->it_ + i + 4),
			*(this->it_ + i + 5),
			*(this->it_ + i + 6),
			*(this->it_ + i + 7)
		};
	}
	virtual std::array<T, 8> operator[] (unsigned int i) const {
		return std::array<T, 8>{
			*(this->it_ + i),
			*(this->it_ + i + 1),
			*(this->it_ + i + 2),
			*(this->it_ + i + 3),
			*(this->it_ + i + 4),
			*(this->it_ + i + 5),
			*(this->it_ + i + 6),
			*(this->it_ + i + 7) };
	}
};

template<typename T, size_t N>
Buffer interlace (const std::array<Buffer, N> &buffers) {
	unsigned int min_count = std::numeric_limits<unsigned int>::max();

	std::for_each(buffers.begin(), buffers.end(),
		[&min_count](const Buffer &buffer) {
			min_count = std::min(buffer.count<T>(), min_count);
		});

	Buffer buffer = Buffer::createBuffer<T>(min_count);
	auto it = interleaved_iterator<T, N>::begin(buffer);

	for (unsigned int i = 0; i < min_count; ++i) {
		std::array<T &, N> frame = *it++;

		for (unsigned int j = 0; j < N; ++j) {
			Buffer &channel = buffers[j];
			frame.at(j) = *(channel.begin<T>() + i);
		}
	}

	return Buffer();
}

#endif /* UTILSINTERLEAVEDITERATOR_H_ */