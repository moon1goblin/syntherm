#pragma once

#include <cstdint>
#include <numbers>

namespace synth {

namespace types {
	using freq_hz_t = double;
	using amplitude_coef_t = double;
	// TODO: what if time overflows? should probably reset it once in a while, not now though
	using time_sampleunits_t = std::uint32_t;
	using time_seconds_t = double;
}

namespace constants {
	constexpr std::uint32_t sample_rate_hz = 44100;
	// it was loud as fuck, i dont know whats wrong with it
	constexpr types::amplitude_coef_t total_amplitude = 0.0000001;
	// library couldnt handle the const on this
	std::uint32_t buffer_sample_frames = 256;
	// for the sinwave oscilator, [1 / hz]
	constexpr double sin_arg_sample_units = 2.0 * std::numbers::pi / sample_rate_hz;
}

}
