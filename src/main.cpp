#include <csignal>
#include <optional>
#include <print>

#include "envelope.hpp"
#include "midiin.hpp"
#include "notebundle.hpp"
#include "oscilator.hpp"
#include "audioout.hpp"

int main() {
	// doesnt do anything for the release yet
	auto my_envelope = synth::Envelope::MakeEnvelope(30, 100, 0, 200);
	if (!my_envelope) {
		std::println("failed to make envelope");
		return 1;
	}

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

	static bool done = false;
	std::signal(SIGINT, [](int) {
		done = true;
	});

	my_midi_in->RecieveShit();
	my_audio_out->PlayShit();

	while (!done) {
	}

	my_audio_out->Stop();

	return 0;
}
