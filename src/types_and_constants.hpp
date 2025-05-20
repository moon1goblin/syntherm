#pragma once

#include <cstdint>
#include <functional>
#include <print>
#include <gcem.hpp>

namespace synth {

namespace types {
	// using freq_hz_t = double;
	// TODO: make wrappers for types that keep range in mind
	using frequency_1_over_sampleunits_t = double;
	using velocity_t = std::uint8_t;
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
	// library couldnt handle the const on this
	std::uint32_t buffer_sample_frames = 256;
	constexpr std::uint8_t max_polyphony_voices = 8;

	constexpr std::array<double, 128> velocity_lookup = std::invoke(
		[]() {
			std::array<double, 128> lookup{};
			constexpr double normalizator = gcem::log2(0.1 * 127 + 2) - 1;
			for (std::size_t i = 0; auto& elem : lookup) {
				elem = (gcem::log2(0.1 * i++ + 2) - 1) / normalizator;
			}
			return lookup;
		}
	);

	constexpr std::array<double, 128> key_to_freq_lookup = std::invoke(
		[]() {
			std::array<double, 128> lookup{0};
			uint8_t key_number = 0;
			for (double& freq : lookup) {
				// using gcem because std::pow is not constexpr for some fucking reason
				// this is for 1 / hz
				// freq = 440 * gcem::pow(2, (static_cast<float>(key_number) - 69) / 12);
				// and this is for 1 / sampleunit
				freq = 440.0 * gcem::pow(2, (static_cast<float>(key_number) - 69) / 12) / constants::sample_rate_hz;
				++key_number;
			}
			return lookup;
		}
	);


}

}
