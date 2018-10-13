# CMake requirement for minimum version
cmake_minimum_required (VERSION 2.8)

# project information
project (test_spec)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")
aux_source_directory(. DIR_SRCS)

message(STATUS "CMAKE_SYSTEM_VERSION: [${CMAKE_SYSTEM_VERSION}")

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

if (APPLE)
    set (FOUNDATION "-framework Foundation")
    message(STATUS "FOUNDATION: ${FOUNDATION}")
endif()

if (MSVC)
else()
    option (WEBRTC_POSIX "not windows" ON)
    add_definitions(-DWEBRTC_POSIX -DWEBRTC_MAC)
    message(STATUS "WEBRTC_POSIX: ${WEBRTC_POSIX}")
endif()

#configure_file (
#  "${PROJECT_SOURCE_DIR}/build_config.h.in"
#  "${PROJECT_BINARY_DIR}/build_config.h"
#)
#include_directories ("${PROJECT_BINARY_DIR}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/abseil-cpp)

set (PROJECT_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/audio_processing_impl.cc
	${PROJECT_SOURCE_DIR}/audio_buffer.cc
	${PROJECT_SOURCE_DIR}/audio_buffer_unittest.cc
	${PROJECT_SOURCE_DIR}/gain_control_impl.cc
	${PROJECT_SOURCE_DIR}/splitting_filter.cc
	${PROJECT_SOURCE_DIR}/three_band_filter_bank.cc
	${PROJECT_SOURCE_DIR}/digital_agc/digital_agc.c
	${PROJECT_SOURCE_DIR}/digital_agc/analog_agc.c
	${PROJECT_SOURCE_DIR}/common_audio/division_operations.c
	${PROJECT_SOURCE_DIR}/common_audio/splitting_filter.c
	${PROJECT_SOURCE_DIR}/common_audio/spl_sqrt.c
	${PROJECT_SOURCE_DIR}/common_audio/spl_sqrt_floor.c
	
	${PROJECT_SOURCE_DIR}/common_audio/copy_set_operations.c
	${PROJECT_SOURCE_DIR}/common_audio/dot_product_with_scale.cc
	${PROJECT_SOURCE_DIR}/common_audio/audio_util.cc
	${PROJECT_SOURCE_DIR}/common_audio/audio_util_unittest.cc
	${PROJECT_SOURCE_DIR}/common_audio/channel_buffer.cc
	${PROJECT_SOURCE_DIR}/common_audio/channel_buffer_unittest.cc
	${PROJECT_SOURCE_DIR}/common_audio/sparse_fir_filter.cc
	${PROJECT_SOURCE_DIR}/common_audio/wav_file.cc
	${PROJECT_SOURCE_DIR}/common_audio/wav_header.cc
	${PROJECT_SOURCE_DIR}/common_audio/resample.c
	${PROJECT_SOURCE_DIR}/common_audio/resample_48khz.c
	${PROJECT_SOURCE_DIR}/common_audio/resample_by_2.c
	${PROJECT_SOURCE_DIR}/common_audio/resample_by_2_internal.c
	${PROJECT_SOURCE_DIR}/common_audio/resample_fractional.c
	${PROJECT_SOURCE_DIR}/common_audio/resampler/push_resampler.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/push_resampler_unittest.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/push_sinc_resampler.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/push_sinc_resampler_unittest.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/resampler.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/resampler_unittest.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/sinc_resampler.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/sinc_resampler_sse.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/sinc_resampler_unittest.cc
	${PROJECT_SOURCE_DIR}/common_audio/resampler/sinusoidal_linear_chirp_source.cc



	${PROJECT_SOURCE_DIR}/api/audio/audio_frame.cc
	${PROJECT_SOURCE_DIR}/api/audio/audio_frame_unittest.cc
	${PROJECT_SOURCE_DIR}/api/audio_codecs/audio_format.cc
	${PROJECT_SOURCE_DIR}/api/units/time_delta.cc
	${PROJECT_SOURCE_DIR}/api/units/time_delta_unittest.cc
	${PROJECT_SOURCE_DIR}/api/video/video_bitrate_allocation.cc
	${PROJECT_SOURCE_DIR}/base/memory/aligned_malloc.cc
	${PROJECT_SOURCE_DIR}/base/memory/aligned_malloc_unittest.cc
	${PROJECT_SOURCE_DIR}/base/memory/aligned_array_unittest.cc
	${PROJECT_SOURCE_DIR}/base/numerics/safe_compare_unittest.cc
	${PROJECT_SOURCE_DIR}/base/numerics/safe_minmax_unittest.cc
	${PROJECT_SOURCE_DIR}/base/strings/audio_format_to_string.cc
	${PROJECT_SOURCE_DIR}/base/strings/string_builder.cc
	${PROJECT_SOURCE_DIR}/base/strings/string_builder_unittest.cc
	${PROJECT_SOURCE_DIR}/base/logging.cc
	${PROJECT_SOURCE_DIR}/base/nullsocketserver.cc
	${PROJECT_SOURCE_DIR}/base/criticalsection.cc
	${PROJECT_SOURCE_DIR}/base/timeutils.cc
	${PROJECT_SOURCE_DIR}/base/location.cc
	${PROJECT_SOURCE_DIR}/base/messagequeue.cc
	${PROJECT_SOURCE_DIR}/base/messagehandler.cc
	${PROJECT_SOURCE_DIR}/base/thread.cc
	${PROJECT_SOURCE_DIR}/base/thread_darwin.mm
	${PROJECT_SOURCE_DIR}/base/thread_checker_impl.cc
	${PROJECT_SOURCE_DIR}/base/platform_thread.cc
	${PROJECT_SOURCE_DIR}/base/signalthread.cc
	${PROJECT_SOURCE_DIR}/base/nethelpers.cc
	${PROJECT_SOURCE_DIR}/base/ipaddress.cc
	${PROJECT_SOURCE_DIR}/base/socketaddress.cc
	${PROJECT_SOURCE_DIR}/base/socket.cc
	${PROJECT_SOURCE_DIR}/base/asyncsocket.cc
	${PROJECT_SOURCE_DIR}/base/physicalsocketserver.cc
	${PROJECT_SOURCE_DIR}/base/asyncpacketsocket.cc
	${PROJECT_SOURCE_DIR}/base/socketaddresspair.cc
	${PROJECT_SOURCE_DIR}/base/virtualsocketserver.cc
	${PROJECT_SOURCE_DIR}/base/asynctcpsocket.cc
	${PROJECT_SOURCE_DIR}/base/asyncudpsocket.cc
	${PROJECT_SOURCE_DIR}/base/asyncudpsocket_unittest.cc
	${PROJECT_SOURCE_DIR}/base/asyncresolverinterface.cc
	${PROJECT_SOURCE_DIR}/base/logging_mac.mm
	${PROJECT_SOURCE_DIR}/base/event.cc
	${PROJECT_SOURCE_DIR}/base/event_tracer.cc
	${PROJECT_SOURCE_DIR}/base/atomicops_unittest.cc
	${PROJECT_SOURCE_DIR}/base/strings/string_builder_unittest.cc
	${PROJECT_SOURCE_DIR}/base/criticalsection_unittest.cc
	${PROJECT_SOURCE_DIR}/base/ipaddress_unittest.cc
	${PROJECT_SOURCE_DIR}/base/bind_unittest.cc
	${PROJECT_SOURCE_DIR}/base/refcountedobject_unittest.cc
	${PROJECT_SOURCE_DIR}/base/asyncinvoker.cc
	${PROJECT_SOURCE_DIR}/base/thread_unittest.cc
	${PROJECT_SOURCE_DIR}/base/testutils.cc
	${PROJECT_SOURCE_DIR}/base/physicalsocketserver_unittest.cc
	${PROJECT_SOURCE_DIR}/base/zero_memory.cc
	${PROJECT_SOURCE_DIR}/base/zero_memory_unittest.cc
	${PROJECT_SOURCE_DIR}/base/stringencode_unittest.cc
	${PROJECT_SOURCE_DIR}/base/stringutils_unittest.cc
	${PROJECT_SOURCE_DIR}/base/byteorder_unittest.cc
	${PROJECT_SOURCE_DIR}/base/nullsocketserver_unittest.cc
	${PROJECT_SOURCE_DIR}/base/helpers.cc
	${PROJECT_SOURCE_DIR}/base/helpers_unittest.cc
	${PROJECT_SOURCE_DIR}/base/openssldigest.cc
	${PROJECT_SOURCE_DIR}/base/messagedigest.cc
	${PROJECT_SOURCE_DIR}/base/copyonwritebuffer.cc
	${PROJECT_SOURCE_DIR}/base/rtccertificate.cc
	${PROJECT_SOURCE_DIR}/base/sslfingerprint.cc
	${PROJECT_SOURCE_DIR}/base/opensslutility.cc
	${PROJECT_SOURCE_DIR}/base/opensslutility_unittest.cc
	${PROJECT_SOURCE_DIR}/base/opensslcertificate.cc
	${PROJECT_SOURCE_DIR}/base/sslcertificate.cc
	${PROJECT_SOURCE_DIR}/base/opensslidentity.cc
	${PROJECT_SOURCE_DIR}/base/base64.cc
	${PROJECT_SOURCE_DIR}/base/sslidentity.cc
	${PROJECT_SOURCE_DIR}/base/testclient.cc
	${PROJECT_SOURCE_DIR}/base/testechoserver.cc
	${PROJECT_SOURCE_DIR}/base/opensslstreamadapter.cc
	${PROJECT_SOURCE_DIR}/base/sslstreamadapter.cc
	${PROJECT_SOURCE_DIR}/base/ssladapter.cc
	${PROJECT_SOURCE_DIR}/base/opensslsessioncache.cc
	${PROJECT_SOURCE_DIR}/base/openssladapter.cc
	${PROJECT_SOURCE_DIR}/base/ssladapter_unittest.cc
	${PROJECT_SOURCE_DIR}/base/socketstream.cc
	${PROJECT_SOURCE_DIR}/base/stream.cc
	${PROJECT_SOURCE_DIR}/base/stream_unittest.cc
	${PROJECT_SOURCE_DIR}/base/numerics/safe_compare_unittest.cc
	${PROJECT_SOURCE_DIR}/base/numerics/safe_minmax_unittest.cc
	${PROJECT_SOURCE_DIR}/base/fakeclock.cc
	${PROJECT_SOURCE_DIR}/base/virtualsocket_unittest.cc
	${PROJECT_SOURCE_DIR}/base/asynctcpsocket_unittest.cc
	${PROJECT_SOURCE_DIR}/base/buffer_unittest.cc
	${PROJECT_SOURCE_DIR}/base/thread_checker_unittest.cc
	${PROJECT_SOURCE_DIR}/base/socket_unittest.cc
	${PROJECT_SOURCE_DIR}/base/event_unittest.cc
	${PROJECT_SOURCE_DIR}/base/logging_unittest.cc
	${PROJECT_SOURCE_DIR}/base/testclient_unittest.cc
	${PROJECT_SOURCE_DIR}/base/platform_thread_unittest.cc
	${PROJECT_SOURCE_DIR}/base/signalthread_unittest.cc
	${PROJECT_SOURCE_DIR}/base/timeutils_unittest.cc
	${PROJECT_SOURCE_DIR}/base/thread_annotations_unittest.cc
	${PROJECT_SOURCE_DIR}/base/stringize_macros_unittest.cc
	${PROJECT_SOURCE_DIR}/base/string_to_number_unittest.cc
	${PROJECT_SOURCE_DIR}/base/socketaddress_unittest.cc
	${PROJECT_SOURCE_DIR}/base/event_tracer_unittest.cc
	${PROJECT_SOURCE_DIR}/base/messagequeue_unittest.cc
	
	${PROJECT_SOURCE_DIR}/base/copyonwritebuffer_unittest.cc
	${PROJECT_SOURCE_DIR}/base/sslstreamadapter_unittest.cc
	${PROJECT_SOURCE_DIR}/base/bufferqueue.cc
	${PROJECT_SOURCE_DIR}/base/bufferqueue_unittest.cc
	${PROJECT_SOURCE_DIR}/base/rtccertificate_unittest.cc
	${PROJECT_SOURCE_DIR}/base/openssladapter_unittest.cc
	${PROJECT_SOURCE_DIR}/base/messagedigest_unittest.cc
	${PROJECT_SOURCE_DIR}/base/opensslsessioncache_unittest.cc
	${PROJECT_SOURCE_DIR}/base/sslidentity_unittest.cc
	${PROJECT_SOURCE_DIR}/base/fakesslidentity.cc
	
	${PROJECT_SOURCE_DIR}/base/checks.cc
	${PROJECT_SOURCE_DIR}/base/gunit.cc
	${PROJECT_SOURCE_DIR}/base/string_to_number.cc
	${PROJECT_SOURCE_DIR}/base/string_to_number_unittest.cc
	${PROJECT_SOURCE_DIR}/base/stringencode.cc
	${PROJECT_SOURCE_DIR}/base/stringutils.cc
	${PROJECT_SOURCE_DIR}/base/stringutils_unittest.cc
    ${PROJECT_SOURCE_DIR}/base/platform_thread_types.cc
    ${PROJECT_SOURCE_DIR}/system_wrappers/source/sleep.cc
    ${PROJECT_SOURCE_DIR}/system_wrappers/source/cpu_features.cc
    ${PROJECT_SOURCE_DIR}/test/testsupport/fileutils.cc
    ${PROJECT_SOURCE_DIR}/test/testsupport/fileutils_unittest.cc
	${PROJECT_SOURCE_DIR}/tools/input_audio_file.cc
	${PROJECT_SOURCE_DIR}/tools/input_audio_file_unittest.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/test_spec.cc
	)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
