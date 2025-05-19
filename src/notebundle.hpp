#pragma once

#include "types_and_constants.hpp"
#include <functional>
#include <print>

namespace synth {

class NoteBundle {
public:
	using adsr_t = std::function<double(
		const types::time_sampleunits_t& time_at_this_note_sampleunits
		, bool is_playing
	)>;
	 using oscilator_t = std::function<double(
		 const types::freq_1_over_sampleunits_t frequency_1_over_sampleunits
		 , const types::time_sampleunits_t& time_sample_units
	 )>;

public:
	// NoteBundle(const synth::oscilators::SinWave& t_oscilator, const envelopes::ADSR& t_adsr)
	NoteBundle(oscilator_t t_oscilator, adsr_t t_adsr)
	: oscilator(t_oscilator)
	, adsr(t_adsr)
	{
	}

	void SetNote(const types::freq_1_over_sampleunits_t& frequency_1_over_sampleunits) {
		frequency_1_over_sampleunits_ = frequency_1_over_sampleunits;
		time_at_this_note_sampleunits = 0;
		is_playing = true;
		std::println("set note called");
	}

	void ClearNote() {
		is_playing = false;
		time_at_this_note_sampleunits = 0;
	}

	const types::freq_1_over_sampleunits_t& GetFrequency() const {
		return frequency_1_over_sampleunits_;
	}

	// // TODO: make this call the destructor if adsr is finished
	// void IncrementTimeAtThisNote() {
	// }

public:
	oscilator_t oscilator;
	adsr_t adsr;
	// for the oscilator envelope
	types::time_sampleunits_t time_at_this_note_sampleunits = 0;
	bool is_playing = false;

private:
	types::freq_1_over_sampleunits_t frequency_1_over_sampleunits_ = 0;
	// // TODO: add velocity with this
	// types::amplitude_coef_t amplitude_coef;
};

}
