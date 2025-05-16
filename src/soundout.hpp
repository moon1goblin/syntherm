#include <RtAudio.h>
#include <atomic>
#include <cstdint>
#include <print>

#include "sinwave.hpp"

namespace synth {

class AudioOut {
public:
	AudioOut(const std::atomic<double>& t_cur_freq_)
		: cur_freq_hz_(t_cur_freq_)
	{
		RtAudio::StreamParameters parameters;
		parameters.deviceId = rt_audio_.getDefaultOutputDevice();
		parameters.nChannels = 2;

		void* user_data = const_cast<void*>(static_cast<const void*>(&cur_freq_hz_));

		// RtAudio::StreamOptions options;
		// options.flags = RTAUDIO_NONINTERLEAVED;

		RtAudioErrorType error;
		error = rt_audio_.openStream(&parameters, nullptr, RTAUDIO_FLOAT32
				, G_SAMPLE_RATE_HZ, &G_BUFFER_SAMPLE_FRAMES, &Shapes::sinwave, user_data);
		if (error) {
			std::println("error opening a stream: {}/n", rt_audio_.getErrorText());
			return;
		}
	}

	void PlayShit() {
		RtAudioErrorType error;
		error = rt_audio_.startStream();
		if (error) {
			std::println("error starting a stream: {}/n", rt_audio_.getErrorText());
			if (rt_audio_.isStreamOpen()) {
				rt_audio_.closeStream();
			}
			return;
		}
	}

	void Stop() {
		if (rt_audio_.isStreamRunning()) {
			rt_audio_.stopStream();
		}
		if (rt_audio_.isStreamOpen()) {
			rt_audio_.closeStream();
		}
	}

private:
	const std::atomic<double>& cur_freq_hz_;
	RtAudio rt_audio_;
};

}
