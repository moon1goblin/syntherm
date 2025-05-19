#pragma once

#include <cstdlib>
#include <optional>
#include <print>
#include <RtMidi.h>
#include <RtAudio.h>

#include "voicemanager.hpp"
#include "callbacks.hpp"

namespace synth {

namespace callbacks {

// forward declare
// idk its a whole mess
void MidiInCallback(
	double /*timeStamp*/
	, std::vector<std::uint8_t>* message_vec
	, void* user_data
);

int AudioOutCallback(
	void* outputbuf
	, void* /*inputbuf*/
	, unsigned int buffer_frames
	, double stream_time_seconds
	, ::RtAudioStreamStatus /*stream_status*/
	, void* user_data
);

}

class MidiIn {
private:
	MidiIn(synth::VoiceManager& t_voice_manager)
			: midi_in_()
			, voice_manager_(t_voice_manager)
	{
	}

public:
	[[nodiscard]] static std::optional<MidiIn> MakeMidiIn(synth::VoiceManager& voice_manager) {
		try {
			return std::optional<MidiIn>(MidiIn(voice_manager));
		} catch (::RtMidiError error) {
			std::println(stderr, "error constructing a midi in instance: {}", error.getMessage());
			return std::nullopt;
		}
	}

	void RecieveShit() {
		void* user_data = static_cast<void*>(&voice_manager_);

		// midi_in_.setErrorCallback(&synth::callbacks::MidiInErrorCallback);
		midi_in_.setCallback(&synth::callbacks::MidiInCallback, user_data);

		// // Don't ignore sysex, timing, or active sensing messages.
		// midi_in_.ignoreTypes(true, true, true);
		midi_in_.openVirtualPort("my synth");
	}

private:
	::RtMidiIn midi_in_;
	synth::VoiceManager& voice_manager_;
};

}
