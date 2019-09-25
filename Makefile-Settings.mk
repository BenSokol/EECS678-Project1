# @Filename: Makefile-Settings.mk
# @Author:   Ben Sokol <Ben>
# @Email:    ben@bensokol.com
# @Created:  February 14th, 2019 [5:21pm]
# @Modified: September 25th, 2019 [7:05pm]
# @Version:  1.0.0
#
# Copyright (C) 2019 by Ben Sokol. All Rights Reserved.

###############################################################################
# Makefile Settings                                                           #
###############################################################################
EXE = quash

ifeq ($(shell hostname | head -c5),cycle)
CXXSTD = -std=c++1z
else
CXXSTD = -std=c++17
endif

CC=clang
CXX=clang++
CCSTD = -std=c11
CFLAGS = -O3 -g -DNDEBUG
CXXFLAGS = -O3 -g -DNDEBUG
WARNING_FLAGS = -Wall -Wextra -Wpedantic
WARNING_FLAGS_GCC =
WARNING_FLAGS_CLANG = -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-date-time

MACOS_FLAGS =
LINUX_FLAGS = -pthread

ENABLE_LOGGING = 1

#CPPCHECK_DEFINES = -D__cplusplus -DENABLE_LOGGING
CPPCHECK_SUPPRESS = --suppress=missingIncludeSystem --suppress=preprocessorErrorDirective:submodules/utilities/UTL_textWrap.cpp:17
CPPCHECK_FLAGS = -q --platform=native --error-exitcode=1 --force --std=c11 --std=c++14
CPPCHECK_ENABLE = --enable=warning --enable=style --enable=missingInclude
