#pragma once

#include <RtAudio.h>
#include <optional>

#include "types_and_constants.hpp"
#include "midiparser.hpp"
#include "midiin.hpp"
#include "voice.hpp"
#include "voicemanager.hpp"

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

	synth::VoiceManager* voice_manager_ptr = static_cast<synth::VoiceManager*>(user_data);
	voice_manager_ptr->RemoveDeadVoices();

	for(std::size_t i = 0; i < buffer_frames; ++i) {
		*buf_doubles = 0;
		for (std::optional<synth::Voice>& cur_voice : voice_manager_ptr->GetVoices()) {
			if (!cur_voice) {
				continue;
			}
			*buf_doubles += 
				voice_manager_ptr->total_amplitude
				* voice_manager_ptr->oscilator(cur_voice->GetFrequency(), cur_time_sampleunits)
				* voice_manager_ptr->adsr(
					cur_voice->time_at_this_note_sampleunits
					, cur_voice->is_playing
			);
			++cur_voice->time_at_this_note_sampleunits;
		}
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

	synth::VoiceManager* voice_manager_ptr = static_cast<synth::VoiceManager*>(user_data);

	// note on
	if (midi_event->Command == 1) {
		voice_manager_ptr->AddVoice(MidiParser::key_to_freq_1_over_sampleunits_lookup()[midi_event->KeyNumber]);
	}
	// note off
	else if (midi_event->Command == 0) {
		voice_manager_ptr->ClearVoice(MidiParser::key_to_freq_1_over_sampleunits_lookup()[midi_event->KeyNumber]);
	}

	// for (const auto& voice : voice_manager_ptr->GetVoices()) {
	// 	if (voice != std::nullopt) {
	// 		std::println("{:0.5f}", voice->GetFrequency());
	// 	}
	// }
	// std::println("");
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
