#include <csignal>
#include <cstdlib>
#include <expected>
#include <print>

#include "oscilator.hpp"
#include "envelopes.hpp"
#include "types_and_constants.hpp"
#include "voicemanager.hpp"
#include "midiin.hpp"
#include "audioout.hpp"

int main() {
	auto handle_error = []<typename T>(const std::expected<T, std::string>& expected) {
		if (!expected.has_value()) {
			std::println("bruuuh {}", expected.error());
			exit(1);
		}
	};

	auto my_envelope = synth::envelopes::ADSR::CreateEnvelope(40, 50, 30, 200);
	handle_error(my_envelope);

	// its loud as fuck
	double total_amplitude = 0.00000000001;

	// polyphony sounds very distorted, no fucking clue why
	synth::VoiceManager my_voice_manager(
			synth::oscilators::TriangleWave()
			, *my_envelope
			, synth::envelopes::VelocityCurve()
			, total_amplitude
	);

	auto my_midi_in = synth::MidiIn::CreateMidiIn(my_voice_manager);
	handle_error(my_midi_in);

	auto my_audio_out = synth::AudioOut::CreateAudioOut(my_voice_manager);
	handle_error(my_audio_out);

	my_midi_in->RecieveShit();
	handle_error(my_audio_out->PlayShit());

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
