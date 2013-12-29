#include <gtest/gtest.h>

#include <audio/error>
#include <audio/format>
#include <audio/buffer>

using namespace com::nealrame;

class audioBufferTest : public testing::Test {
protected:
	audioBufferTest() : 
		rates{16000, 22050, 44100, 48000, 96000},
		depths{8, 16},
		channels{1, 2, 3, 6, 8} {
	}
	std::vector<unsigned int> rates;
	std::vector<unsigned int> depths;
	std::vector<unsigned int> channels;
};

TEST_F(audioBufferTest, Constructor) {
	for (unsigned int channel_count : channels) {
		for (unsigned int bit_depth : depths) {
			for (unsigned int sample_rate : rates) {
				audio::format fmt{channel_count, sample_rate, bit_depth};
				audio::buffer buf(fmt);
				EXPECT_EQ(fmt, buf.format());
			}
		}
	}
}
