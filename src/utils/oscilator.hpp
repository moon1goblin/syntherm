#pragma once

#include <cmath>

#include "types_and_constants.hpp"

namespace synth {

// TODO: make this a virtual base class and add other wave shapes, for now this
class Oscilator {
public:
	static double Sinwave(types::freq_hz_t frequency_hz, types::time_sampleunits_t time_sample_units) {
		return std::sin(constants::sin_arg_sample_units 
				* frequency_hz 
				* time_sample_units);
	}
};

}
