#pragma once

#include <cstdlib>
#include <optional>
#include <print>
#include <RtMidi.h>

#include "midiparser.hpp"
#include "notebundle.hpp"

namespace synth {

namespace callbacks {

static void MidiInErrorCallback(
	RtMidiError::Type type
	, const std::string& errorText
	, void* /*userData*/
) {
	std::println("error with the midi in has occured: {}\n", errorText);
}

static void MidiInCallback(
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
		note_bundle_ptr->SetNote(MidiParser::key_to_freq_lookup()[midi_event->KeyNumber]);
	}
	// note off
	else if (midi_event->Command == 0){
		// doesnt do anything yet
		note_bundle_ptr->ClearNote();
	}
}

}

class MidiIn {
private:
	MidiIn(synth::NoteBundle& t_note_bundle)
		: midi_in_()
		, note_bundle_(t_note_bundle)
	{
	}

public:
	[[nodiscard]] static std::optional<MidiIn> MakeMidiIn(synth::NoteBundle& t_note_bundle)
	{
		try {
			return std::optional<MidiIn>(MidiIn(t_note_bundle));
		} catch (::RtMidiError error) {
			std::println(stderr, "error constructing a midi in instance: {}", error.getMessage());
			return std::nullopt;
		}
	}

	void RecieveShit() {
		void* user_data = static_cast<void*>(&note_bundle_);

		midi_in_.setErrorCallback(&callbacks::MidiInErrorCallback);
		midi_in_.setCallback(&callbacks::MidiInCallback, user_data);

		// // Don't ignore sysex, timing, or active sensing messages.
		// midi_in_.ignoreTypes(true, true, true);
		midi_in_.openVirtualPort("my synth");
	}
	
	// void Stop() {
	// }
	
private:
	::RtMidiIn midi_in_;
	synth::NoteBundle& note_bundle_;
};

}
