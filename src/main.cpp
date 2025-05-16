#include "midiin.hpp"
#include <csignal>
// #include "soundout.hpp"

int main() {
	synth::MidiIn midi_in{};

	static bool done = false;
	std::signal(SIGINT, [](int) {
		done = true;
	});

	midi_in.RecieveShit();

	while (!done) {
	}

	midi_in.Stop();

	return 0;
}
