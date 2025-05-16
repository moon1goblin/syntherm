#include <csignal>

#include "midiin.hpp"
#include "soundout.hpp"

int main() {
	std::atomic<double> cur_freq{0.0};

	synth::MidiIn midi_in(cur_freq);
	synth::AudioOut audio_out(cur_freq);

	static bool done = false;
	std::signal(SIGINT, [](int) {
		done = true;
	});

	midi_in.RecieveShit();
	audio_out.PlayShit();

	while (!done) {
	}

	audio_out.Stop();
	midi_in.Stop();

	return 0;
}
