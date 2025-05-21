#pragma once

#include <cstdlib>
#include <expected>
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
			: voice_manager_(t_voice_manager)
	{
		try {
			RtMidiIn midi_in_{};
		} catch (::RtMidiError error) {
			throw std::string("error constructing a midi in instance: " + error.getMessage());
		}
	}

public:
	[[nodiscard]] static std::expected<MidiIn, std::string> 
	CreateMidiIn(synth::VoiceManager& voice_manager) {
		try {
			return MidiIn(voice_manager);
		} catch (const std::string& error) {
			return std::unexpected(error);
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
