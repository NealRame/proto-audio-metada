#include <gtest/gtest.h>
#include "../utils/buffer.h"
#include "../utils/mutable_buffer.h"

#include <memory>

using namespace com::nealrame;

static char mutable_buffer_test_data[] = "0123456789abcdef";

class mutableBufferTest : public testing::Test {
public:
	static const size_t length;
	static const size_t capacity;

protected:
	mutableBufferTest() :
		buf_(mutable_buffer_test_data, sizeof(mutable_buffer_test_data)),
		buf2_(length),
		buf3_(length, capacity),
		buf4_(length, 0UL),
		buf5_(0UL, capacity),
		buf6_(mutable_buffer_test_data, sizeof(mutable_buffer_test_data)),
		buf7_(mutable_buffer_test_data, sizeof(mutable_buffer_test_data), sizeof(mutable_buffer_test_data)*2),
		buf8_(mutable_buffer_test_data, sizeof(mutable_buffer_test_data), 0),
		buf9_(mutable_buffer_test_data + sizeof(mutable_buffer_test_data)/4, sizeof(mutable_buffer_test_data)/2),
		buf10_(buf_),
		buf11_(buf10_) {
	}

	virtual void SetUp () {
	}

	virtual void TearDown () {
	}

	com::nealrame::utils::buffer buf_;

	com::nealrame::utils::mutable_buffer buf1_;
	com::nealrame::utils::mutable_buffer buf2_;
	com::nealrame::utils::mutable_buffer buf3_;
	com::nealrame::utils::mutable_buffer buf4_;
	com::nealrame::utils::mutable_buffer buf5_;
	com::nealrame::utils::mutable_buffer buf6_;
	com::nealrame::utils::mutable_buffer buf7_;
	com::nealrame::utils::mutable_buffer buf8_;
	com::nealrame::utils::mutable_buffer buf9_;
	com::nealrame::utils::mutable_buffer buf10_;
	com::nealrame::utils::mutable_buffer buf11_;
};

const size_t mutableBufferTest::length = sizeof(mutable_buffer_test_data);
const size_t mutableBufferTest::capacity = 2*length;

// const char mutable_buffer_test::test_data_str[] = "Test data string";

TEST_F(mutableBufferTest, DefaultConstructor) {	
	EXPECT_EQ(nullptr, buf1_.data());
	EXPECT_EQ(0, buf1_.length());
	EXPECT_EQ(0, buf1_.capacity());
}

TEST_F(mutableBufferTest, LengthConstructor) {	
	EXPECT_NE(nullptr, buf2_.data());
	EXPECT_EQ(length, buf2_.length());
	EXPECT_LE(length, buf2_.capacity());
}

TEST_F(mutableBufferTest, LengthAndCapacityConstructor) {	
	EXPECT_NE(nullptr, buf3_.data());
	EXPECT_LE(buf3_.length(), buf3_.capacity());
	EXPECT_EQ(length, buf3_.length());
	EXPECT_EQ(capacity, buf3_.capacity());
}

TEST_F(mutableBufferTest, LengthAndLowerCapacityConstructor) {
	EXPECT_NE(nullptr, buf4_.data());
	EXPECT_LE(buf4_.length(), buf4_.capacity());
	EXPECT_EQ(length, buf4_.length());
}

TEST_F(mutableBufferTest, EmptyWithCapacityConstructor) {
	EXPECT_NE(nullptr, buf5_.data());
	EXPECT_EQ(capacity, buf5_.capacity());
	EXPECT_EQ(0, buf5_.length());
}

TEST_F(mutableBufferTest, DataCopyConstructor) {
	EXPECT_NE(nullptr, buf6_.data());
	EXPECT_NE(mutable_buffer_test_data, buf6_.data());
	EXPECT_EQ(sizeof(mutable_buffer_test_data), buf6_.length());
	EXPECT_LE(buf6_.length(), buf6_.capacity());
	EXPECT_EQ(0, memcmp(buf6_.data(), mutable_buffer_test_data, sizeof(mutable_buffer_test_data)));
}

TEST_F(mutableBufferTest, DataCopyAndGreaterCapacityConstructor) {
	EXPECT_NE(nullptr, buf7_.data());
	EXPECT_NE(mutable_buffer_test_data, buf7_.data());
	EXPECT_EQ(sizeof(mutable_buffer_test_data), buf7_.length());
	EXPECT_EQ(2*buf7_.length(), buf7_.capacity());
	EXPECT_EQ(0, memcmp(buf7_.data(), mutable_buffer_test_data, sizeof(mutable_buffer_test_data)));
}

