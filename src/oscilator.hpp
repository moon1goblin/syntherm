#pragma once

#include <cmath>
#include <numbers>

#include "types_and_constants.hpp"

namespace synth {
namespace oscilators {

// TODO: make lookup tables for all these
class SinWave {
public:
	double operator()(
		const types::frequency_1_over_sampleunits_t frequency_1_over_sampleunits
		, const types::time_sampleunits_t& time_sample_units
	) const {
		return std::sin(2.0 * std::numbers::pi * frequency_1_over_sampleunits * time_sample_units);
	}
};

class SawWave {
public:
	double operator()(
		const types::frequency_1_over_sampleunits_t frequency_1_over_sampleunits
		, const types::time_sampleunits_t& time_sample_units
	) const {
		// [0, +1]: (time_sampleunits % period_sampleunits) / period_sampleunits
		// [-1, +1]: 2 * that - 1
		return 2.0 * std::fmod(
				static_cast<double>(time_sample_units)
				, 1.0 / frequency_1_over_sampleunits
			) * frequency_1_over_sampleunits - 1;
	}
};

class TriangleWave {
public:
	double operator()(
		const types::frequency_1_over_sampleunits_t frequency_1_over_sampleunits
		, const types::time_sampleunits_t& time_sample_units
	) const {
		double normalized_time = std::fmod(
				static_cast<double>(time_sample_units)
				, 1.0 / frequency_1_over_sampleunits
		);
		return normalized_time > 0.5 / frequency_1_over_sampleunits
			? 4 * normalized_time - 1 : 3 - 4 * normalized_time;
	}
};

}
}
