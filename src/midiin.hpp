#include <atomic>
#include <cstdint>
#include <memory>
#include <cstdlib>
#include <csignal>
#include <optional>
#include <print>
#include <RtMidi.h>

#include "midiparser.hpp"

namespace synth {

void Callback(double /*timeStamp*/, std::vector<unsigned char>* message_vec, void* user_data) {
	if (!message_vec->empty()) {
		// im really sure this will not work but lets try it
		std::atomic<double>& cur_freq = *static_cast<std::atomic<double>*>(user_data);
		// if (cur_freq != 0) {
		// 	std::println("cur_freq {}", cur_freq.load());
		// }

		std::optional<MidiParser::MidiEvent> midi_event = MidiParser::Parse(*message_vec);
		if (midi_event) {
			std::println("current keynumber: {}", midi_event->KeyNumber);
			std::println("current command: {}", midi_event->Command);
			std::println("current note: {}"
					, MidiParser::key_to_freq_lookup()[midi_event->KeyNumber]);
			if (midi_event->Command == 1) {
				cur_freq.store(MidiParser::key_to_freq_lookup()[midi_event->KeyNumber]);
			}
			else {
				cur_freq.store(0);
			}
		}
	}
}

class MidiIn {
public:
	MidiIn(std::atomic<double>& t_cur_freq_)
		: cur_freq_(t_cur_freq_)
	{
		try {
			midi_in_ = std::make_shared<::RtMidiIn>();
		} catch (::RtMidiError error) {
			std::println("error constructing a midi in instance: {}", error.getMessage());
			return;
		}
	}

	void RecieveShit() {
		// is_running = true;
		midi_in_->openVirtualPort();
		// // Don't ignore sysex, timing, or active sensing messages.
		midi_in_->ignoreTypes(true, false, true);

		std::vector<std::uint8_t> message_vec;
		void* user_data = static_cast<void*>(&cur_freq_);

		midi_in_->setCallback(&Callback, user_data);
	}
	
	void Stop() {
	}
	
private:
	std::atomic<double>& cur_freq_;
	std::shared_ptr<::RtMidiIn> midi_in_;
	// std::thread midi_thr_;
	// std::atomic<bool> is_running{false};
};

}
