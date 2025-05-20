cmake_minimum_required(VERSION 4.0)

project(synth)

set (CMAKE_CXX_STANDARD 23)
# for clangd lsp to work
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(run src/main.cpp)

# gcem
find_package(gcem REQUIRED)
message(STATUS "gcem found: ${gcem_FOUND}")
target_include_directories(gcem INTERFACE ${gcem_INCLUDE_DIRS})

# rtmidi and rtaudio (they dont have a find package module fuck my life)
find_library(RTMIDI_LIBRARY rtmidi)
find_path(RTMIDI_INCLUDE_DIR include/rtmidi)

message(STATUS "rtmidi library path: ${RTMIDI_LIBRARY}")
message(STATUS "rtmidi include path: ${RTMIDI_INCLUDE_DIR}/include/rtmidi/")

find_library(RTAUDIO_LIBRARY rtaudio)
find_path(RTAUDIO_INCLUDE_DIR include/rtaudio)

message(STATUS "rtaudio library path: ${RTAUDIO_LIBRARY}")
message(STATUS "rtaudio include path: ${RTAUDIO_INCLUDE_DIR}/include/rtaudio/")

target_include_directories(run PUBLIC
	${RTMIDI_INCLUDE_DIR}/include/rtmidi
	${RTAUDIO_INCLUDE_DIR}/include/rtaudio
)

target_link_libraries(run PRIVATE
	${RTMIDI_LIBRARY}	
	${RTAUDIO_LIBRARY}	
)
