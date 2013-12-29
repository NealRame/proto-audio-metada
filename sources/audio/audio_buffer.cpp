#include "audio_buffer.h"

using namespace com::nealrame;

audio::buffer::buffer (audio::format fmt) :
	utils::mutable_buffer(), 
	format_(fmt) {
}

audio::format audio::buffer::format () const {
	return format_;
}

double audio::buffer::duration () const {
	return format_.duration(length_);
}

unsigned int audio::buffer::frame_count () const {
	return format_.frame_count(length_);
}
