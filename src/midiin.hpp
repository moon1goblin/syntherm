#pragma once

#include <cstdlib>
#include <optional>
#include <print>
#include <RtMidi.h>

#include "notebundle.hpp"
#include "callback.hpp"

namespace synth {

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

		// midi_in_.setErrorCallback(&synth::callbacks::MidiInErrorCallback);
		midi_in_.setCallback(&synth::callbacks::MidiInCallback, user_data);

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
