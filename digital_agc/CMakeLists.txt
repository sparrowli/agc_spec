#cmake_minimum_required(VERSION 2.8)

#project (digital_agc)

add_library(digital_agc digital_agc.c analog_agc.c signal_processing_library.c)

