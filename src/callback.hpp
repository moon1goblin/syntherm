#pragma once

#include <RtAudio.h>
#include <print>

#include "types_and_constants.hpp"
#include "notebundle.hpp"
#include "midiparser.hpp"

namespace synth {
namespace callbacks {

int AudioOutCallback(
	void* outputbuf
	, void* /*inputbuf*/
	, unsigned int buffer_frames
	, double stream_time_seconds
	, ::RtAudioStreamStatus /*stream_status*/
	, void* user_data
) {
	double* buf_doubles = static_cast<double*>(outputbuf);
	types::time_sampleunits_t cur_time_sampleunits = stream_time_seconds * constants::sample_rate_hz;

	synth::NoteBundle* note_bundle_ptr = static_cast<synth::NoteBundle*>(user_data);

	for(std::size_t i = 0; i < buffer_frames; ++i) {
		*buf_doubles = 
			constants::total_amplitude
			* note_bundle_ptr->oscilator(note_bundle_ptr->GetFrequency(), cur_time_sampleunits)
			* note_bundle_ptr->adsr(
				note_bundle_ptr->time_at_this_note_sampleunits
				, note_bundle_ptr->is_playing
			);

		++note_bundle_ptr->time_at_this_note_sampleunits;
		++cur_time_sampleunits;
		++buf_doubles;
	}

	return 0;
}

void MidiInCallback(
	double /*timeStamp*/
	, std::vector<std::uint8_t>* message_vec
	, void* user_data
) {
	std::optional<MidiParser::MidiEvent> midi_event = MidiParser::Parse(*message_vec);
	if (!midi_event) {
		return;
	}

	synth::NoteBundle* note_bundle_ptr = static_cast<synth::NoteBundle*>(user_data);

	// std::println("current keynumber: {}", midi_event->KeyNumber);
	// std::println("current command: {}", midi_event->Command);
	// std::println("current note: {}"
	// 		, MidiParser::key_to_freq_lookup()[midi_event->KeyNumber]);

	// note on
	if (midi_event->Command == 1) {
		note_bundle_ptr->SetNote(MidiParser::key_to_freq_1_over_sampleunits_lookup()[midi_event->KeyNumber]);
	}
	// note off
	else if (midi_event->Command == 0){
		// doesnt do anything yet
		note_bundle_ptr->ClearNote();
	}
}

// void MidiInErrorCallback(
// 	::RtMidiError::Type type
// 	, const std::string& errorText
// 	, void* /*userData*/
// ) {
// 	std::println("error with the midi in has occured: {}\n", errorText);
// }

}
}
