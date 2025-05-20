#pragma once

#include "types_and_constants.hpp"

namespace synth {

class Voice {
public:
	Voice(
			const types::frequency_1_over_sampleunits_t t_freq
			, const types::velocity_t t_velocity
	)
			: frequency_1_over_sampleunits_(t_freq)
			, velocity_(t_velocity)
	{
	}

	void Clear() {
		is_playing = false;
		time_at_this_note_sampleunits = 0;
	}

	types::frequency_1_over_sampleunits_t
	GetFrequency() const {
		return frequency_1_over_sampleunits_;
	}

	types::velocity_t
	GetVelocity() const {
		return velocity_;
	}

public:
	types::time_sampleunits_t time_at_this_note_sampleunits = 0;
	bool is_playing = true;

private:
	const types::frequency_1_over_sampleunits_t frequency_1_over_sampleunits_;
	const types::velocity_t velocity_;
};

}
