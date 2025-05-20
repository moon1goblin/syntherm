#include <csignal>
#include <print>

#include "oscilator.hpp"
#include "envelopes.hpp"
#include "types_and_constants.hpp"
#include "voicemanager.hpp"
#include "midiin.hpp"
#include "audioout.hpp"

int main() {
	auto my_envelope = synth::envelopes::ADSR::MakeEnvelope(40, 50, 30, 200);
	if (!my_envelope) {
		std::println("failed to make envelope");
		return 1;
	}

	// its loud as fuck
	double total_amplitude = 0.00000000001;

	// polyphony sounds very distorted, no fucking clue why
	synth::VoiceManager my_voice_manager(
			synth::oscilators::TriangleWave()
			, *my_envelope
			, synth::envelopes::VelocityCurve()
			, total_amplitude
	);

	auto my_midi_in = synth::MidiIn::MakeMidiIn(my_voice_manager);
	if (!my_midi_in) {
		std::println("failed to make midi in");
		return 1;
	}

	auto my_audio_out = synth::AudioOut::MakeAudioOut(my_voice_manager);
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

	std::println("get yourself a midi keyboard and choose \"my synth\" as the virtual port");
	std::println("press Ctrl+C to quit");

	while (!done) {
	}

	my_audio_out->Stop();

	return 0;
}
