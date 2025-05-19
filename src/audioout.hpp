#pragma once

#include <RtAudio.h>
#include <cstdio>
#include <memory>
#include <optional>
#include <print>

#include "types_and_constants.hpp"
#include "midiin.hpp"
#include "voicemanager.hpp"

namespace synth {

class AudioOut {
private:
	AudioOut(VoiceManager& t_voice_manager)
		: voice_manager_(t_voice_manager)
	{
		rt_audio_ = std::make_shared<::RtAudio>();
		RtAudio::StreamParameters parameters;
		parameters.deviceId = rt_audio_->getDefaultOutputDevice();
		parameters.nChannels = 2;

		// RtAudio::DeviceInfo info;
		// info = rt_audio_.getDeviceInfo(parameters.deviceId);
		// std::println("device name: {}", info.name);
		// std::println("number of output channels: {}", info.outputChannels);

		void* user_data_for_callback = static_cast<void*>(&t_voice_manager);

		RtAudioErrorType error = rt_audio_->openStream(
			&parameters
			, nullptr
			, RTAUDIO_FLOAT32
			, constants::sample_rate_hz
			, &constants::buffer_sample_frames
			, &synth::callbacks::AudioOutCallback
			, user_data_for_callback
		);
		if (error) {
			std::println(stderr, "error opening a stream: {}/n", rt_audio_->getErrorText());
			throw "bruh";
		}
	}

public:
	[[nodiscard]] static std::optional<AudioOut> MakeAudioOut(VoiceManager& voice_manager) {
		try {
			return std::optional<AudioOut>(AudioOut(voice_manager));
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
	synth::VoiceManager& voice_manager_;
};

}
