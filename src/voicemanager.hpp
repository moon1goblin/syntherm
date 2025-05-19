#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <algorithm>
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
			, types::amplitude_coef_t t_total_amplitude
	) 
			: oscilator(t_oscilator)
			, adsr(t_adsr)
			, total_amplitude(t_total_amplitude)
	{
	}

	void AddVoice(types::frequency_1_over_sampleunits_t freq) {
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
			*free_place = std::make_optional<synth::Voice>(synth::Voice(freq));
			++voices_occupied;
		}
	}

	void ClearVoice(types::frequency_1_over_sampleunits_t freq) {
		auto find_all_by_freq = voices_container_ 
			| std::views::filter([freq](const auto& voice) {
				return voice && voice->GetFrequency() == freq;
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
				voice = std::nullopt;
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
	types::amplitude_coef_t total_amplitude;

private:
	std::uint8_t voices_occupied = 0;
	std::array<std::optional<synth::Voice>, constants::max_polyphony_voices> voices_container_;
};

}
