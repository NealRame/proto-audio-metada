#ifndef AUDIO_BUFFER_H_
#define AUDIO_BUFFER_H_

#include <functional>
#include <iterator>
#include <vector>

#include <utils/mutable_buffer>
#include <audio/format>

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::buffer
/// 
/// The `buffer` class provides representation of an audio buffer. It is 
/// nothing more than a sample container with an assign format descriptor.
class buffer : private com::nealrame::utils::mutable_buffer {
public:
	class frame {
		friend buffer;
		::std::vector<buffer::iterator<double>> samples;
	public:
		frame & operator=(const ::std::vector<double> &values);
		frame & operator=(const ::std::initializer_list<double> &values);
	};

public:
	explicit buffer (format);

	format format () const;
	double duration () const;
	unsigned int frame_count () const;

public:


private:
	class format format_;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */