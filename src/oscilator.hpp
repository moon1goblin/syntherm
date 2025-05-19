#pragma once

#include <cmath>
#include <numbers>

#include "types_and_constants.hpp"

namespace synth {
namespace oscilators {

class SinWave {
public:
	double operator()(
		const types::freq_1_over_sampleunits_t frequency_1_over_sampleunits
		, const types::time_sampleunits_t& time_sample_units
	) const {
		return std::sin(2.0 * std::numbers::pi * frequency_1_over_sampleunits * time_sample_units);
	}
};

class SawWave {
public:
	double operator()(
		const types::freq_1_over_sampleunits_t frequency_1_over_sampleunits
		, const types::time_sampleunits_t& time_sample_units
	) const {
		// [0, +1]: (time_sampleunits % period_sampleunits) / period_sampleunits
		// [-1, +1]: 2 * that - 1
		return 2.0 * std::fmod(
				static_cast<double>(time_sample_units)
				, 1 / frequency_1_over_sampleunits
			) * frequency_1_over_sampleunits - 1;
	}
};

}
}
