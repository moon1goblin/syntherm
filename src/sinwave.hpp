#include <numbers>
#include <cstdint>
#include <RtAudio.h>

namespace synth {

const unsigned int G_SAMPLE_RATE_HZ = 44100;
// it was loud as fuck, i dont know whats wrong with it
const double G_AMPLITUDE = 0.00001;
// library couldnt handle the const on this
std::uint32_t G_BUFFER_SAMPLE_FRAMES = 256;

class Shapes {
public:
	static int sinwave(void* outputbuf
			, void* /*inputbuf*/
			, unsigned int buffer_frames
			, double stream_time_seconds
			, RtAudioStreamStatus /*stream_status*/
			, void* user_data)
	{
		double* buf = static_cast<double*>(outputbuf);
		double cur_freq_hz = static_cast<std::atomic<double>*>(user_data)->load();

		// if (cur_freq_hz != 0) {
		// 	std::println("cur_freq_hz {}", cur_freq_hz);
		// }

		std::uint32_t cur_time_samples = static_cast<std::uint32_t>(stream_time_seconds * G_SAMPLE_RATE_HZ);

		double sin_arg = 2 * std::numbers::pi * cur_freq_hz;
		double cur_time_seconds;

		for(std::size_t i = 0; i < buffer_frames; ++i) {
			cur_time_seconds = static_cast<double>(cur_time_samples) / G_SAMPLE_RATE_HZ;
			*buf = std::sin(sin_arg * cur_time_seconds) * G_AMPLITUDE;
			++cur_time_samples;
			++buf;
		}

		return 0;
	}
};

}
