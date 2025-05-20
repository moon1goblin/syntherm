#pragma once
#include <cstdlib>
#include <optional>
#include <vector>

namespace synth {

class MidiParser {
public:
	struct MidiEvent {
		uint8_t Command = 0;
		uint8_t TrackNumber = 0;
		// only proccessing the note on/off events for now
		uint8_t KeyNumber;
		uint8_t Velocity;
		// 	1000nnnn 0kkkkkkk 0vvvvvvv Note Off event
		// 	1001nnnn 0kkkkkkk 0vvvvvvv Note On event
	};

	static auto Parse(const std::vector<std::uint8_t>& message_buffer) -> std::optional<MidiEvent> {
		uint8_t buffer;
		size_t cur_index_in_buffer = 0;

		// std::println("[midi parser] message length: {}", message_buffer.size());

		auto read_bytes_from_message = [&](size_t how_many = 1) -> void {
			while (how_many) {
				if (cur_index_in_buffer >= message_buffer.size()) {
					// for some reason it wasnt catching the throw; without the string
					throw "bruh";
				}
				buffer = message_buffer[cur_index_in_buffer];
				++cur_index_in_buffer;
				--how_many;
			}
			// std::println("[midi parser] read byte: {:08b}", buffer);
		};

		auto read_variable_length = [&]() mutable -> uint32_t {
			uint32_t value = 0;
			do {
				read_bytes_from_message();
				value = (value << 7) + (buffer & ~(1 << 7));
				buffer = buffer & (1 << 7);
			} while (buffer & (1 << 7));

			return value;
		};

		try {
			// // we are only reading the first event lmao
			// uint32_t delta_time = read_variable_length();
			read_bytes_from_message();

			// // sysex event
			// // F0 <length> <sysex_data>
			// // F7 <length> <any_data>
			// // todo: proccess sysex and meta events for real
			// if (buffer == 0xF0 || buffer == 0xF7) {
			// 	// uint32_t sysex_event_length;
			// 	// bytes_read += ReadVariableLength(ifs, sysex_event_length);
			// 	// ifs.ignore(sysex_event_length);
			// 	do {
			// 		read_bytes_from_message();
			// 	} while (buffer != 0xF7);
			// }
			// // meta event
			// // FF <type> <length> <data>
			// else if (buffer == 0xFF) {
			// 	read_bytes_from_message();
			// 	uint32_t meta_event_length = read_variable_length();
			// 	cur_index_in_buffer+=meta_event_length;
			// }
			// // midi event duh
			// else {
				// TODO: take all theses statics and make them class members
				static uint16_t last_midi_command;
				static uint16_t last_midi_track_number;
				static bool running_event = false;
				MidiEvent midi_event;

				if (buffer & (1 << 7)) {
					// std::println("midi event");
					midi_event.TrackNumber = buffer & (0b0000'1111);
					midi_event.Command = (buffer & (0b0111'0000)) >> 4;
					read_bytes_from_message();
					running_event = true;
				}
				else if (running_event){
					// std::println("running midi event");
					midi_event.Command = last_midi_command;
					midi_event.TrackNumber = last_midi_track_number;
				}
				else {
					// std::println("not a midi event");
					running_event = false;
					return std::nullopt;
				}

				if (midi_event.Command == 0 || midi_event.Command == 1) {
					// std::println("a note on/off midi event");
					midi_event.KeyNumber = buffer;
					read_bytes_from_message();
					midi_event.Velocity = buffer;

					if (midi_event.Velocity == 0) {
						midi_event.Command = 0;
					}

					return std::make_optional(midi_event);
				}
			// }
		} catch (...) {
		}
		return std::nullopt;
	}
};

}
