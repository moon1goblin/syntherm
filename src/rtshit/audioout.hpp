#pragma once

#include <RtAudio.h>
#include <cstdio>
#include <memory>
#include <optional>
#include <print>

#include "../utils/envelope.hpp"
#include "../utils/types_and_constants.hpp"
#include "notebundle.hpp"

namespace synth {

namespace callbacks {

static int AudioOutCallback(
	void* outputbuf
	, void* /*inputbuf*/
	, unsigned int buffer_frames
	, double stream_time_seconds
	, ::RtAudioStreamStatus /*stream_status*/
	, void* user_data
) {
	double* buf_doubles = static_cast<double*>(outputbuf);
	types::time_sampleunits_t cur_time_sampleunits = stream_time_seconds * constants::sample_rate_hz;

	synth::NoteBundle* note_bundle_ptr = static_cast<synth::NoteBundle*>(user_data);

	for(std::size_t i = 0; i < buffer_frames; ++i) {
		*buf_doubles = 
			constants::total_amplitude
			* note_bundle_ptr->oscilator.Sinwave(note_bundle_ptr->GetFrequency(), cur_time_sampleunits)
			* note_bundle_ptr->oscilator_envelope.GetEnvelopeCoefficient(
				note_bundle_ptr->time_at_this_note_sampleunits
				, note_bundle_ptr->is_playing
			);
		++note_bundle_ptr->time_at_this_note_sampleunits;
		++cur_time_sampleunits;
		++buf_doubles;
	}

	return 0;
}

}

class AudioOut {
private:
	AudioOut(synth::NoteBundle& t_note_bundle)
		: note_bundle_(t_note_bundle)
	{
		rt_audio_ = std::make_shared<::RtAudio>();
		RtAudio::StreamParameters parameters;
		parameters.deviceId = rt_audio_->getDefaultOutputDevice();
		parameters.nChannels = 2;

		// RtAudio::DeviceInfo info;
		// info = rt_audio_.getDeviceInfo(parameters.deviceId);
		// std::println("device name: {}", info.name);
		// std::println("number of output channels: {}", info.outputChannels);

		void* user_data_for_callback = static_cast<void*>(&note_bundle_);

		RtAudioErrorType error = rt_audio_->openStream(
			&parameters
			, nullptr
			, RTAUDIO_FLOAT32
			, constants::sample_rate_hz
			, &constants::buffer_sample_frames
			, &callbacks::AudioOutCallback
			, user_data_for_callback
		);
		if (error) {
			std::println(stderr, "error opening a stream: {}/n", rt_audio_->getErrorText());
			throw;
		}
	}

public:
	[[nodiscard]] static std::optional<AudioOut> MakeAudioOut(synth::NoteBundle& t_note_bundle) {
		try {
			return std::optional<AudioOut>(AudioOut(t_note_bundle));
		} catch (...) {
			std::println(stderr, "error making audio out");
		}
		return std::nullopt;
	}

	bool PlayShit() {
		RtAudioErrorType error = rt_audio_->startStream();
		if (error) {
			std::println(stderr, "error starting a stream: {}/n", rt_audio_->getErrorText());

			if (rt_audio_->isStreamOpen()) {
				rt_audio_->closeStream();
			}
			return false;
		}
		return true;
	}

	void Stop() {
		if (rt_audio_->isStreamRunning()) {
			rt_audio_->stopStream();
		}
		if (rt_audio_->isStreamOpen()) {
			rt_audio_->closeStream();
		}
	}

private:
	// kept segfaulting so i had to do make it shared bruh
	std::shared_ptr<RtAudio> rt_audio_;
	synth::NoteBundle& note_bundle_;
};

}