message(STATUS "CMAKE_CXX_FLAGS: [${CMAKE_CXX_FLAGS}]")

find_package(
    openssl 
    REQUIRED
)
message(STATUS "OPENSSL_LIB: [${OPENSSL_LIB}]")

find_library (
	OPENSSL_CRYPTO
		NAMES crypto
)

find_library (
    OPENSSL
		NAMES ssl
)

find_library (
    GTEST_LIB
    NAMES gtest libgtest # what to look for
    HINTS "${PROJECT_SOURCE_DIR}/lib" # where to look
    NO_DEFAULT_PATH # do not search system default paths
)

message(STATUS "OPENSSL_CRYPTO: [${OPENSSL_CRYPTO}]")
message(STATUS "OPENSSL: [${OPENSSL}]")
message(STATUS "GTEST_LIB: [${GTEST_LIB}]")

include_directories (${PROJECT_SOURCE_DIR}
  "${PROJECT_SOURCE_DIR}/include"
	"/usr/local/opt/openssl/"
)

link_directories (${PROJECT_SOURCE_DIR}/lib)

add_executable (test_spec ${PROJECT_SOURCE_FILES})
#target_link_libraries(test_spec PUBLIC ${GTEST_LIB})
#target_link_libraries(test_spec PUBLIC gtest)
target_link_libraries(test_spec PUBLIC gtest gmock ${FOUNDATION} ${OPENSSL_CRYPTO} ${OPENSSL} absl::base absl::optional absl::strings absl::memory)
