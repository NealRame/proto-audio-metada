#if defined(__APPLE__)

#include <cstdint>

inline uint16_t bswap_16 (uint16_t v) {
	return (v >> 8)
		| (v << 8);
}
inline  int16_t bswap_16 (int16_t v) {
	return (int16_t)bswap_16((uint16_t)v);
}
inline uint32_t bswap_32 (uint32_t v) {
	return ((uint32_t)bswap_16((uint16_t)(v & 0x0000ffff)))
		| ((uint32_t)bswap_16((uint16_t)(v >> 16)));
}
inline  int32_t bswap_32 (int32_t v) {
	return (int32_t)bswap_32((uint32_t)v);
}

#else
extern "C" {
#	include <byteswap.h>
}
#include <cstdint>
#endif

#include <boost/detail/endian.hpp>

template<typename T> T htoid3 (T);
template<> uint32_t htoid3<uint32_t> (uint32_t v) {
#if defined (BOOST_LITTLE_ENDIAN)
	return bswap_32(v);
#else
	return v;
#endif	
}
template<> uint16_t htoid3<uint16_t> (uint16_t v) {
#if defined (BOOST_LITTLE_ENDIAN)
	return bswap_16(v);
#else
	return v;
#endif
}

template<typename T> T id3toh (T);
template<> uint32_t id3toh<uint32_t> (uint32_t v) {
#if defined (BOOST_LITTLE_ENDIAN)
	return bswap_32(v);
#else
	return v;
#endif
}
template<> uint16_t id3toh<uint16_t> (uint16_t v) {
#if defined (BOOST_LITTLE_ENDIAN)
	return bswap_16(v);
#else
	return v;
#endif
}

uint32_t synchsafe (uint32_t in) {
	uint32_t out, mask = 0x7F;
	while (mask ^ 0x7FFFFFFF) {
		out = in & ~mask;
		out <<= 1;
		out |= in & mask;
		mask = ((mask + 1) << 8) - 1;
		in = out;
	}
	return out;
}

uint32_t unsynchsafe (uint32_t in) {
	uint32_t out = 0, mask = 0x7F000000;
	while (mask) {
		out >>= 1;
		out |= in & mask;
		mask >>= 8;
	}
	return out;
}