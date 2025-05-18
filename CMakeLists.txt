cmake_minimum_required(VERSION 4.0)

project(synth)

set (CMAKE_CXX_STANDARD 23)
# for clangd lsp to work
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(run src/main.cpp)

# i know this is horrible but this is the only way i could get it to work
# im done fucking with cmake for now

# gcem
add_library(gcem INTERFACE)
target_include_directories(gcem INTERFACE /opt/homebrew/Cellar/gcem/1.18.0/include/)

# rt shit
target_include_directories(run PUBLIC
	/opt/homebrew/Cellar/rtmidi/6.0.0/include/rtmidi/
	/opt/homebrew/Cellar/rtaudio/6.0.1/include/rtaudio/
)
target_link_directories(run PUBLIC
	/opt/homebrew/Cellar/rtmidi/6.0.0/lib/
	/opt/homebrew/Cellar/rtaudio/6.0.1/lib/
)

target_link_libraries(run PRIVATE
	gcem
	rtmidi
	rtaudio
)
