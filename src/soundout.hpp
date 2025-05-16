#include <RtAudio.h>
#include <cstdint>
#include <print>
#include <numbers>

const unsigned int g_sample_rate_hz = 44100;
const unsigned short g_freq_hz = 400;
// it was loud as fuck, i dont know whats wrong with it
const double g_amplitude = 0.00001;

int sinwave(void *outputbuf, void* /*inputbuf*/, unsigned int buffer_frames
	, double stream_time, RtAudioStreamStatus stream_status, void* user_data) {

	double* buf = static_cast<double*>(outputbuf);

	double sin_arg = 2 * std::numbers::pi * g_freq_hz;
	double cur_time_seconds;
	std::uint32_t cur_time_samples = static_cast<std::uint32_t>(stream_time * g_sample_rate_hz);

	for(std::size_t i = 0; i < buffer_frames; ++i) {
		cur_time_seconds = static_cast<double>(cur_time_samples)/ g_sample_rate_hz;
		*buf = std::sin(sin_arg * cur_time_seconds) * g_amplitude;
		++cur_time_samples;
		++buf;
	}

	return 0;
}

int main() {
	RtAudio audio;
	RtAudio::DeviceInfo device_info;

	// for (std::uint32_t device_id : audio.getDeviceIds()) {
	// 	device_info = audio.getDeviceInfo(device_id);
	// 	std::println("device name: ", device_info.name);
	// 	std::println("device id: {}", device_info.ID);
	// 	std::println("number of output channels : {}\n", device_info.outputChannels);
	// }
	// std::println("no suitable devices");
	// return 0;

	RtAudio::StreamParameters parameters;
	parameters.deviceId = audio.getDefaultOutputDevice();
	parameters.nChannels = 2;
	unsigned int bufferFrames = 256; // 256 sample frames
	uint32_t cur_size_samples = 0;
	void* user_data = static_cast<void*>(&cur_size_samples);

	RtAudio::StreamOptions options;
	// options.flags = RTAUDIO_NONINTERLEAVED;

	RtAudioErrorType error;
	error = audio.openStream(
			&parameters, nullptr, RTAUDIO_FLOAT32, g_sample_rate_hz, 
			&bufferFrames, &sinwave, user_data);
	if (error) {
		std::println("error opening a stream: {}/n", audio.getErrorText());
		return 0;
	}

	error = audio.startStream();
	if (error) {
		std::println("error starting a stream: {}/n", audio.getErrorText());
		if (audio.isStreamOpen()) {
			audio.closeStream();
		}
		return 0;
	}

	char input;
	std::println("\nPlaying ... press <enter> to quit.\n");
	std::cin.get(input);

	if (audio.isStreamRunning()) {
		audio.stopStream();
	}

	if (audio.isStreamOpen()) {
		audio.closeStream();
	}
 
	return 0;
}
