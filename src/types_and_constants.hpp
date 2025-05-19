#pragma once

#include <cstdint>
#include <functional>

namespace synth {

namespace types {
	// using freq_hz_t = double;
	// TODO: make wrappers for types that keep range in mind
	using frequency_1_over_sampleunits_t = double;
	using amplitude_coef_t = double;
	// TODO: what if time overflows? should probably reset it once in a while, not now though
	using time_sampleunits_t = std::uint32_t;
	using time_seconds_t = double;
	using oscilator_t = std::function<double(
		const types::frequency_1_over_sampleunits_t frequency_1_over_sampleunits
		, const types::time_sampleunits_t& time_sample_units
	)>;
}

namespace constants {
	constexpr std::uint32_t sample_rate_hz = 44100;
	// it was loud as fuck, i dont know whats wrong with it
	// constexpr types::amplitude_coef_t total_amplitude = 0.000000001;
	// library couldnt handle the const on this
	std::uint32_t buffer_sample_frames = 256;
	constexpr std::uint8_t max_polyphony_voices = 8;
}

}