TEST_F(mutableBufferTest, DataCopyAndLowerCapacityConstructor) {
	EXPECT_NE(nullptr, buf8_.data());
	EXPECT_NE(mutable_buffer_test_data, buf8_.data());
	EXPECT_EQ(sizeof(mutable_buffer_test_data), buf8_.length());
	EXPECT_LE(buf8_.length(), buf8_.capacity());
	EXPECT_EQ(0, memcmp(buf8_.data(), mutable_buffer_test_data, sizeof(mutable_buffer_test_data)));
}

TEST_F(mutableBufferTest, DataCopySliceConstructor) {
	EXPECT_NE(nullptr, buf9_.data());
	EXPECT_NE(mutable_buffer_test_data, buf9_.data());
	EXPECT_EQ(sizeof(mutable_buffer_test_data)/2, buf9_.length());
	EXPECT_LE(buf9_.length(), buf9_.capacity());
	EXPECT_EQ(0, memcmp(buf9_.data(), mutable_buffer_test_data + sizeof(mutable_buffer_test_data)/4, buf9_.length()));
}

TEST_F(mutableBufferTest, AbstractBufferCopyConstructor) {
	EXPECT_NE(nullptr, buf10_.data());
	EXPECT_NE(buf_.data(), buf10_.data());
	EXPECT_EQ(buf_.length(), buf10_.length());
	EXPECT_LE(buf10_.length(), buf10_.capacity());
	EXPECT_EQ(0, memcmp(buf10_.data(), buf_.data(), buf10_.length()));
}

TEST_F(mutableBufferTest, MutableBufferCopyConstructor) {
	EXPECT_NE(nullptr, buf11_.data());
	EXPECT_NE(buf10_.data(), buf11_.data());
	EXPECT_EQ(buf10_.length(), buf11_.length());
	EXPECT_LE(buf11_.length(), buf11_.capacity());
	EXPECT_EQ(0, memcmp(buf11_.data(), buf10_.data(), buf11_.length()));
}

TEST_F(mutableBufferTest, Reserve) {
	buf1_.reserve(capacity);
	EXPECT_NE(nullptr, buf1_.data());
	EXPECT_EQ(0, buf1_.length());
	EXPECT_EQ(capacity, buf1_.capacity());
}

TEST_F(mutableBufferTest, Enlarge) {
	// enlarge an empty buffer
	EXPECT_EQ(nullptr, buf1_.data());
	buf1_.enlarge(capacity);
	EXPECT_NE(nullptr, buf1_.data());
	EXPECT_EQ(0, buf1_.length());
	EXPECT_EQ(capacity, buf1_.capacity());
	// enlarge a filled buffer
	size_t l = buf2_.length();
	size_t c = buf2_.capacity();
	EXPECT_NE(0, l);
	EXPECT_NE(0, c);
	utils::mutable_buffer b(buf2_);
	buf2_.enlarge(capacity);
	EXPECT_NE(nullptr, buf2_.data());
	EXPECT_EQ(l, buf2_.length());
	EXPECT_EQ(c + capacity, buf2_.capacity());
	EXPECT_EQ(0, memcmp(buf2_.data(), b.data(), b.length()));
}

TEST_F(mutableBufferTest, Shrink) {
	// shrink an empty buffer
	EXPECT_EQ(nullptr, buf1_.data());
	buf1_.shrink(buf1_.capacity());
	EXPECT_EQ(nullptr, buf1_.data());
	EXPECT_EQ(0, buf1_.length());
	EXPECT_EQ(0, buf1_.capacity());
	// shrink a filled buffer
	EXPECT_NE(nullptr, buf2_.data());
	buf2_.shrink(buf2_.capacity());
	EXPECT_EQ(nullptr, buf2_.data());
	EXPECT_EQ(0, buf2_.length());
	EXPECT_EQ(0, buf2_.capacity());
}

TEST_F(mutableBufferTest, AbstractBufferAffectationOperator) {
	utils::mutable_buffer & b = (buf1_ = buf_);
	EXPECT_EQ(&b, &buf1_);
	EXPECT_EQ(buf_.length(), buf1_.length());
	EXPECT_LE(buf1_.length(), buf1_.capacity());
	EXPECT_EQ(0, memcmp(buf1_.data(), buf_.data(), buf1_.length()));
}

