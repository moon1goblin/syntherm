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

	void SetNote(const types::freq_hz_t& frequency_hz) {
		frequency_hz_ = frequency_hz;
		time_at_this_note_sampleunits = 0;
		is_playing = true;
	}

	void ClearNote() {
		is_playing = false;
		time_at_this_note_sampleunits = 0;
	}

	const types::freq_hz_t& GetFrequency() {
		return frequency_hz_;
	}

public:
	Oscilator oscilator;
	Envelope oscilator_envelope;
	// for the oscilator envelope
	types::time_sampleunits_t time_at_this_note_sampleunits = 0;
	bool is_playing = false;

private:
	types::freq_hz_t frequency_hz_ = 0;
	// // TODO: add velocity with this
	// types::amplitude_coef_t amplitude_coef;
};

}
