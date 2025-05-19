#pragma once

#include "types_and_constants.hpp"

namespace synth {

class Voice {
public:
	explicit Voice(const types::frequency_1_over_sampleunits_t t_freq)
			: frequency_1_over_sampleunits_(t_freq)
	{
	}

	void Clear() {
		is_playing = false;
		time_at_this_note_sampleunits = 0;
	}

	types::frequency_1_over_sampleunits_t GetFrequency() const {
		return frequency_1_over_sampleunits_;
	}

public:
	types::time_sampleunits_t time_at_this_note_sampleunits = 0;
	bool is_playing = true;

private:
	types::frequency_1_over_sampleunits_t frequency_1_over_sampleunits_ = 0;
	// // TODO: add velocity with this
	// types::amplitude_coef_t amplitude_coef;
};

// class NoteBundle {
// public:
// 	NoteBundle(
// 		types::oscilator_t t_oscilator
// 		, const envelopes::ADSR& t_adsr
// 		, const types::frequency_1_over_sampleunits_t t_freq = 0
// 		, bool t_is_playing = true
// 	)
// 		: oscilator(t_oscilator)
// 		, adsr(t_adsr)
// 		, frequency_1_over_sampleunits_(t_freq)
// 		, is_playing(t_is_playing)
// 	{
// 	}
//
// 	void SetNote(const types::frequency_1_over_sampleunits_t& frequency_1_over_sampleunits) {
// 		frequency_1_over_sampleunits_ = frequency_1_over_sampleunits;
// 		time_at_this_note_sampleunits = 0;
// 		is_playing = true;
// 	}
//
// 	void ClearNote() {
// 		is_playing = false;
// 		time_at_this_note_sampleunits = 0;
// 	}
//
// 	const types::frequency_1_over_sampleunits_t& GetFrequency() const {
// 		return frequency_1_over_sampleunits_;
// 	}
//
// public:
// 	const types::oscilator_t oscilator;
// 	const envelopes::ADSR& adsr;
// 	// for the oscilator envelope
// 	types::time_sampleunits_t time_at_this_note_sampleunits = 0;
// 	bool is_playing = false;
//
// private:
// 	types::frequency_1_over_sampleunits_t frequency_1_over_sampleunits_ = 0;
// 	// // TODO: add velocity with this
// 	// types::amplitude_coef_t amplitude_coef;
// };

}