TEST_F(mutableBufferTest, MutableBufferAffectationOperator) {
	utils::mutable_buffer & b = (buf1_ = buf2_);
	EXPECT_EQ(&b, &buf1_);
	EXPECT_EQ(buf2_.length(), buf1_.length());
	EXPECT_LE(buf1_.length(), buf1_.capacity());
	EXPECT_EQ(0, memcmp(buf1_.data(), buf2_.data(), buf1_.length()));
}

TEST_F(mutableBufferTest, Clear) {
	// clear an empty buffer
	EXPECT_EQ(0, buf1_.length());	
	EXPECT_EQ(nullptr, buf1_.data());
	buf1_.clear();
	EXPECT_EQ(0, buf1_.length());	
	EXPECT_EQ(nullptr, buf1_.data());

	// clear a filled buffer
	EXPECT_NE(0, buf2_.length());	
	EXPECT_NE(nullptr, buf2_.data());
	buf2_.clear();
	EXPECT_EQ(0, buf2_.length());	
	EXPECT_NE(nullptr, buf2_.data());
}

TEST_F(mutableBufferTest, Copy) {
	// copy in a default constructed buffer
	EXPECT_EQ(0, buf1_.length());
	EXPECT_EQ(0, buf1_.capacity());
	EXPECT_EQ(nullptr, buf1_.data());
	buf1_.copy(mutable_buffer_test_data, sizeof(mutable_buffer_test_data), 0);
	EXPECT_EQ(sizeof(mutable_buffer_test_data), buf1_.length());
	EXPECT_LE(buf1_.length(), buf1_.capacity());
	EXPECT_NE(nullptr, buf1_.data());
	EXPECT_EQ(0, memcmp(buf1_.data(), mutable_buffer_test_data, sizeof(mutable_buffer_test_data)));

	// copy in a cleared buffer
	EXPECT_EQ(0, buf5_.length());
	EXPECT_EQ(capacity, buf5_.capacity());
	buf5_.copy(mutable_buffer_test_data, sizeof(mutable_buffer_test_data), 0);
	EXPECT_LE(sizeof(mutable_buffer_test_data), buf5_.length());
	EXPECT_EQ(capacity, buf5_.capacity());
	EXPECT_EQ(0, memcmp(buf5_.data(), mutable_buffer_test_data, sizeof(mutable_buffer_test_data)));
}

TEST_F(mutableBufferTest, CopyWithOffset) {
	// copy in a default constructed buffer
	EXPECT_EQ(0, buf1_.length());
	EXPECT_EQ(0, buf1_.capacity());
	EXPECT_EQ(nullptr, buf1_.data());
	buf1_.copy(mutable_buffer_test_data, sizeof(mutable_buffer_test_data), sizeof(mutable_buffer_test_data));
	EXPECT_EQ(sizeof(mutable_buffer_test_data)*2, buf1_.length());
	EXPECT_LE(buf1_.length(), buf1_.capacity());
	EXPECT_NE(nullptr, buf1_.data());
	EXPECT_EQ(0, memcmp(static_cast<uint8_t *>(buf1_.data()) + sizeof(mutable_buffer_test_data), mutable_buffer_test_data, sizeof(mutable_buffer_test_data)));

	// copy in a cleared buffer
	EXPECT_EQ(0, buf5_.length());
	EXPECT_EQ(capacity, buf5_.capacity());
	buf5_.copy(mutable_buffer_test_data, sizeof(mutable_buffer_test_data), capacity);
	EXPECT_LE(capacity + sizeof(mutable_buffer_test_data), buf5_.length());
	EXPECT_LE(buf5_.length(), buf5_.capacity());
	EXPECT_EQ(0, memcmp(static_cast<uint8_t *>(buf5_.data()) + capacity, mutable_buffer_test_data, sizeof(mutable_buffer_test_data)));
}

// TEST_F(mutableBufferTest, OutOfBoundCopy) {
// 	FAIL();
// }

// TEST_F(mutableBufferTest, Fill) {
// 	FAIL();
// }

// TEST(mutableBuffer, BoundFill) {
// 	FAIL();
// }

// TEST(mutableBuffer, OutOfBoundFill) {
// 	FAIL();
// }
