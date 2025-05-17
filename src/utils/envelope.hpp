#pragma once

#include "types_and_constants.hpp"
#include <optional>
#include <print>

namespace synth {

class Envelope {
private:
	Envelope(const double& attack_delta_ms
		, const double& decay_delta_ms
		, const double& sustain_percents
		, const double& release_delta_ms
	)
		: attack_sampleunits_(attack_delta_ms * 0.001 * constants::sample_rate_hz)
		, decay_sampleunits_((attack_delta_ms + decay_delta_ms) * 0.001 * constants::sample_rate_hz)
		, sustain_coef_(sustain_percents * 0.01)
		, release_sampleunits_(release_delta_ms * 0.001 * constants::sample_rate_hz)
	{
		if (attack_sampleunits_ < -0.001 || decay_sampleunits_ < -0.001 || release_sampleunits_ < -0.001
			|| sustain_coef_ < -0.001 || sustain_coef_ > 1.001) {
			throw;
		}
	}

public:
	[[nodiscard]] static std::optional<Envelope> MakeEnvelope(
		const double& attack_delta_ms
		, const double& decay_delta_ms
		, const double& sustain_percents
		, const double& release_delta_ms
	) {
		try {
			return std::optional<Envelope>(Envelope(
				attack_delta_ms
				, decay_delta_ms
				, sustain_percents
				, release_delta_ms
			));
		} catch (...) {
			std::println(stderr, "error making an envelope: invalid envelope parameters");
		}
		return std::nullopt;
	}

	// returns a coefficient from 0 to 1
	double GetEnvelopeCoefficient(const types::time_sampleunits_t& time_sampleunits, bool is_playing) const { 
		// helper lambda
		auto between = [&time_sampleunits](double left, double right) -> bool
		{
			return time_sampleunits >= left && time_sampleunits <= right;
		};

		// attack
		if (is_playing && between(0, attack_sampleunits_)) {
			// linear curve, fuck it
			return static_cast<double>(time_sampleunits) / attack_sampleunits_;
		}

		// decay
		else if (is_playing && between(attack_sampleunits_, decay_sampleunits_)) {
			return (static_cast<double>(time_sampleunits) - attack_sampleunits_) * (sustain_coef_ - 1) / (decay_sampleunits_ - attack_sampleunits_) + 1;
		}

		// sustain
		else if (is_playing && time_sampleunits >= decay_sampleunits_) {
			return sustain_coef_;
		}

		// release
		if (!is_playing && time_sampleunits <= release_sampleunits_) {
			return - static_cast<double>(time_sampleunits) * sustain_coef_ / release_sampleunits_ + sustain_coef_;
		}

		else {
			return 0.0;
		}
	}

private:
	double attack_sampleunits_;
	double decay_sampleunits_;
	double sustain_coef_; // 0 to 1
	double release_sampleunits_;
};

}
