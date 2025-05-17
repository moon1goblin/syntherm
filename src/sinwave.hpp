#pragma once
#include <numbers>
#include <cstdint>
#include <RtAudio.h>
#include <print>

#include "soundout.hpp"

namespace synth {

class AudioOut;

constexpr unsigned int G_SAMPLE_RATE_HZ = 44100;
// it was loud as fuck, i dont know whats wrong with it
constexpr double G_AMPLITUDE = 0.000001;
// library couldnt handle the const on this
std::uint32_t G_BUFFER_SAMPLE_FRAMES = 256;

class Shapes {
public:
	static int sinwave(void* outputbuf
			, void* /*inputbuf*/
			, std::uint32_t buffer_frames
			, double stream_time_seconds
			, RtAudioStreamStatus /*stream_status*/
			, void* user_data)
	{
		// getting some parameters in order
		double* buf
			= static_cast<double*>(outputbuf);
		synth::UserDataBundle* data_bundle_ptr
			= static_cast<synth::UserDataBundle*>(user_data);
		std::uint32_t cur_time_samples
			= static_cast<std::uint32_t>(stream_time_seconds * G_SAMPLE_RATE_HZ);

		static std::uint16_t cur_freq_hz = 0;
		static double sin_arg = 0.0;

		// meassure time at current note for the adsr envelope
		// TODO: i dont think this is a good way to do it
		static std::uint32_t cur_time_at_this_note_samples = 0;

		std::uint16_t new_freq_hz = data_bundle_ptr->cur_freq_hz.load();

		// std::println("new freq: {}", new_freq_hz);
		// std::println("cur freq: {}", cur_freq_hz);

		if (cur_freq_hz != new_freq_hz) {
			// FIXME: fix this mess
			std::println("im not here though");
			cur_freq_hz = new_freq_hz;

			cur_time_at_this_note_samples = 0;
			sin_arg = 2.0 * std::numbers::pi * cur_freq_hz / G_SAMPLE_RATE_HZ;
		}

		constexpr double env_arg = 1.0 / G_SAMPLE_RATE_HZ;

		// // does this even work? worry about optimisation later though
		// if (cur_freq_hz == 0) {
		// 	return 0;
		// }

		for(std::uint32_t i = 0; i < buffer_frames; ++i) {
			*buf = std::sin(sin_arg * cur_time_samples) 
				* data_bundle_ptr->envelope.GetEnvelopeCoefficient(
						env_arg * cur_time_at_this_note_samples)
				* G_AMPLITUDE;
			++buf;
			++cur_time_samples;
			++cur_time_at_this_note_samples;
		}

		return 0;
	}
};

}
