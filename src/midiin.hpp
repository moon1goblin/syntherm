#include <atomic>
#include <cstdint>
#include <memory>
#include <cstdlib>
#include <csignal>
#include <optional>
#include <print>
#include <thread>
#include <RtMidi.h>

#include "midiparser.hpp"

namespace synth {

class MidiIn {
public:
	MidiIn() {
		try {
			midiin_ = std::make_shared<::RtMidiIn>();
		} catch (::RtMidiError error) {
			std::println("error constructing a midi in instance: {}", error.getMessage());
			return;
		}
	}

	void RecieveShit() {
		is_running = true;

		midiin_->openVirtualPort();
		// // Don't ignore sysex, timing, or active sensing messages.
		midiin_->ignoreTypes(true, false, true);

		std::vector<std::uint8_t> message_vec;

		// TODO: understand if this is right, does it copy the this ptr or the this object
		// but fuck it it works lmao
		midi_thr_ = std::thread([&message_vec, this]{
			while(is_running.load()) {
				midiin_->getMessage(&message_vec);
				if (!message_vec.empty()) {
					std::optional<MidiParser::MidiEvent> midi_event = MidiParser::Parse(message_vec);
					if (midi_event) {
						std::println("current keynumber: {}", midi_event->KeyNumber);
						std::println("current command: {}", midi_event->Command);
						std::println("current note: {}"
								, MidiParser::key_to_freq_lookup()[midi_event->KeyNumber]);
					}
				}
			}
		});
	}
	
	void Stop() {
		is_running = false;
		midi_thr_.join();
	}

public:
	std::atomic<std::uint16_t> cur_freq{0};
private:
	std::shared_ptr<::RtMidiIn> midiin_;
	std::thread midi_thr_;
	std::atomic<bool> is_running{false};
};

}
