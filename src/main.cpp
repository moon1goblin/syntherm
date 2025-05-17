#include <csignal>

#include "rtshit/audioout.hpp"
#include "rtshit/midiin.hpp"

int main() {
	// TODO: add ability to change envelope in real time
	auto my_envelope = synth::Envelope::MakeEnvelope(30, 40, 50, 300);
	if (!my_envelope) {
		std::println("failed to make envelope");
		return 1;
	}

	// bundle is the communication method between midi in and audio out
	synth::NoteBundle my_bundle(synth::Oscilator(), *my_envelope);

	auto my_midi_in = synth::MidiIn::MakeMidiIn(my_bundle);
	if (!my_midi_in) {
		std::println("failed to make midi in");
		return 1;
	}

	auto my_audio_out = synth::AudioOut::MakeAudioOut(my_bundle);
	if (!my_audio_out) {
		std::println("failed to make audio out");
		return 1;
	}

	my_midi_in->RecieveShit();
	my_audio_out->PlayShit();

	static bool done = false;
	std::signal(SIGINT, [](int) {
		done = true;
	});
	while (!done) {
	}

	my_audio_out->Stop();

	return 0;
}
