#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <algorithm>
#include <print>
#include <ranges>

#include "types_and_constants.hpp"
#include "envelopes.hpp"
#include "voice.hpp"

namespace synth {

class VoiceManager {
public:
	VoiceManager(
			types::oscilator_t t_oscilator
			, const envelopes::ADSR& t_adsr
			, envelopes::VelocityCurve t_velocity_curve
			, double t_total_amplitude
	) 
			: oscilator(t_oscilator)
			, adsr(t_adsr)
			, velocity_curve(t_velocity_curve)
			, total_amplitude(t_total_amplitude)
	{
	}

	void AddVoice(
			const types::frequency_1_over_sampleunits_t frequency
			, const types::velocity_t velocity
	) {
		if (voices_occupied >= constants::max_polyphony_voices) {
			return;
		}
		auto free_place = std::ranges::find_if(
				voices_container_
				, [](const std::optional<synth::Voice>& voice) { 
					return !voice;
				}
		);
		// theoretically shouldnt need this but just to be safe
		if (free_place != voices_container_.end()) {
			free_place->emplace(frequency, velocity);
			++voices_occupied;
		}
	}

	void ClearVoice(
			const types::frequency_1_over_sampleunits_t frequency
			, const types::velocity_t velocity
	) {
		auto find_all_by_freq = voices_container_ 
			| std::views::filter([=](const auto& voice) {
				return voice 
					&& voice->GetFrequency() == frequency;
					// so yea note off messages can have velocity of 0, cant use this
					// && voice->GetVelocity() == velocity;
			}
		);
		for (auto& voice : find_all_by_freq) {
			voice->Clear();
		}
	}

	void RemoveDeadVoices() {
		for (std::optional<synth::Voice>& voice : voices_container_) {
			if (voices_occupied == 0) {
				break;
			}
			if (voice && adsr.IsDonePlaying(voice->time_at_this_note_sampleunits, voice->is_playing)) {
				voice.reset();
				--voices_occupied;
			}
		}
	}

	std::array<std::optional<synth::Voice>, constants::max_polyphony_voices>&
	GetVoices()
	{
		return voices_container_;
	}

public:
	types::oscilator_t oscilator;
	envelopes::ADSR adsr;
	envelopes::VelocityCurve velocity_curve;
	double total_amplitude;

private:
	std::uint8_t voices_occupied = 0;
	std::array<std::optional<synth::Voice>, constants::max_polyphony_voices> voices_container_;
};

}
