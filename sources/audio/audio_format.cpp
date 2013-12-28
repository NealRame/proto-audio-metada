/**
 * audio_format.cpp
 *
 * Created on: Dec 28, 2013
 *     Author: Neal.Rame. <contact@nealrame.com>
 */
#include "audio_format.h"

using namespace com::nealrame::audio;

format & format::set_channel_count (unsigned int count) throw(audio::error) {
	if (count < 1) {
		error::raise(error::status::FormatBadValue);
	}
	channel_count_ = count;
	return *this; 
}

format & format::set_sample_rate (unsigned int rate) throw(audio::error) {
	switch (rate) {
		case  8000:
		case 16000:
		case 22050:
		case 44100:
		case 48000:
		case 96000:
			sample_rate_ = rate;
			break;
		default:
			error::raise(error::status::FormatBadValue);
	}
	return *this;
}

format & format::set_bit_depth (unsigned int depth) throw(audio::error) {
	switch (depth) {
		case  8:
		case 16:
			bit_depth_ = depth;
			break;
		default:
			error::raise(error::status::FormatBadValue);
	}
	return *this;
}

double format::duration (unsigned int frame_count) const {
	return static_cast<double>(frame_count/static_cast<double>(sample_rate_));
}

double format::duration (size_t size) const {
	return duration(frame_count(size));
}

unsigned int format::frame_count (size_t size) const {
	return size/(channel_count_*(bit_depth_/8));
}

unsigned int format::frame_count (double duration) const {
	return static_cast<double>(sample_rate_)*duration;
}

size_t format::size (unsigned int frame_count) const {
	return static_cast<size_t>(frame_count*channel_count_*(bit_depth_/8));
}

size_t format::size (double duration) const {
	return size(frame_count(duration));
}