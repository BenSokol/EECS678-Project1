# @Filename: Makefile-Settings.mk
# @Author:   Ben Sokol <Ben>
# @Email:    ben@bensokol.com
# @Created:  February 14th, 2019 [5:21pm]
# @Modified: October 20th, 2019 [1:23am]
# @Version:  1.0.0
#
# Copyright (C) 2019 by Ben Sokol. All Rights Reserved.

###############################################################################
# Makefile Settings                                                           #
###############################################################################
EXE = quash

ifeq ($(shell hostname | head -c5),cycle)
CC=/home/b843s521/gcc-9.2.0/bin/gcc-9.2
CXX=/home/b843s521/gcc-9.2.0/bin/g++-9.2
CFLAGS += -I/home/b843s521/gcc-9.2.0/include -L/home/b843s521/gcc-9.2.0/lib -static-libstdc++
CXXFLAGS += -I/home/b843s521/gcc-9.2.0/include -L/home/b843s521/gcc-9.2.0/lib -static-libstdc++
endif

CCSTD = -std=c11
CXXSTD = -std=c++2a
WARNING_FLAGS += -Wall -Wextra -Wpedantic
WARNING_FLAGS_GCC +=
WARNING_FLAGS_CLANG += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-date-time -Wno-exit-time-destructors -Wno-global-constructors -Wno-disabled-macro-expansion -Wno-switch-enum -Wno-unreachable-code-break -Wno-unreachable-code-return -Wno-missing-declarations

MACOS_FLAGS +=
LINUX_FLAGS += -pthread

CPPCHECK_SUPPRESS = --suppress=missingIncludeSystem --suppress=preprocessorErrorDirective:submodules/utilities/UTL_textWrap.cpp:18 --suppress=preprocessorErrorDirective:submodules/debug/DBG_out.hpp:36
CPPCHECK_FLAGS = -q --platform=native --error-exitcode=1 --force --std=c11 --std=c++14
CPPCHECK_ENABLE = --enable=warning --enable=style --enable=missingInclude
