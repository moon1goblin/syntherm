#include <csignal>
#include <print>

#include "audioout.hpp"
#include "midiin.hpp"
#include "oscilator.hpp"

int main() {
	// TODO: change envelope and oscilator in real time
	synth::Oscilator my_oscilator(synth::Oscilator::OscilatorType::SAW);

	// adsr
	auto my_envelope = synth::Envelope::MakeEnvelope(10, 100, 50, 0);
	if (!my_envelope) {
		std::println("failed to make envelope");
		return 1;
	}

	// bundle is the communication method between midi in and audio out
	synth::NoteBundle my_bundle(my_oscilator, *my_envelope);

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
	std::println("get yourself a (virtual) midi keyboard and choose \"my synth\" in the connections");
	std::println("press Ctrl+C to quit");
	while (!done) {
	}

	my_audio_out->Stop();

	return 0;
}
