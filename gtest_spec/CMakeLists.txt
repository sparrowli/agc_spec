# CMake requirement for minimum version
cmake_minimum_required (VERSION 2.8)

# project information
project (test_spec)

aux_source_directory(. DIR_SRCS)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

set (PROJECT_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/audio_processing_impl.cc
	${PROJECT_SOURCE_DIR}/gain_control_impl.cc
	${PROJECT_SOURCE_DIR}/digital_agc/digital_agc.c
	${PROJECT_SOURCE_DIR}/digital_agc/analog_agc.c
	${PROJECT_SOURCE_DIR}/digital_agc/signal_processing_library.c
	${PROJECT_SOURCE_DIR}/common_audio/wav_file.cc
	${PROJECT_SOURCE_DIR}/common_audio/wav_header.cc
	${PROJECT_SOURCE_DIR}/base/checks.cc
	${PROJECT_SOURCE_DIR}/base/numerics/safe_compare_unittest.cc
	${PROJECT_SOURCE_DIR}/tools/input_audio_file.cc
	${PROJECT_SOURCE_DIR}/tools/input_audio_file_unittest.cc
	${CMAKE_CURRENT_SOURCE_DIR}/test_spec.cc
	)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

find_library (
    GTEST_LIB
    NAMES gtest libgtest # what to look for
    HINTS "${PROJECT_SOURCE_DIR}/lib" # where to look
    NO_DEFAULT_PATH # do not search system default paths
)

message(STATUS "GTEST_LIB: [${GTEST_LIB}]")

include_directories (${PROJECT_SOURCE_DIR})
link_directories (${PROJECT_SOURCE_DIR}/lib)

add_executable (test_spec ${PROJECT_SOURCE_FILES})
#target_link_libraries(test_spec PUBLIC ${GTEST_LIB})
target_link_libraries(test_spec PUBLIC gtest)
