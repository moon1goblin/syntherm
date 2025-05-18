#pragma once

#include "envelope.hpp"
#include "types_and_constants.hpp"
#include "oscilator.hpp"

namespace synth {

class NoteBundle {
public:
	NoteBundle(const synth::Oscilator& t_oscilator, const synth::Envelope& t_oscilator_envelope)
	: oscilator(t_oscilator)
	, oscilator_envelope(t_oscilator_envelope)
	{
	}

	void SetNote(const types::freq_1_over_sampleunits_t& frequency_1_over_sampleunits) {
		frequency_1_over_sampleunits_ = frequency_1_over_sampleunits;
		time_at_this_note_sampleunits = 0;
		is_playing = true;
	}

	void ClearNote() {
		is_playing = false;
		time_at_this_note_sampleunits = 0;
	}

	const types::freq_1_over_sampleunits_t& GetFrequency() {
		return frequency_1_over_sampleunits_;
	}

public:
	Oscilator oscilator;
	Envelope oscilator_envelope;
	// for the oscilator envelope
	types::time_sampleunits_t time_at_this_note_sampleunits = 0;
	bool is_playing = false;

private:
	types::freq_1_over_sampleunits_t frequency_1_over_sampleunits_ = 0;
	// // TODO: add velocity with this
	// types::amplitude_coef_t amplitude_coef;
};

}
