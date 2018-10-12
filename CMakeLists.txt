# CMake requirement for minimum version
cmake_minimum_required (VERSION 2.8)

# project information
project (audio_processing)

aux_source_directory(. DIR_SRCS)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (PROJECT_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/audio_processing_impl.cc
	${PROJECT_SOURCE_DIR}/gain_control_impl.cc
	${PROJECT_SOURCE_DIR}/digital_agc/digital_agc.c
	${PROJECT_SOURCE_DIR}/digital_agc/analog_agc.c
	${PROJECT_SOURCE_DIR}/digital_agc/signal_processing_library.c
	${PROJECT_SOURCE_DIR}/common_audio/wav_file.cc
	${PROJECT_SOURCE_DIR}/common_audio/wav_header.cc
	)

include_directories (${PROJECT_SOURCE_DIR})

add_library (audio_processing ${PROJECT_SOURCE_FILES})
