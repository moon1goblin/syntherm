#pragma once

#include <RtAudio.h>
#include <cstdio>
#include <expected>
#include <memory>
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
		// i don think this throws, but just to be safe
		try {
			rt_audio_ = std::make_unique<::RtAudio>();
		} catch (...) {
			throw;
		}

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
			throw std::string("error opening a stream: {}\n" + rt_audio_->getErrorText());
		}
	}

public:
	[[nodiscard]] static std::expected<AudioOut, std::string> CreateAudioOut(VoiceManager& voice_manager) {
		try {
			return AudioOut(voice_manager);
		} catch (const std::string& error) {
			return std::unexpected(error);
		} catch (...) {
			return std::unexpected("error constructing an RtAudio instance");
		}
	}

	std::expected<void, std::string> 
	PlayShit() {
		RtAudioErrorType error = rt_audio_->startStream();
		if (error) {
			if (rt_audio_->isStreamOpen()) {
				rt_audio_->closeStream();
			}
			return std::unexpected("error starting a stream: {}\n" + rt_audio_->getErrorText());
		}
		return {};
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
	std::unique_ptr<RtAudio> rt_audio_;
	synth::VoiceManager& voice_manager_;
};

}
