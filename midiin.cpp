#include <memory>
#include <cstdlib>
#include <csignal>
#include <thread>
#include <RtMidi.h>

// #include "midi-parser.hpp"

int main() {
	// TODO: learn the error handling, but for now fine, exceptions

	std::shared_ptr<RtMidiIn> midiin;
	// RtMidiIn constructor
	try {
		midiin = std::make_shared<RtMidiIn>();
	}
	catch ( RtMidiError &error ) {
		error.printMessage();
		return 0;
	}

	midiin->openVirtualPort();
	//  // Don't ignore sysex, timing, or active sensing messages.
	midiin->ignoreTypes( false, false, false );

	// midiin->setCallback([](double deltatime, std::vector<unsigned char>* message, void* userData){
	// midiin->setCallback([](double, std::vector<unsigned char>* message, void*){
	// 		// std::print("{:08b}", *message);
	// 		for (char ch : *message) {
	// 			std::print("{:08b} ", ch);
	// 		}
	// 		std::print("\n");
	// 		// flush here?
	// });

	// char input;
	// while(input != 'q') {
	// 	std::cin.get(input);
	// }

	static bool done = false;
	// (void) signal(SIGINT, finish);
	std::signal(SIGINT, [](int) {
		done = true;
	});

	std::vector<unsigned char> message;
	// std::println("sizeof(unsigned char) {}", sizeof(unsigned char));
	while (!done) {
		while (message.empty()) {
			midiin->getMessage(&message);
		}
		// std::print("message.size(): {}\n", message.size());
		// message.clear();

		// Sleep for 10 milliseconds ... platform-dependent.
		using namespace std::chrono;
		std::this_thread::sleep_for(10ms);
	}

	return 0;
}
